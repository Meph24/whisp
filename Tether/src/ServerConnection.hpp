#ifndef SERVERCONNECTION_HPP
#define SERVERCONNECTION_HPP

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include "protocol.hpp"

struct Cfg;
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
    bool connected() const;

    const WallClock::time_point& latestServerTime() const;
    const WallClock::duration& latency() const;

    bool sendUdp(syncprotocol::udp::Packet&);
    unique_ptr<sf::Packet> receiveUdp();

    ServerConnection(SimulationClient& client, Cfg& cfg);
private:
    bool is_connected = false;
    WallClock::duration latency_;
    WallClock::time_point latest_server_time;
};

#endif // SERVERCONNECTION_HPP
