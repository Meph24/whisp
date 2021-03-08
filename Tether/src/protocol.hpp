#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <memory>

#include <SFML/Network.hpp>

#include "IPv4Address.hpp"
#include "WallClock.hpp"
#include "Syncable.h"

using std::unique_ptr;

using network::IPv4Address;
using network::Port;
using network::FullIPv4;

namespace syncprotocol {

    bool receiveInTime( WallClock& wc,
                        sf::TcpSocket& socket, 
                        char* buffer, size_t size, 
                        WallClock::duration timeout_duration);

    bool receiveInTime( WallClock& wc,
                        sf::UdpSocket& socket, 
                        sf::Packet& p, 
                        WallClock::duration timeout_duration);
                        
    const char introduction_success[9] = "SUCCESS";
    const char introduction_fail[9] = "FAILURE";
    const size_t introduction_response_size = 9;
    const char client_introduction_greeting[41] = "Hi, I want to join this Dwengine Server!";

    struct ClientIntroduction
    {
        char greeting[41] = {0};
        char clientname [64] = {0};
        Port client_udp_port = 0;

        ClientIntroduction() = default;
        ClientIntroduction(string client_name, Port client_udp_port);
        void setClientName(string input_client_name);
    };
    ostream& operator<< (ostream&, const ClientIntroduction&);
    struct ServerInfo
    {
        char heading[29] = "Dwengine Server Information:";
        char name[64] = {0};
        Port udpport = 0;

        ServerInfo() = default;
        ServerInfo(string server_name, Port udpport);
        void setName(string server_name);
    };
    ostream& operator<< (ostream&, const ServerInfo&);
    struct ClientToken
    {
        FullIPv4 server_known_fullip;
        syncID avatar_syncid = 0;
    };
    ostream& operator<< (ostream&, const ClientToken&);

    namespace udp {
         struct Header
        {
            WallClock::time_point server_time, client_time;
        };
        struct Packet : public sf::Packet
        {
            using sf::Packet::operator<<;
            using sf::Packet::operator>>;
            void setHeader(const Header&);
        };

        sf::Packet& operator<<(sf::Packet& p, const Header& h);
        sf::Packet& operator>>(sf::Packet& p, Header& h);
    } // namespace udp
} // namespace syncprotocol

#endif // PROTOCOL_HPP
