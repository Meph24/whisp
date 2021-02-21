#include "server.hpp"

#include <iostream> //TODO replace with logging #56

#include "protocol.hpp"

using std::cout; //TODO replace with logging #56

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

ConnectionInitialProcessor::ConnectionInitialProcessor(
    SimulationServer& server,
    ConnectionListener& listener,
    WallClock& wc, 
    syncprotocol::ServerInfo& server_info
    )
    : server(server)
    , listener(listener)
    , wc(wc)
    , server_info(server_info)
{
    initial_connection_processing_thread = thread(&ConnectionInitialProcessor::processIncomingConnections, this);
}

ConnectionInitialProcessor::~ConnectionInitialProcessor()
{
    connections.clear();
    stopThread();
}

void ConnectionInitialProcessor::stopThread()
{
    initial_processing_on = false;
    initial_connection_processing_thread.join();
}

syncprotocol::ClientToken ConnectionInitialProcessor::newClientToken() const
{
    syncprotocol::ClientToken token;
    
    if(connections.size() + server.clients.size() >= 255) return token;

    set<uint8_t> existing_ids;
    for(auto& c : connections){ existing_ids.emplace(c->token.client_id); }
    for(auto& c : server.clients.connections){ existing_ids.emplace(c->token.client_id); }
    for (uint16_t i = 1; i < 256; i++)
    {
        if(existing_ids.find(i) != existing_ids.end()) continue;
        else
        {
            token.client_id = i;
            break;
        }
    }
    return token;
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
            cout << " --- local port : " << connection->tcpsocket.getLocalPort() << '\n';

            //receive greeting in 3s
            syncprotocol::ClientIntroduction introduction;
            if( ! syncprotocol::receiveInTime(wc, connection->tcpsocket, (char*) &introduction, sizeof(syncprotocol::ClientIntroduction), 3s) ) continue;

            if( !std::strcmp(introduction.greeting, syncprotocol::client_introduction_greeting) ) continue;
            connection->name = introduction.clientname;
            connection->udpport = introduction.client_udp_port;
            
            //send introduction success
            connection->tcpsocket.setBlocking(true);

            cout    << " --- introduction success !" 
                    << "\n ------ Name: " << connection->name
                    << "\n ------ TCP: " << connection->tcpsocket.getRemoteAddress() << ':' << connection->tcpsocket.getRemotePort()
                    << "\n ------ UDP: " << connection->tcpsocket.getRemoteAddress() << ':' << connection->udpport 
                    << '\n';
            connection->tcpsocket.send( 
                &syncprotocol::introduction_success, 
                sizeof( syncprotocol::introduction_success ) 
            );

            //send back server Information
            cout << " --- sending server info\n";
            connection->tcpsocket.send( &server_info, sizeof(syncprotocol::ServerInfo) );

            //send individual info for this client
            cout << " --- creating client token\n";
            syncprotocol::ClientToken client_token = newClientToken();
            client_token.server_known_fullip = FullIPv4(IPv4Address(connection->tcpsocket.getRemoteAddress().toInteger()), connection->tcpsocket.getRemotePort());

            cout << " --- sending client token\n";
            connection->tcpsocket.send( &client_token, sizeof(syncprotocol::ClientToken) );
            connection->token = client_token;

            std::lock_guard<mutex> lock_guard( connections_lock );
            connections.emplace_back(std::move(connection));
            
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
    unique_ptr<ClientConnection> next_connection = std::move(connections.front()); connections.pop_front();
    return next_connection;
}




UdpServerProcessor::UdpServerProcessor( ClientConnectionListing& clients, Cfg& cfg )
    : clients(clients)
{
    socket.bind( *cfg.getInt("server", "default_udp_port") );
    socket.setBlocking(true);
    send_thread = thread(&UdpServerProcessor::sendProcess, this);
    receive_thread = thread(&UdpServerProcessor::receiveProcess, this);
}

void UdpServerProcessor::stopThreads() 
{ 
    running = false;
    send_thread.join();
    receive_thread.join();
}

