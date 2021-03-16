#ifndef CLIENTCONNECTION_HPP
#define CLIENTCONNECTION_HPP

#include <memory>
#include <mutex>
#include <string>

#include <SFML/Network/TcpSocket.hpp>

#include "protocol.hpp"
#include "User.hpp"

using std::mutex;
using std::shared_ptr;
using std::string;

struct SyncUser : public SimulationUser
{
	void operateSimulation(Simulation& simulation);
	void disconnectSimulation();
};

struct ClientConnection
{
    string name;
    sf::TcpSocket tcpsocket;
    syncprotocol::ClientToken token;
    Port udpport;

    SyncUser user;

    FullIPv4 remoteUdpFullip() const;

    const WallClock::duration& latency() const;

    void receiveHeader(const syncprotocol::udp::Header& h);

private:
    WallClock::duration latency_;
    WallClock::time_point last_client_time;

    mutex udp_inbox_lock;
    mutex udp_outbox_lock;
    deque<unique_ptr<sf::Packet>  > udp_inbox; 
    deque<shared_ptr<syncprotocol::udp::Packet> > udp_outbox;

    friend struct UdpServerProcessor;
    friend struct SimulationServer;
};

#endif // CLIENTCONNECTION_HPP
