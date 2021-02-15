#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <memory>
#include <queue>

#include <SFML/Network.hpp>

#include "Cfg.hpp"
#include "IPv4Address.hpp"
#include "MainApp.h"
#include "protocol.hpp"
#include "WallClock.hpp"

using std::queue;
using std::unique_ptr;

using network::IPv4Address;
using network::Port;

struct SimulationClient;

struct ServerConnection
{
    sf::TcpSocket tcpsocket;
    SimulationClient& client;

    syncprotocol::ServerInfo server_info;
    syncprotocol::ClientToken client_token;

    sf::UdpSocket udpsocket;

    bool tryConnect(Cfg& cfg, const IPv4Address& addr, Port port);
    void disconnect();

    bool sendUdp(sf::Packet&);
    unique_ptr<sf::Packet> receiveUdp();

    ServerConnection(SimulationClient& client, Cfg& cfg);
private:
    bool is_connected = false;
    std::queue<unique_ptr<sf::Packet>> udp_backlog;
};

struct SimulationClient
{
    WallClock& wc;
    string name;
    ServerConnection connection;

    SimulationClient(WallClock& wc, Cfg& cfg);
    ~SimulationClient();
};

struct ClientApp : public App
{
    SimulationClient client;

    LocalUser user;

    ClientApp(WallClock& wc, Cfg& cfg, IPv4Address& addr, Port port);
    void run();
};

#endif // CLIENT_HPP
