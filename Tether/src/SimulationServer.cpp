#include "SimulationServer.hpp"

#include "SyncableManager.h"

SimulationServer::SimulationServer(WallClock& wc, Cfg& cfg, Port port)
    : wc(wc)
    , cfg(cfg)
    , listener(port)
    , initial_processor(*this, listener, wc)
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
        if(syncman->fillUpdatePacket(*udp_packet, sf::UdpSocket::MaxDatagramSize - udp_packet->getDataSize() ))
        {
            broadcastUdp(std::move(udp_packet));
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
