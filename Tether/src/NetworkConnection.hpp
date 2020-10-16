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

struct ConnectionHead
{
    unique_ptr<TcpSocket> tcpsocket;

    ConnectionHead( unique_ptr<TcpSocket>&& );
};

ConnectionHead::ConnectionHead( unique_ptr<TcpSocket>&& socket ) : tcpsocket( ::std::move(socket) ) {}


struct TcpListener
{
    ::sf::TcpListener listener;
    void listen(Port tcp_port);
    unique_ptr<TcpSocket> nonBlockingNext();
};

void TcpListener::listen(Port tcp_port) { listener.listen(tcp_port); }
unique_ptr<TcpSocket> TcpListener::nonBlockingNext() 
{
    unique_ptr<TcpSocket> new_socket (new TcpSocket) ;
    listener.setBlocking(false);
    if( listener.accept(*new_socket) == ::sf::Socket::Status::Done) 
    {
        new_socket->setBlocking(false);
        std::cout << "Connection established! " << new_socket.get() << "\n";
        return new_socket;
    }
    return nullptr;
}

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

void ConnectionReception::listen ( Port tcp_port )
{
    listener.listen( tcp_port );
}

void ConnectionReception::processNewTcpConnection( unique_ptr<TcpSocket>&& socket )
{
    //TODO default allow is bad
    heads.emplace_back( ::std::move(socket) );
}

bool ConnectionReception::processNextIncoming()
{
    unique_ptr<TcpSocket> new_socket (listener.nonBlockingNext());
    if( !new_socket ) return false;

    processNewTcpConnection( ::std::move(new_socket) );
    return true;
}

/*


struct TcpListener : public Socket
{
    enum Status{ Fine, Error, NUM_STATI };
    virtual Status status() const;

    virtual void listen(unsigned short port, const IPv4Address& address = IPv4Address::any) = 0;
    virtual unique_ptr<TcpSocket> incoming() = 0;
    virtual void close() = 0;
};

#include "SFML/Network.hpp"
#include "SFML/System/Time.hpp"

class SFMLTcpSocket : public TcpSocket
{
    sf::TcpSocket socket;
    TcpSocket::Status current_status;
public:
    SFMLTcpSocket();

    TcpSocket::Status status() const;

    void setBlocking(bool blocking = true);
    bool isBlocking() const;
    Port localPort() const;
    IPv4Address remoteAddress() const;
    Port remotePort() const;

    Status connect(const IPv4Address&, const Port&, const milliseconds&);
    void disconnect();

    void send(const void* data, size_t size);
    void send(const void* data, size_t size, size_t& sent);
    void receive(void* data, size_t size, size_t& received);
};

SFMLTcpSocket::SFMLTcpSocket() : current_status( TcpSocket::Status::Disconnected ) {}
virtual Status SFMLTcpSocket::status() const { return current_status; }
void SFMLTcpSocket::setBlocking(bool blocking = true) { socket.setBlocking(blocking); }
bool SFMLTcpSocket::isBlocking() const { return socket.isBlocking(); }
Port SFMLTcpSocket::SFMLTcpSocket::localPort() const { return socket.getLocalPort(); }
IPv4Address SFMLTcpSocket::remoteAddress() const { return IPv4Address( socket.getRemoteAddress().toInteger() ); }
Port SFMLTcpSocket::remotePort() const { return socket.getRemotePort(); }

TcpSocket::Status SFMLTcpSocket::connect(const IPv4Address& addr, const Port& port, const milliseconds& timeout = milliseconds(0) )
{ 
    if(timeout == milliseconds(0)) socket.connect(addr, port); 
    else socket.connect(addr, port, sf::milliseconds(timeout.count()));
}
void SFMLTcpSocket::disconnect() { socket.disconnect(); }

TcpSocket::Status SFMLTcpSocket::send(const void* data, size_t size) { return (TcpSocket::SendStatus) socket.send(data, size); }
TcpSocket::Status SFMLTcpSocket::send(const void* data, size_t size, size_t& sent) { return (TcpSocket::SendStatus) socket.send(data, size, sent); }
TcpSocket::Status SFMLTcpSocket::receive(void* data, size_t size, size_t& received) { return (TcpSocket::SendStatus) socket.receive(data, size, received); };

class SFMLTcpListener : public TcpListener
{
    sf::TcpListener socket;
    unique_ptr<SFMLTcpSocket> to_construct_socket;
    TcpListener::Status current_status;
public:
    SFMLTcpListener();
    virtual void setBlocking(bool blocking = true) ;
    virtual bool isBlocking() const;
    virtual Port localPort() const;

    virtual void listen(unsigned short port, const IPv4Address& address = IPv4Address::any);
    virtual unique_ptr<TcpSocket> incoming();
    virtual void close() = 0;
};


SFMLTcpListener::SFMLTcpListener() : to_construct_socket( new SFMLTcpSocket ), current_status( TcpListener::Status::Fine ) {}
void SFMLTcpListener::setBlocking(bool blocking = true) { socket.setBlocking(blocking); }
bool SFMLTcpListener::isBlocking() const { return socket.isBlocking(); }
Port SFMLTcpListener::localPort() const { return socket.getLocalPort(); }

void SFMLTcpListener::listen(unsigned short port, const IPv4Address& address = IPv4Address::any) { return socket.listen(port, address); }
unique_ptr<TcpSocket> SFMLTcpListener::incoming()
{
    if(socket.accept( *to_construct_socket ) == sf::Socket::Done )
    {
        unique_ptr<TcpSocket> temp ( move(to_construct_socket) );
        to_construct_socket.reset( new SFMLTcpSocket );
        return move(temp);
    }
    return unique_ptr<TcpSocket>(nullptr);
}
void SFMLTcpListener::close() { socket.close(); }


*/





} /* namespace network */
#endif // NETWORKCONNECTION_HPP
