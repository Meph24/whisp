#include "server.hpp"

#include <iostream> //TODO replace with logging #56

#include "protocol.hpp"
#include "Simulation.hpp"

using std::cout; //TODO replace with logging #56

void SyncUser::operateSimulation(Simulation& simulation)
{
    if(this->simulation) return;

    this->simulation = &simulation;
    simulation.registerUser(this);
    if(!avatar()) { std::cerr << "User could not be registered in Simulation!" << std::endl; }
}
void SyncUser::disconnectSimulation()
{
    simulation->kickUser(this);
}

void ClientConnection::sendUdp(shared_ptr<syncprotocol::udp::Packet>& p)
{
    std::lock_guard<std::mutex> lockguard (udp_outbox_lock);
    udp_outbox.emplace_back(p);
}

unique_ptr<sf::Packet> ClientConnection::receiveUdp()
{
    if(udp_inbox.empty()) return nullptr;
    
    std::lock_guard<std::mutex> lockguard (udp_inbox_lock);
    unique_ptr<sf::Packet> newp = std::move(udp_inbox.front());
    udp_inbox.pop_front();
    return newp;
}

FullIPv4 ClientConnection::remoteUdpFullip() const
{
    return FullIPv4( tcpsocket.getRemoteAddress().toInteger(), udpport );
}

const WallClock::duration& ClientConnection::latency() const { return latency_; }

bool ConnectionListener::hasNewConnections() const { return !connections.empty(); }
unique_ptr<ClientConnection> ConnectionListener::nextConnection()
{
    if(!hasNewConnections()) return nullptr;

    std::lock_guard<mutex> lock_guard( connections_lock );
    unique_ptr<ClientConnection> next_connection = std::move(connections.front()); connections.pop();
    return next_connection;
}

void ConnectionListener::start( Port port )
{ 
    tcplistener.listen(port);
    is_listening = true;
    t = thread(&ConnectionListener::listenProcess, this); 
}
void ConnectionListener::stop()
{ 
    is_listening = false;
    t.join();
}
bool ConnectionListener::isListening() const { return is_listening; }

ConnectionListener::ConnectionListener(Port port) 
{
    start(port);
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
    WallClock& wc
    )
    : server(server)
    , listener(listener)
    , wc(wc)
{}

void ConnectionInitialProcessor::start()
{
    running = true;
    main_process = thread(&ConnectionInitialProcessor::mainProcess, this);
}

void ConnectionInitialProcessor::stop()
{
    running = false;
    main_process.detach();
}

void ConnectionInitialProcessor::mainProcess()
{
    cout << "Initial Processing is up!\n";
    while(running)
    {
        while(listener.hasNewConnections())
        {
            asyncProcessNewConnection();
        }
        std::this_thread::sleep_for(200ms);
    }
}

ConnectionInitialProcessor::~ConnectionInitialProcessor()
{
    stop();
}

ConnectionInitialProcessor::SingleConnectionProcessor::SingleConnectionProcessor(
    unique_ptr<ClientConnection>&& connection,
    ConnectionInitialProcessor& cip
    )
    : connection(std::move(connection))
    , cip(&cip)
    , t(&SingleConnectionProcessor::process, this)
{
    t.detach();
}

void ConnectionInitialProcessor::asyncProcessNewConnection()
{
    if(!listener.hasNewConnections()) return;

    std::lock_guard<std::mutex> lg_ (process_lock);

    cout << " --- processing new incoming connection\n";
    processes.emplace_back( listener.nextConnection(), *this );
}

void ConnectionInitialProcessor::SingleConnectionProcessor::process()
{
    cout << "Asynchronous thread started!\n";

    //receive greeting in 3s
    syncprotocol::ClientIntroduction introduction;
    if( ! syncprotocol::receiveInTime(cip->wc, connection->tcpsocket, (char*) &introduction, sizeof(syncprotocol::ClientIntroduction), 3s) )
    {
        cout << "timeout\n";
        return;
    }

    if( !std::strcmp(introduction.greeting, syncprotocol::client_introduction_greeting) ) 
    {
        cout << "wrong greeting\n";
        return;
    }
    connection->name = introduction.clientname;
    connection->udpport = introduction.client_udp_port;
    
    //send introduction success
    connection->tcpsocket.setBlocking(true);

    connection->tcpsocket.send( 
        &syncprotocol::introduction_success, 
        sizeof( syncprotocol::introduction_success ) 
    );

    //send back server Information
    connection->tcpsocket.send( &cip->server.info, sizeof(syncprotocol::ServerInfo) );

    //send individual info for this client
    syncprotocol::ClientToken client_token;
    client_token.server_known_fullip = FullIPv4(IPv4Address(connection->tcpsocket.getRemoteAddress().toInteger()), connection->tcpsocket.getRemotePort());

    //TODO
    Simulation* sim = cip->server.syncman->getSim();
    if(!sim) { cout << "No Simulation found on Server.\n"; return; }

    connection->user.operateSimulation(*sim);
    //WAIT FOR ENTITYPLAYER to be spawned
    //wait for syncablemanager to have an id
    WallClock::time_point wait_begin = cip->wc.now();
    WallClock::duration timeout (10s);
    while( !connection->user.avatar() || !connection->user.avatar()->sID)
    {
        if( cip->wc.now() - wait_begin > timeout )
        {
            cout << "Timeout in Avatar spawning process!\n";
            return;
        }
        std::this_thread::sleep_for(50ms);
    }
    //add id to clienttoken
    client_token.avatar_syncid = connection->user.avatar()->sID;
    
    connection->tcpsocket.send( &client_token, sizeof(syncprotocol::ClientToken) );
    connection->token = client_token;

    cout << " --- new connection accepted\n";
    std::lock_guard<mutex> lg_ ( cip->process_lock );
    cip->connections.emplace_back(std::move(connection));
}


