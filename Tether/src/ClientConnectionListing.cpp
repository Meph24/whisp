#include "ClientConnectionListing.hpp"

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