void UdpServerProcessor::sendProcess()
{
    while(running)
    {
        for(auto& c : clients.connections)
        {
            ClientConnection& connection = *c;
            if( connection.udp_outbox.empty() ) continue;
            
            if( !connection.udp_outbox_lock.try_lock() )
            {
                ClientConnection* cptr = c.get();
                if(lock_fail_counters.find(cptr)==lock_fail_counters.end()) lock_fail_counters[cptr] = 0;
                lock_fail_counters[cptr]++;
                if(lock_fail_counters[cptr] < lock_fail_tolerance) continue;
                else connection.udp_outbox_lock.lock();
            }

            socket.send( *connection.udp_outbox.front(), connection.tcpsocket.getRemoteAddress(), connection.udpport);
            connection.udp_outbox.pop_front();
            lock_fail_counters[c.get()] = 0;

            connection.udp_outbox_lock.unlock();
        }
    }
}

void UdpServerProcessor::receiveProcess()
{
    unique_ptr<sf::Packet> newpacket;
    sf::IpAddress sender_addr; Port sender_port;

    cout << "Processing incoming udp.\n";
    while(running)
    {
        newpacket.reset(new sf::Packet);
        if( sf::Socket::Status::Done != socket.receive( *newpacket, sender_addr, sender_port ))
            continue;

        if( !clients.containsUdp(sender_addr, sender_port) )
        {
            cout << "Received from unknown sender " << sender_addr << ':' << sender_port << '\n';
            //sender not known!
            //sender not known behavior goes here ... see ticket #55
            continue;
        }
        ClientConnection& connection = *clients.atUdp(sender_addr, sender_port);

        std::lock_guard<std::mutex>(clients.atUdp(sender_addr, sender_port)->udp_inbox_lock);
        connection.udp_inbox.emplace_back(std::move(newpacket));
    }
}


FullIPv4 ClientConnection::remoteUdpFullip() const
{
    return FullIPv4( tcpsocket.getRemoteAddress().toInteger(), udpport );
}



void ClientConnectionListing::addClient( unique_ptr<ClientConnection>&& new_connection )
{
    connections.emplace_back(std::move( new_connection ));
}

void ClientConnectionListing::removeClient(uint8_t id)
{
    auto f = std::find_if( connections.begin(), connections.end(),
                        [&](const unique_ptr<ClientConnection>& p)->bool
                            { return p->token.client_id == id; }
                      );
    if(f == connections.end()) return;

    connections.erase(f);
}

size_t ClientConnectionListing::size() const { return connections.size(); }
ClientConnection* ClientConnectionListing::atTcp(const sf::IpAddress& addr, Port port)
{ 
    auto f = std::find_if(   connections.begin(), connections.end(),
                            [&](const unique_ptr<ClientConnection>& p)->bool
                                { 
                                    return      p->tcpsocket.getRemoteAddress() == addr 
                                            &&  p->tcpsocket.getRemotePort() == port; 
                                }
                         );
    if( f == connections.end() ) return nullptr;
    return f->get();
}
ClientConnection* ClientConnectionListing::atUdp(const sf::IpAddress& addr, Port port)
{
    auto f = std::find_if(   connections.begin(), connections.end(),
                            [&](const unique_ptr<ClientConnection>& p)->bool
                                { 
                                    return      p->tcpsocket.getRemoteAddress() == addr 
                                            &&  p->udpport == port; 
                                }
                         );
    if( f == connections.end() ) return nullptr;
    return f->get();
}

bool ClientConnectionListing::containsTcp(const sf::IpAddress& addr, Port port)
{ return atTcp(addr, port); }
bool ClientConnectionListing::containsUdp(const sf::IpAddress& addr, Port port)
{ return atUdp(addr, port); }




SimulationServer::SimulationServer(WallClock& wc, Cfg& cfg, Port port)
    : cfg(cfg)
    , listener(port)
    , initial_processor(*this, listener, wc, info)
    , udp(clients, cfg)
{
    info.setName(*cfg.getStr("server", "name"));
    info.udpport = udp.socket.getLocalPort();
}

void SimulationServer::processIncomingConnections()
{
    if(! initial_processor.hasProcessedConnections()) return;

    unique_ptr<ClientConnection> newconnection = initial_processor.nextConnection();
    clients.addClient(std::move(newconnection));

}
void SimulationServer::process()
{
    processIncomingConnections();
}

ServerApp::ServerApp(WallClock& wc, Cfg& cfg, Port port)
    : server(wc, cfg, port)
{}

void ServerApp::run()
{
    while(1){}
}
