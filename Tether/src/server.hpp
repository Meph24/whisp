#ifndef SERVER_HPP
#define SERVER_HPP

#include <deque>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "MainApp.h"
#include "protocol.hpp"

using std::deque;
using std::mutex;
using std::queue;
using std::string;
using std::thread;
using std::vector;

struct ClientConnection
{
    string name;
    sf::TcpSocket tcpsocket;
    syncprotocol::ClientToken token;
    Port udpport;

    void sendUdp(unique_ptr<syncprotocol::udp::Packet>);
    unique_ptr<sf::Packet> receiveUdp();

    FullIPv4 remoteUdpFullip() const;

    const WallClock::duration& latency() const;

private:
    WallClock::duration latency_;
    WallClock::time_point last_client_time;

    mutex udp_inbox_lock;
    mutex udp_outbox_lock;
    deque<unique_ptr<sf::Packet>  > udp_inbox; 
    deque<unique_ptr<syncprotocol::udp::Packet> > udp_outbox;

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

    syncprotocol::ServerInfo& server_info;


    bool initial_processing_on = true;
    thread initial_connection_processing_thread;

    void stopThread();

    ConnectionInitialProcessor(
        SimulationServer& server,
        ConnectionListener& listener, 
        WallClock& wc, 
        syncprotocol::ServerInfo& server_info
        );
    ~ConnectionInitialProcessor();

    mutex connections_lock;
    bool hasProcessedConnections();
    unique_ptr<ClientConnection> nextConnection();

    syncprotocol::ClientToken newClientToken() const;    

private:
    deque<unique_ptr<ClientConnection> > connections;
    void processIncomingConnections();
};

struct ClientConnectionListing
{
    vector<unique_ptr<ClientConnection> > connections;

    void addClient(unique_ptr<ClientConnection>&& new_connection);
    void removeClient(uint8_t);

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

    void processIncomingConnections();
    void process();

    SimulationServer(WallClock& wc, Cfg& cfg, Port port);
    ~SimulationServer() = default;
};

struct ServerApp : public App
{
    SimulationServer server;
    ServerApp(WallClock& wc, Cfg& cfg, Port port);
    void run();
};

#endif // SERVER_HPP
