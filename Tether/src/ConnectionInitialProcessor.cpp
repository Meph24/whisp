#include "ConnectionInitialProcessor.hpp"

#include "ConnectionListener.hpp"
#include "protocol.hpp"
#include "SimulationServer.hpp"
#include "SyncableManager.h"

#include <iostream>
using std::cout; //TODO replace with logging #56

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

    SyncableManager* syncman = cip->server.syncman;
    if(!syncman){ cout << "ConnectionInitialProcessor: Server has no initialized syncable manager!\n"; return; }
    Simulation* sim = syncman->getSim();
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
            connection->user.disconnectSimulation();
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