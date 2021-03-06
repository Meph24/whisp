#include "SimulationServer.hpp"

#include <algorithm>

#include "SyncableManager.h"

using std::min;

SimulationServer::SimulationServer(WallClock& wc, Cfg& cfg, Port port)
    : wc(wc)
    , cfg(cfg)
    , listener(port)
    , initial_processor(*this, listener, wc)
    , upload_budget(wc, 50000)
{
    udpsocket.bind( *cfg.getInt("server", "default_udp_port") );
    udpsocket.setBlocking(false);
    info.setName(*cfg.getStr("server", "name"));
    info.udpport = udpsocket.getLocalPort();
}

void SimulationServer::processIncomingConnections()
{
    if(! initial_processor.hasProcessedConnections()) return;

    vector<unique_ptr<ClientConnection> >new_connections;
    while(initial_processor.hasProcessedConnections())
    {
        new_connections.emplace_back(initial_processor.nextConnection());
    }

    sf::Packet packet;
    syncman->fillCompletePacket(packet);
    for(auto& c : new_connections)
    {
        c->tcpsocket.send(packet);
        c->last_udp_received = wc.now();
        clients.addClient(std::move(c));
    }
}

void SimulationServer::processClosedConnections()
{
    auto& cs = clients.connections;

    //check for timeouts
    for(auto& c : cs)
    {
        if( wc.now() - c->last_udp_received > udp_remote_timeout_check_time)
        {
            //preserve blocking status of socket
            bool blocking_status = c->tcpsocket.isBlocking();
            c->tcpsocket.setBlocking(false);
            size_t dummy;
            //tcp connection is checked for disconnection, since we haven't heard from the client on udp
            if(c->tcpsocket.receive(&dummy, 0, dummy) == sf::Socket::Disconnected)
            {
                c->disconnected = true;
                std::cout << "Client connection " << c->tcpsocket.getRemoteAddress() << ':' << c->tcpsocket.getRemotePort() << " timed out!\n";
            }
            c->tcpsocket.setBlocking(blocking_status);
        }
    }

    //send reminders

    size_t initial_size = cs.size();
    cs.erase(
        std::remove_if(cs.begin(), cs.end(), 
            [](const unique_ptr<ClientConnection>& c)->bool{ return !c->tcpsocket.getRemotePort() || c->disconnected; }),
        cs.end()
    );
    size_t closed = initial_size-cs.size();
    if(closed) std::cout << "Removed " << closed << " closed connections!\n";
}

void SimulationServer::process()
{
    if(!syncman) return;
    sf::Packet packet;

    while(syncman->fetchEventPackets(packet))
    {
        broadcastTcp(packet);
        upload_budget.used(clients.connections.size() * packet.getDataSize());
    }

    processClosedConnections();
    processIncomingConnections();

    if(!clients.connections.empty())
    {
        const WallClock::time_point& server_time = wc.now();
        unique_ptr<syncprotocol::udp::Packet> udp_packet = std::make_unique<syncprotocol::udp::Packet>();
        i64 bytes = 0;
        if( syncman->fillUpdatePacket(*udp_packet, upload_budget.current() / clients.connections.size()) )
        {
            bytes = udp_packet->getDataSize() * clients.connections.size();
            broadcastUdp(std::move(udp_packet), server_time);
        }
        upload_budget.used(bytes);

        receiveClientInputs();
    }
}

#include "SimulationInputStatusSet.hpp"

void SimulationServer::receiveClientInputs()
{
    sf::Packet newpacket;
    sf::IpAddress remote_addr; Port remote_port;
    while(udpsocket.receive(newpacket, remote_addr, remote_port) == sf::Socket::Status::Done )
    {
        if(! clients.containsUdp(remote_addr, remote_port) ) continue;

        //extract header
        syncprotocol::udp::Header header; newpacket >> header;

        //friend access set latency
        //TODO make less direct accessly
        ClientConnection& client = *clients.atUdp(remote_addr, remote_port);
        client.last_udp_received = wc.now();
        client.latency_ = header.server_time - wc.now();
        client.last_client_time = max(header.client_time, client.last_client_time);

        SimulationInputStatusSet status_set;
        newpacket >> status_set;
        if(status_set.timestamp >= client.user.input_status.timestamp)
        {
            client.user.input_status = status_set;
        }
    }
}

void SimulationServer::setup(
    SyncableManager& syncable_manager
    )
{ 
    syncman = &syncable_manager; 
    initial_processor.start();
}

void SimulationServer::broadcastTcp(sf::Packet& p)
{
    for(auto& c : clients.connections)
    {
        c->disconnected = sf::Socket::Status::Disconnected == c->tcpsocket.send(p);
    }
}

void SimulationServer::broadcastUdp(unique_ptr<syncprotocol::udp::Packet>&& packet, const WallClock::time_point& server_time)
{
	syncID packID;//TODO remove after debugging start
	sf::Packet hackPack1;
	sf::Packet hackPack2;
	syncprotocol::udp::Header h;
	hackPack1<<h;
	hackPack1<<packID;
	size_t timestampSize=hackPack1.getDataSize();
	assert(packet.get()->getDataSize()>=timestampSize);
	hackPack2.append(packet.get()->getData(),timestampSize);
	hackPack2>>h;
	hackPack2>>packID;
	//TODO remove after debugging end

    syncprotocol::udp::Header header{ server_time, server_time };
    for(auto& c : clients.connections)
    {
        header.client_time = c->last_client_time;
        packet->setHeader(header);
        //std::cout<<"before UDP send (packetID="<<packID<<")"<<std::endl;//TODO remove after debugging
        udpsocket.send(*packet, c->tcpsocket.getRemoteAddress(), c->udpport);
        //std::cout<<"after UDP send (packetID="<<packID<<")"<<std::endl;//TODO remove after debugging
    }
    packet.reset();
}
