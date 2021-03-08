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

    if(is_connected) return false;
    cout << "Connecting to " << addr << ':' << port << '\n';

    tcpsocket.setBlocking(true);
    is_connected = sf::Socket::Status::Done == tcpsocket.connect(
            sf::IpAddress( (uint32_t)addr ), port, sf::seconds(5) );
    if(!is_connected)
    {
        cout << "tryConnect: connection attempt failed!\n";
        return false;
    }

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

    udpsocket.bind( server_info.udpport );

    return is_connected;
}

void ServerConnection::disconnect()
{
    tcpsocket.disconnect();
    is_connected = false;
}

bool ServerConnection::connected() const { return tcpsocket.getRemoteAddress() != sf::IpAddress::None; }

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
            newp.reset();
            break;
        }
    } while (sender_addr != tcpsocket.getRemoteAddress() || sender_port != server_info.udpport );

    syncprotocol::udp::Header header; *newp >> header;
    latency_ = (header.client_time - client.wc.now()) / 2;
    if(latest_server_time < header.server_time) latest_server_time = header.server_time;

    return newp;
}

bool ServerConnection::sendUdp( syncprotocol::udp::Packet& p ) 
{ 
    syncprotocol::udp::Header header {latest_server_time, client.wc.now()};
    p.setHeader(header);
    return sf::Socket::Status::Done == udpsocket.send(p, tcpsocket.getRemoteAddress(), server_info.udpport); 
}

SimulationClient::SimulationClient(WallClock& wc, Cfg& cfg)
    : wc(wc)
    , name(*cfg.getStr("client", "name"))
    , connection(*this, cfg)
    , syncman(wc, cfg)
{
    connection.tcpsocket.setBlocking(false);
}

SimulationClient::~SimulationClient()
{
    connection.disconnect();
}

bool SimulationClient::connected() const { return connection.connected(); }
bool SimulationClient::initialized() const { return syncman.getSim(); }

bool SimulationClient::processInitialSync()
{
    if(initialized()) return true;
    if(!connected()) return false;

    sf::Packet incoming_tcp;

    if(connection.tcpsocket.receive(incoming_tcp) != sf::Socket::Done)
        return false;
    syncman.applyEventPacket(incoming_tcp);
    return initialized();
}

void SimulationClient::processCyclicSync()
{
    if(!connected() || !initialized()) return;

    //tcp ; game events
    unique_ptr<sf::Packet> incoming;
    if(connection.tcpsocket.receive(*incoming) == sf::Socket::Done && incoming->getDataSize())
    {
        syncman.applyEventPacket(*incoming);
    }

    //udp ; update packets
    incoming->clear();
    incoming = connection.receiveUdp();
    if(incoming)
        syncman.applyUpdatePacket(*incoming);
}

EntityPlayer* SimulationClient::avatar() const
{
    //TODO implement
    //the avatar must be sampled from the syncable manager
    //the syncable manager must however be updated, so that
    // the entity player can be probed
    return nullptr;
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
    cout << "Waiting on Simulation to be transmitted from server ...\n";
    {
        //for visual feedback of processing on terminal
        char wheel[4] = {'-', '\\', '|', '/' };
        int wheelindex = 0;
        
        glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
        while(user.window.isOpen() && !client.processInitialSync())
        {
            wheelindex = (wheelindex + 1) % sizeof(wheel);
            cout << '\r' << wheel[wheelindex];
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            user.window.display();
            InputEvent e; while(user.window.pollEvent(e)); //keep window open
        }
        cout << "\rSimulation received!\n";
    }

    while(user.window.isOpen())
    {
        if(!client.avatar())
        {
            //Switch the color to give visual feedback of no avatar
            glClearColor(0.5f, 0.0f, 0.0f, 0.0f);
            continue;
        }
        else
        {
            glClearColor(0.0f, 0.0f, 0.25f, 0.0f);
        }
        client.processCyclicSync();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        user.draw();
        user.window.display();

        user.pollEvents();
    }
}