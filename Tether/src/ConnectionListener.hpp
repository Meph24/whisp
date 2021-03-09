#ifndef CONNECTIONLISTENER_HPP
#define CONNECTIONLISTENER_HPP

#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include <SFML/Network/TcpListener.hpp>

#include "ClientConnection.hpp"
#include "IPv4Address.hpp"

using std::mutex;
using std::queue;
using std::thread;
using std::unique_ptr;

struct ConnectionListener
{
    sf::TcpListener tcplistener;

    thread t;
    mutex connections_lock;
    queue<unique_ptr<ClientConnection> > connections;

    bool hasNewConnections() const;
    unique_ptr<ClientConnection> nextConnection();

    void start( Port port );
    void stop();
    bool isListening() const;

    ConnectionListener() = default;
    ConnectionListener(Port port);

private:
    bool is_listening = false;
    void listenProcess();
};

#endif // CONNECTIONLISTENER_HPP
