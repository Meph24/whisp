#include "synchronized.hpp"

#include <iostream>

using std::cout;

bool ConnectionListener::hasNewConnections() const { return !connections.empty(); }
unique_ptr<ClientConnection> ConnectionListener::nextConnection()
{
    if(!hasNewConnections()) return nullptr;

    std::lock_guard<mutex> lock_guard( connections_lock );
    unique_ptr<ClientConnection> next_connection = std::move(connections.front()); connections.pop();
    return next_connection;
}

void ConnectionListener::startListening( Port port )
{ 
    tcplistener.listen(port);
    t = thread(&ConnectionListener::listenProcess, this); 
    is_listening = true;
}
void ConnectionListener::stopListening()
{ 
    is_listening = false;
    t.join();
}
bool ConnectionListener::isListening() const { return is_listening; }

ConnectionListener::ConnectionListener(Port port) 
{
    startListening(port);
}

void ConnectionListener::listenProcess()
{
    cout << "Start listening for tcp connections on port " << tcplistener.getLocalPort() << ".\n";
    unique_ptr<ClientConnection> newconnection = std::make_unique<ClientConnection>();
    while(is_listening)
    {
        //Segfault here, because sf::Socket::close() is called, most likely on empty socket
        //I recon this has to do with the socket not being declared in same thread
        // need to investigate further
        if (tcplistener.accept(newconnection->tcpsocket) == sf::Socket::Done)
        {
            std::lock_guard<mutex> lock_guard( connections_lock );
            connections.emplace( std::move(newconnection) );
            newconnection = std::make_unique<ClientConnection>();
        }
        else
        {
            //TODO investigate blocking behavior of socket
            std::this_thread::sleep_for(200ms);
        }
    }
}

namespace syncmessages
{

    bool receiveInTime(WallClock& wc, sf::TcpSocket& socket, char* buffer, size_t size, WallClock::duration timeout_duration)
    {
        WallClock::time_point call_time = wc.now();
        size_t received = 0;
        socket.setBlocking(false);
        while( received < size &&  wc.now() - call_time < timeout_duration )
        {
            socket.receive(buffer + received, size - received, received);
        }
        return received == size;
    }

    ClientIntroduction::ClientIntroduction(string input_client_name)
    {
        std::fill(clientname, clientname+sizeof(clientname), '\0');
        if(input_client_name.size() < 0 || input_client_name.size() > sizeof(clientname) - 1 )
            throw std::runtime_error("Input client name is wrongly sized [max " + std::to_string(sizeof(clientname)-1) + "bit]");
        input_client_name.copy(clientname, input_client_name.size());
    }

    ServerInfo::ServerInfo(string server_name)
    {
        std::fill(name, name+sizeof(name), '\0');
        if(server_name.size() < 0 || server_name.size() > sizeof(name)-1)
            throw std::runtime_error("Input server name is wrongly sized [max " + std::to_string(sizeof(name)-1) + " bit]");
        server_name.copy(name, server_name.size());
    }

    ostream& operator<< (ostream& os, const syncmessages::ServerInfo& server_info)
    {
        os << "Dwengine Server Information:\n";
        os << "Name: " << server_info.name << '\n';
        return os;
    }

} /* namespace syncmessages */

ConnectionInitialProcessor::ConnectionInitialProcessor(ConnectionListener& listener, WallClock& wc, syncmessages::ServerInfo& server_info)
    : listener(listener)
    , wc(wc)
    , server_info(server_info)
{
    intital_connection_processing_thread = thread(&ConnectionInitialProcessor::processIncomingConnections, this);
}

ConnectionInitialProcessor::~ConnectionInitialProcessor()
{
    queue<unique_ptr<ClientConnection>> empty_queue; empty_queue.swap(connections);
}

