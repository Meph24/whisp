#ifndef SYNCHRONIZED_HPP
#define SYNCHRONIZED_HPP

#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include <SFML/Network.hpp>

#include "MainApp.h"
#include "IPv4Address.hpp"

using std::mutex;
using std::queue;
using std::string;
using std::thread;
using std::unique_ptr;
using std::vector;

using network::IPv4Address;
using network::Port;

struct ClientConnection
{
    string name;
    sf::TcpSocket tcpsocket;
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

namespace syncmessages
{
    bool receiveInTime( WallClock& wc,
                        sf::TcpSocket& socket, 
                        char* buffer, size_t size, 
                        WallClock::duration timeout_duration);
                        
    const char introduction_success[9] = "SUCCESS";
    const char introduction_fail[9] = "FAILURE";
    const size_t introduction_response_size = 9;
    const char client_introduction_greeting[41] = "Hi, I want to join this Dwengine Server!";

    struct ClientIntroduction
    {
        char greeting[41] = {0};
        char clientname [64] = {0};

        ClientIntroduction(string name = "");
    };
    struct ServerInfo
    {
        char heading[29] = "Dwengine Server Information:";
        char name[64] = {0};

        ServerInfo(string name = "");
        friend ostream& operator<< (ostream&, const ServerInfo&);
    };
    ostream& operator<< (ostream&, const ServerInfo&);
    
}/* namespace syncmessages */

struct ConnectionInitialProcessor
{
    ConnectionListener& listener;
    WallClock& wc;

    syncmessages::ServerInfo& server_info;

    bool initial_processing_on = true;
    thread intital_connection_processing_thread;

    ConnectionInitialProcessor(
        ConnectionListener& listener, 
        WallClock& wc, 
        syncmessages::ServerInfo& server_info
        );
    ~ConnectionInitialProcessor();

    mutex connections_lock;
    bool hasProcessedConnections();
    unique_ptr<ClientConnection> nextConnection();

private:
    queue<unique_ptr<ClientConnection> > connections;
    void processIncomingConnections();
};

struct SimulationServer
{
    syncmessages::ServerInfo info;
    Cfg& cfg;
    ConnectionListener listener;
    ConnectionInitialProcessor intital_processor;

    SimulationServer(WallClock& wc, Cfg& cfg, Port port);
    ~SimulationServer() = default;
};

struct SimulationClient;

struct ServerConnection
{
    sf::TcpSocket tcpsocket;
    SimulationClient& client;

    syncmessages::ServerInfo server_info;

    bool tryConnect(const IPv4Address& addr, Port port);
    void disconnect();

    ServerConnection(SimulationClient& client);
private:
    bool is_connected = false;
};

struct SimulationClient
{
    WallClock& wc;
    string name;
    ServerConnection connection;

    SimulationClient(WallClock& wc, Cfg& cfg);
    ~SimulationClient();
};

struct ServerApp : public App
{
    SimulationServer server;
    ServerApp(WallClock& wc, Cfg& cfg, Port port);
    void run();
};

struct ClientApp : public App
{
    SimulationClient client;
    ClientApp(WallClock& wc, Cfg& cfg, IPv4Address& addr, Port port);
    void run();
};

#endif // SYNCHRONIZED_HPP
