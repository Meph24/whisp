#include "SimulationServer.hpp"

#include <algorithm>

#include "SyncableManager.h"

using std::min;

SimulationServer::SimulationServer(WallClock& wc, Cfg& cfg, Port port)
    : wc(wc)
    , cfg(cfg)
    , listener(port)
    , initial_processor(*this, listener, wc)
    , last_sent(wc.now())
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
        clients.addClient(std::move(c));
    }
}

void SimulationServer::process()
{
    if(!syncman) return;
    sf::Packet packet;

    while(syncman->fetchEventPackets(packet))
    {
        broadcastTcp(packet);
    }

    processIncomingConnections();

    if(!clients.connections.empty())
    {
        unique_ptr<syncprotocol::udp::Packet> udp_packet = std::make_unique<syncprotocol::udp::Packet>();
        //bytes allowed to send
        FloatSeconds since_last_sent ( min(wc.now() - last_sent, WallClock::duration(1s)) );
        float bytes_allowed = min( 
            (float)since_last_sent * max_send_bytes_per_second,
            (float)sf::UdpSocket::MaxDatagramSize
            ) - udp_packet->getDataSize();

        if( syncman->fillUpdatePacket(*udp_packet, bytes_allowed) )
        {
            broadcastUdp(std::move(udp_packet));
            last_sent = wc.now();
        }

        receiveClientInputs();
    }
}

void SimulationServer::receiveClientInputs()
{
    sf::Packet newpacket;
    sf::IpAddress remote_addr; Port remote_port;
    while(udpsocket.receive(newpacket, remote_addr, remote_port) == sf::Socket::Status::Done )
    {
        if(! clients.containsUdp(remote_addr, remote_port) ) continue;

        newpacket >> clients.atUdp(remote_addr, remote_port)->user.input_status;
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
        c->tcpsocket.send(p);
    }
}

void SimulationServer::broadcastUdp(unique_ptr<syncprotocol::udp::Packet>&& packet)
{
    syncprotocol::udp::Header header; header.server_time = wc.now();
    for(auto& c : clients.connections)
    {
        header.client_time = c->last_client_time;
        udpsocket.send(*packet, c->tcpsocket.getRemoteAddress(), c->udpport);
    }
    packet.reset();
}
