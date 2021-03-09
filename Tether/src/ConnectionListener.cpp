#include "ConnectionListener.hpp"

#include <iostream>
using std::cout; //TODO replace with logging #56

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
            std::this_thread::sleep_for(200ms);
        }
    }
}