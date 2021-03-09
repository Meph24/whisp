#ifndef CLIENTCONNECTIONLISTING_HPP
#define CLIENTCONNECTIONLISTING_HPP

#include <memory>
#include <vector>

#include "ClientConnection.hpp"

using std::unique_ptr;
using std::vector;

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

#endif // CLIENTCONNECTIONLISTING_HPP
