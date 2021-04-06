#include "ServerConnection.hpp"

#include <iostream> //TODO replace with logging #56

#include "Cfg.hpp"
#include "SimulationClient.hpp"

ServerConnection::ServerConnection(SimulationClient& client, Cfg& cfg)
    : client(client)
{
    Port this_udp_port = *cfg.getInt("client", "default_udp_port");
    udpsocket.bind( this_udp_port );
    std::cout << "Udp port bound to " << udpsocket.getLocalPort() << '\n';
    udpsocket.setBlocking(false);
}

bool ServerConnection::tryConnect( WallClock& wc, Cfg& cfg, const IPv4Address& addr, Port port )
{

    if(is_connected) return false;
    std::cout << "Connecting to " << addr << ':' << port << '\n';

    tcpsocket.setBlocking(true);
    is_connected = sf::Socket::Status::Done == tcpsocket.connect(
            sf::IpAddress( (uint32_t)addr ), port, sf::seconds(5) );
    if(!is_connected)
    {
        std::cout << "tryConnect: connection attempt failed!\n";
        return false;
    }

    std::cout << "Socket connection established\nIntroduction with ";

    syncprotocol::ClientIntroduction introduction(client.name, client.connection.udpsocket.getLocalPort());
    std::cout << introduction << " ... ";

    tcpsocket.setBlocking(true);
    if( tcpsocket.send(&introduction, sizeof(introduction)) != sf::Socket::Done ) return false;

    char introduction_response[syncprotocol::introduction_response_size];
    if( !syncprotocol::receiveInTime(   client.wc,
                                        tcpsocket, 
                                        introduction_response, 
                                        sizeof(introduction_response),
                                        10s) ) return false;
    if( std::strcmp(introduction_response, syncprotocol::introduction_success) ) 
    {
        std::cout << "FAILURE\n";
        return false;
    }
    std::cout << "SUCCESS\n";

    if( !syncprotocol::receiveInTime( client.wc, tcpsocket, (char*) &server_info, sizeof(server_info), 10s ) ) return false;

    std::cout << "Server Info:" << server_info << '\n';

    if( !syncprotocol::receiveInTime( client.wc, tcpsocket, (char*) &client_token, sizeof(client_token), 10s ) ) return false;

    std::cout << "Client Token:" << client_token << '\n';

    last_remote_received = wc.now();

    return is_connected;
}

void ServerConnection::disconnect()
{
    tcpsocket.disconnect();
    is_connected = false;
}

bool ServerConnection::connected() const 
{ 
    return is_connected;
}

bool ServerConnection::isTimedOut()
{
    if(! is_connected ) return true;

    if(!tcpsocket.getRemotePort()) 
    { 
        is_connected = false;
        return true;
    }

    if(client.wc.now() - last_remote_received > udp_remote_timeout_check_time)
    {
        bool blocking_status = tcpsocket.isBlocking();
        tcpsocket.setBlocking(false);
        //check for remote tcp status
        size_t dummy;
        if(tcpsocket.receive(&dummy, 0, dummy) == sf::Socket::Disconnected)
        {
            is_connected = false;
        }
        tcpsocket.setBlocking(blocking_status);
    }

    return !is_connected;
}

const WallClock::time_point& ServerConnection::latestServerTime() const { return latest_server_time; }
const WallClock::duration& ServerConnection::latency() const { return latency_; }

unique_ptr<sf::Packet> ServerConnection::receiveUdp()
{
    unique_ptr<sf::Packet> newp;
    newp = std::make_unique<sf::Packet>();
    sf::IpAddress sender_addr; Port sender_port;
    do
    {
        if(udpsocket.receive(*newp, sender_addr, sender_port) != sf::Socket::Status::Done )
        {
            return nullptr;
            break;
        }
    } while (       sender_addr != tcpsocket.getRemoteAddress() 
                ||  sender_port != server_info.udpport             );

    last_remote_received = client.wc.now();
    syncprotocol::udp::Header header; *newp >> header;
    latency_ = (header.client_time - client.wc.now()) / 2;
    if(latest_server_time < header.server_time) latest_server_time = header.server_time;

    return newp;
}

bool ServerConnection::sendUdp( syncprotocol::udp::Packet& p ) 
{ 
    return sf::Socket::Status::Done == udpsocket.send(p, tcpsocket.getRemoteAddress(), server_info.udpport); 
}