#ifndef SIMULATIONSERVER_HPP
#define SIMULATIONSERVER_HPP

#include "ClientConnectionListing.hpp"
#include "ConnectionListener.hpp"
#include "ConnectionInitialProcessor.hpp"
#include "UdpServerProcessor.hpp"

struct Cfg;

struct SimulationServer
{
    WallClock& wc;
    Cfg& cfg;
    ConnectionListener listener;
    ConnectionInitialProcessor initial_processor;

    sf::UdpSocket udpsocket;

    syncprotocol::ServerInfo info;

    ClientConnectionListing clients;

    SyncableManager* syncman;

    void processIncomingConnections();
    void process();

    void setup(SyncableManager& syncable_manager);

    SimulationServer(WallClock& wc, Cfg& cfg, Port port);
    ~SimulationServer() = default;

private:
    WallClock::time_point last_sent;
    float max_send_bytes_per_second = 3000;
    void broadcastTcp(sf::Packet&);
    void broadcastUdp(unique_ptr<syncprotocol::udp::Packet>&&);

    void receiveClientInputs();
};

#endif // SIMULATIONSERVER_HPP