void ConnectionInitialProcessor::processIncomingConnections()
{
    cout << "Start Processing incoming connections.\n";
    while( initial_processing_on )
    {
        if(listener.hasNewConnections())
        {
            cout << " --- new incoming connection\n";
            unique_ptr<ClientConnection> connection = listener.nextConnection();

            //receive greeting in 3s
            syncmessages::ClientIntroduction introduction;
            if( ! syncmessages::receiveInTime(wc, connection->tcpsocket, (char*) &introduction, sizeof(syncmessages::ClientIntroduction), 3s) ) continue;

            if( !std::strcmp(introduction.greeting, syncmessages::client_introduction_greeting) ) continue;
            connection->name = introduction.clientname;
            
            //send introduction success
            connection->tcpsocket.setBlocking(true);

            cout << " --- introduction success ! Welcome " << connection->name << " !\n";
            connection->tcpsocket.send( 
                &syncmessages::introduction_success, 
                sizeof( syncmessages::introduction_success ) 
            );

            //send back server Information
            cout << " --- sending server info\n";
            connection->tcpsocket.send( &server_info, sizeof(syncmessages::ServerInfo) );

            std::lock_guard<mutex> lock_guard( connections_lock );
            connections.emplace(std::move(connection));
            
            cout 
                << connections.back()->name << " connected with " 
                << connections.back()->tcpsocket.getRemoteAddress().toString() 
                << ':' << connections.back()->tcpsocket.getRemotePort() << '\n';
        }
        else
            std::this_thread::sleep_for(200ms);
    }
}

bool ConnectionInitialProcessor::hasProcessedConnections(){ return !connections.empty(); }
unique_ptr<ClientConnection> ConnectionInitialProcessor::nextConnection()
{ 
    if(!hasProcessedConnections()) return nullptr;

    std::lock_guard<mutex> lock_guard( connections_lock );
    unique_ptr<ClientConnection> next_connection = std::move(connections.front()); connections.pop();
    return next_connection;
}


SimulationServer::SimulationServer(WallClock& wc, Cfg& cfg, Port port)
    : info( *cfg.getStr("server", "name") )
    , cfg(cfg)
    , listener(port)
    , intital_processor(listener, wc, info)
{}

ServerConnection::ServerConnection(SimulationClient& client) : client(client) {}

bool ServerConnection::tryConnect( const IPv4Address& addr, Port port )
{
    cout << "Connecting to " << addr << ':' << port << '\n';
    if(is_connected) return false;

    is_connected = tcpsocket.connect(sf::IpAddress( (uint32_t)addr ), port) == sf::Socket::Done;
    if(!is_connected) return false;

    cout << "Socket connection established\nIntroduction...";

    syncmessages::ClientIntroduction introduction(client.name);
    tcpsocket.setBlocking(true);
    if( tcpsocket.send(&introduction, sizeof(introduction)) != sf::Socket::Done ) return false;

    char introduction_response[syncmessages::introduction_response_size];
    if( !syncmessages::receiveInTime(   client.wc,
                                        tcpsocket, 
                                        introduction_response, 
                                        sizeof(introduction_response),
                                        10s) ) return false;
    if( std::strcmp(introduction_response, syncmessages::introduction_success) ) 
    {
        cout << "FAILURE\n";
        return false;
    }
    cout << "SUCCESS\n";

    if( !syncmessages::receiveInTime( client.wc, tcpsocket, (char*) &server_info, sizeof(server_info), 10s ) ) return false;

    cout << "Server Info:" << server_info << '\n';

    return is_connected;
}

void ServerConnection::disconnect()
{
    tcpsocket.disconnect();
    is_connected = false;
}

SimulationClient::SimulationClient(WallClock& wc, Cfg& cfg)
    : wc(wc)
    , name(*cfg.getStr("client", "name"))
    , connection(*this)
{}

SimulationClient::~SimulationClient()
{
    connection.disconnect();
}

ServerApp::ServerApp(WallClock& wc, Cfg& cfg, Port port)
    : server(wc, cfg, port)
{}

void ServerApp::run()
{
    while(1){}
}

ClientApp::ClientApp(WallClock& wc, Cfg& cfg, IPv4Address& addr, Port port)
    : client(wc, cfg)
{
    if( !client.connection.tryConnect(addr, port) )
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
    while(1){}
}