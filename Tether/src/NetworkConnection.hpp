#ifndef NETWORKCONNECTION_HPP
#define NETWORKCONNECTION_HPP

#include <memory>

using std::unique_ptr;

#include <SFML/Network.hpp>

#include "IPv4Address.hpp"

#include <list>
using std::list;

namespace network
{

struct TcpSocket : public ::sf::TcpSocket {};
struct UdpSocket : public ::sf::UdpSocket {};

struct ConnectionHead
{
    unique_ptr<TcpSocket> tcpsocket;
    ConnectionHead( unique_ptr<TcpSocket>&& );
};


struct TcpListener
{
    ::sf::TcpListener listener;
    void listen(Port tcp_port);
    unique_ptr<TcpSocket> nonBlockingNext();
};


class ConnectionReception
{
    void processNewTcpConnection( unique_ptr<TcpSocket>&& );
    
    TcpListener listener;
public:
    void listen( Port tcp_port );
    bool isListening() const;

    list <ConnectionHead> heads;

    bool processNextIncoming();
};



} /* namespace network */
#endif // NETWORKCONNECTION_HPP
