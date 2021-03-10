#include "SimulationServer.hpp"

#include "SyncableManager.h"

SimulationServer::SimulationServer(WallClock& wc, Cfg& cfg, Port port)
    : cfg(cfg)
    , listener(port)
    , initial_processor(*this, listener, wc)
    , udp(clients, cfg, wc)
{
    info.setName(*cfg.getStr("server", "name"));
    info.udpport = udp.socket.getLocalPort();
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
    std::cout << "Complete Packet is created!\n";
    for(auto& c : new_connections)
    {
        c->tcpsocket.send(packet);
        std::cout << "Complete Packet sent!\n";
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
        std::cout << "Event Packet broadcast!\n";
    }

    processIncomingConnections();

    if(!clients.connections.empty())
    {
        unique_ptr<syncprotocol::udp::Packet> udp_packet = std::make_unique<syncprotocol::udp::Packet>();
        if(syncman->fillUpdatePacket(*udp_packet, sf::UdpSocket::MaxDatagramSize - udp_packet->getDataSize() ))
            broadcastUdp(std::move(udp_packet));
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
    syncprotocol::udp::Packet* packet_ptr = packet.get();
    packet.release();
    shared_ptr<syncprotocol::udp::Packet> shared_packet (packet_ptr);
    for(auto& c : clients.connections)
    {
        c->sendUdp(shared_packet);
    }
}
