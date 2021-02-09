#include "client.hpp"

#include <iostream> //TODO replace with logging #56

#include "protocol.hpp"

using std::cout; //TODO replace with logging #56

ServerConnection::ServerConnection(SimulationClient& client, Cfg& cfg)
    : client(client) 
{
    udpsocket.bind( *cfg.getInt("client", "default_udp_port") );
    cout << "Port bound to " << *cfg.getInt("client", "default_udp_port") << '\n';
}

bool ServerConnection::tryConnect( Cfg& cfg, const IPv4Address& addr, Port port )
{
    cout << "Connecting to " << addr << ':' << port << '\n';
    if(is_connected) return false;

    is_connected = tcpsocket.connect(sf::IpAddress( (uint32_t)addr ), port) == sf::Socket::Done;
    if(!is_connected) return false;

    cout << "Socket connection established\nIntroduction with ";

    syncprotocol::ClientIntroduction introduction(client.name, client.connection.udpsocket.getLocalPort());
    cout << introduction << " ... ";

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
        cout << "FAILURE\n";
        return false;
    }
    cout << "SUCCESS\n";

    if( !syncprotocol::receiveInTime( client.wc, tcpsocket, (char*) &server_info, sizeof(server_info), 10s ) ) return false;

    cout << "Server Info:" << server_info << '\n';

    if( !syncprotocol::receiveInTime( client.wc, tcpsocket, (char*) &client_token, sizeof(client_token), 10s ) ) return false;

    cout << "Client Token:" << client_token << '\n';

    udpsocket.bind( *cfg.getInt("client", "default_udp_port") );

    return is_connected;
}

void ServerConnection::disconnect()
{
    tcpsocket.disconnect();
    is_connected = false;
}

unique_ptr<sf::Packet> ServerConnection::receiveUdp()
{
    unique_ptr<sf::Packet> newp;
    if(!udp_backlog.empty())
    {
        newp = std::move(udp_backlog.front());
        udp_backlog.pop();
        return newp;
    }

    newp = std::make_unique<sf::Packet>();
    sf::IpAddress sender_addr; Port sender_port;
    do
    {
        if(udpsocket.receive(*newp, sender_addr, sender_port) != sf::Socket::Status::Done )
        {
            newp.reset();
            return newp;
        }
    } while (sender_addr != tcpsocket.getRemoteAddress() || sender_port != server_info.udpport );
    
    return newp;
}

bool ServerConnection::sendUdp( sf::Packet& p ) 
{ return sf::Socket::Status::Done == udpsocket.send(p, tcpsocket.getRemoteAddress(), server_info.udpport); }

SimulationClient::SimulationClient(WallClock& wc, Cfg& cfg)
    : wc(wc)
    , name(*cfg.getStr("client", "name"))
    , connection(*this, cfg)
{}

SimulationClient::~SimulationClient()
{
    connection.disconnect();
}

ClientApp::ClientApp(WallClock& wc, Cfg& cfg, IPv4Address& addr, Port port)
    : client(wc, cfg)
	, user(cfg, "Dwengine - Client", *cfg.getInt("graphics", "resolutionX"), *cfg.getInt("graphics", "resolutionY"))
{
    if( !client.connection.tryConnect(cfg, addr, port) )
        cout << "Connection failed on " << addr << ':' << port << " !\n";
    else
    {
       cout << "Connection established !\n";
       cout << "Client Name : " << client.name << '\n';
       cout << "Server : " << client.connection.server_info.name << '\n';
    }
}

void ClientApp::run()
{
    while(user.window->isOpen())
    {
        user.draw();
        user.display();

        user.pollEvents();
    }
}