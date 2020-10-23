#include "NetworkConnection.hpp"

#include <iostream>

namespace network{

ConnectionHead::ConnectionHead( unique_ptr<TcpSocket>&& socket ) : tcpsocket( ::std::move(socket) ) {}

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

} // namespace network