ConnectionInitialProcessor::SingleConnectionProcessor::SingleConnectionProcessor(
    SingleConnectionProcessor&& other
    )
    : connection(std::move(other.connection))
    , cip(other.cip)
    , t(std::move(other.t))
{}

ConnectionInitialProcessor::SingleConnectionProcessor::~SingleConnectionProcessor()
{
    t.detach();
}

bool ConnectionInitialProcessor::SingleConnectionProcessor::finished() const
{ return (bool)connection; }

ConnectionInitialProcessor::SingleConnectionProcessor& 
ConnectionInitialProcessor::SingleConnectionProcessor::operator=(
    SingleConnectionProcessor&& other
    )
{
    connection = std::move(other.connection);
    cip = other.cip;
    t = std::move(other.t);
    return *this;
}

bool ConnectionInitialProcessor::hasProcessedConnections(){ return !connections.empty(); }
unique_ptr<ClientConnection> ConnectionInitialProcessor::nextConnection()
{ 
    if(!hasProcessedConnections()) return nullptr;
    std::lock_guard<mutex> lock_guard( process_lock );

    //cleanup finished threads
    auto prev_size = processes.size();
    if(prev_size)
    {
        processes.remove_if([&](const SingleConnectionProcessor& p)->bool{return p.finished();});
        cout << "Cleaning up " << prev_size - processes.size() << " threads!\n";
    }
    unique_ptr<ClientConnection> next_connection = std::move(connections.front()); connections.pop_front();
    return next_connection;
}

UdpServerProcessor::UdpServerProcessor( ClientConnectionListing& clients, Cfg& cfg, const WallClock& wc )
    : clients(clients)
    , wc(wc)
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

            syncprotocol::udp::Header header { wc.now(), c->last_client_time };
            connection.udp_outbox.front()->setHeader(header);

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
        syncprotocol::udp::Header header; *newpacket >> header;
        connection.latency_ = (header.server_time - wc.now()) / 2;
        if(connection.last_client_time < header.client_time) 
            connection.last_client_time = header.client_time;

        std::lock_guard<std::mutex>(clients.atUdp(sender_addr, sender_port)->udp_inbox_lock);
        connection.udp_inbox.emplace_back(std::move(newpacket));
    }
}

void ClientConnectionListing::addClient( unique_ptr<ClientConnection>&& new_connection )
{
    connections.emplace_back(std::move( new_connection ));
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
    , initial_processor(*this, listener, wc)
    , udp(clients, cfg, wc)
{
    info.setName(*cfg.getStr("server", "name"));
    info.udpport = udp.socket.getLocalPort();
}

void SimulationServer::processIncomingConnections()
{
    if(! initial_processor.hasProcessedConnections()) return;

    vector<unique_ptr<ClientConnection> >new_connections;
    while(initial_processor.hasProcessedConnections())
    {
        new_connections.emplace_back(initial_processor.nextConnection());
    }

    sf::Packet packet;
    syncman->fillCompletePacket(packet);
    for(auto& c : new_connections)
    {
        c->tcpsocket.send(packet);
        clients.addClient(std::move(c));
    }
}

void SimulationServer::process()
{
    if(!syncman) return;
    sf::Packet packet;

    while(syncman->fetchEventPackets(packet))
        broadcastTcp(packet);

    processIncomingConnections();

    if(!clients.connections.empty())
    {
        unique_ptr<syncprotocol::udp::Packet> udp_packet = std::make_unique<syncprotocol::udp::Packet>();
        if(syncman->fillUpdatePacket(*udp_packet, sf::UdpSocket::MaxDatagramSize - udp_packet->getDataSize() ))
            broadcastUdp(std::move(udp_packet));
    }
}

void SimulationServer::setup(
    SyncableManager& syncable_manager
    )
{ 
    syncman = &syncable_manager; 
    initial_processor.start();
}

#include "Simulation_World.h"
#include "Zombie_World.h"

HostApp::HostApp(WallClock& wc, Cfg& cfg, Port port)
    : server(wc, cfg, port)
	, local_user(cfg, *cfg.getStr("general", "application_name") + string(" - Host"),
        *cfg.getInt("graphics", "resolutionX"), *cfg.getInt("graphics", "resolutionY"))
{
    cout << "Initializing Host Application !\n";

	int zombie_mode = *cfg.getInt("test", "zombie_mode");
	if(zombie_mode) 
	{
		simulation = std::make_unique<Zombie_World>(wc, cfg);
	}
	else 
	{
		simulation = std::make_unique<Simulation_World>(wc, cfg);
	}
    if(!simulation) throw std::runtime_error("Simulation could not be initialized!");

    local_user.operateSimulation(*simulation);
    syncman = std::make_unique<SyncableManager>(*simulation);
    server.setup(*syncman);
}

void HostApp::run()
{
    if(!simulation || !syncman) 
		throw std::runtime_error( "Application cannot be run! Initialization Failure!" );

    simulation->init();

    while(local_user.window.isOpen())
    {
        simulation->step();

        server.process(); 

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        local_user.draw();
        local_user.window.display();

        local_user.pollEvents();
    }
}

void SimulationServer::broadcastTcp(sf::Packet& p)
{
    for(auto& c : clients.connections)
    {
        c->tcpsocket.send(p);
    }
}

void SimulationServer::broadcastUdp(unique_ptr<syncprotocol::udp::Packet>&& packet)
{
    syncprotocol::udp::Packet* packet_ptr = packet.get();
    packet.release();
    shared_ptr<syncprotocol::udp::Packet> shared_packet (packet_ptr);
    for(auto& c : clients.connections)
    {
        c->sendUdp(shared_packet);
    }
}
