#ifndef SERVER_HPP
#define SERVER_HPP

#include <deque>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "MainApp.hpp"
#include "protocol.hpp"
#include "SyncableManager.h"

using std::deque;
using std::list;
using std::mutex;
using std::queue;
using std::shared_ptr;
using std::string;
using std::thread;
using std::vector;

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

    void sendUdp(shared_ptr<syncprotocol::udp::Packet>&);
    unique_ptr<sf::Packet> receiveUdp();

    FullIPv4 remoteUdpFullip() const;

    const WallClock::duration& latency() const;

private:
    WallClock::duration latency_;
    WallClock::time_point last_client_time;

    mutex udp_inbox_lock;
    mutex udp_outbox_lock;
    deque<unique_ptr<sf::Packet>  > udp_inbox; 
    deque<shared_ptr<syncprotocol::udp::Packet> > udp_outbox;

    friend struct UdpServerProcessor;
};

struct ConnectionListener
{
    Port listenport = 0;
    sf::TcpListener tcplistener;

    thread t;
    mutex connections_lock;
    queue<unique_ptr<ClientConnection> > connections;

    bool hasNewConnections() const;
    unique_ptr<ClientConnection> nextConnection();

    void startListening( Port port );
    void stopListening();
    bool isListening() const;

    ConnectionListener() = default;
    ConnectionListener(Port port);

private:
    bool is_listening = false;
    void listenProcess();
};

struct SimulationServer;

struct ConnectionInitialProcessor
{
    SimulationServer& server;

    ConnectionListener& listener;
    WallClock& wc;

    ConnectionInitialProcessor(
        SimulationServer& server,
        ConnectionListener& listener, 
        WallClock& wc
        );
    ~ConnectionInitialProcessor();

    bool running = true;
    thread main_process;
    void mainProcess();
    void stopMainProcess();

    struct SingleConnectionProcessor
    {
        unique_ptr<ClientConnection> connection;
        ConnectionInitialProcessor* cip;

        thread t;

        SingleConnectionProcessor(unique_ptr<ClientConnection>&&,
            ConnectionInitialProcessor& cip
            );

        SingleConnectionProcessor(SingleConnectionProcessor&&);
        SingleConnectionProcessor& operator=(SingleConnectionProcessor&&);

        SingleConnectionProcessor(const SingleConnectionProcessor&) = delete;
        SingleConnectionProcessor& operator=(const SingleConnectionProcessor&) = delete;


        void process();
        bool finished() const;
    };

    mutex process_lock;
    list<SingleConnectionProcessor> processes;
    void asyncProcessNewConnection();

    list<unique_ptr<ClientConnection> > processed_connections;
    bool hasProcessedConnections();
    unique_ptr<ClientConnection> nextConnection();

private:
    deque<unique_ptr<ClientConnection> > connections;
    void processIncomingConnections();
};

struct ClientConnectionListing
{
    vector<unique_ptr<ClientConnection> > connections;

    void addClient(unique_ptr<ClientConnection>&& new_connection);

    size_t size() const;

    bool containsTcp(const sf::IpAddress& addr, Port port);
    bool containsUdp(const sf::IpAddress& addr, Port port);
    ClientConnection* atTcp(const sf::IpAddress& addr, Port port);
    ClientConnection* atUdp(const sf::IpAddress& addr, Port port);
};

struct UdpServerProcessor
{
    ClientConnectionListing& clients;
    sf::UdpSocket socket;

    sf::UdpSocket udp_tester_socket;

    map<ClientConnection*, uint8_t> lock_fail_counters;
    uint8_t lock_fail_tolerance = 3;

    const WallClock& wc;

    UdpServerProcessor( ClientConnectionListing& clients, Cfg& cfg, const WallClock& wc);

    bool running = true;
    void stopThreads();

    thread send_thread;
    thread receive_thread;
    void sendProcess();
    void receiveProcess();
};

struct SimulationServer
{
    Cfg& cfg;
    ConnectionListener listener;
    ConnectionInitialProcessor initial_processor;
    UdpServerProcessor udp;

    syncprotocol::ServerInfo info;

    ClientConnectionListing clients;

    SyncableManager* syncman;

    void processIncomingConnections();
    void process();

    void setup(SyncableManager& syncable_manager);

    SimulationServer(WallClock& wc, Cfg& cfg, Port port);
    ~SimulationServer() = default;

private:
    void broadcastTcp(sf::Packet&);
    void broadcastUdp(unique_ptr<syncprotocol::udp::Packet>&&);
};

struct HostApp : public App
{
    unique_ptr<Simulation> simulation;

    SimulationServer server;
    unique_ptr<SyncableManager> syncman;

    LocalUser local_user;

    HostApp(WallClock& wc, Cfg& cfg, Port port);
    void run();
};

#endif // SERVER_HPP
