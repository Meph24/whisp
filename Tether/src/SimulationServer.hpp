#ifndef SIMULATIONSERVER_HPP
#define SIMULATIONSERVER_HPP

#include "ClientConnectionListing.hpp"
#include "ConnectionListener.hpp"
#include "ConnectionInitialProcessor.hpp"
#include "UploadBudget.hpp"

struct Cfg;

struct SimulationServer
{
    WallClock& wc;
    Cfg& cfg;
    ConnectionListener listener;
    ConnectionInitialProcessor initial_processor;

    WallClock::duration udp_remote_timeout_check_time = 1s;
    sf::UdpSocket udpsocket;
    UploadBudget upload_budget;

    syncprotocol::ServerInfo info;

    ClientConnectionListing clients;

    SyncableManager* syncman;

    void processIncomingConnections();
    void processClosedConnections();
    void process();

    void setup(SyncableManager& syncable_manager);

    SimulationServer(WallClock& wc, Cfg& cfg, Port port);
    ~SimulationServer() = default;

private:
    void broadcastTcp(sf::Packet&);
    void broadcastUdp(unique_ptr<syncprotocol::udp::Packet>&&, const WallClock::time_point& server_time);

    void receiveClientInputs();
};

#endif // SIMULATIONSERVER_HPP
