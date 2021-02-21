#ifndef SYNCPROTOCOL_HPP
#define SYNCPROTOCOL_HPP

#include <memory>
#include <string>

#include <SFML/Network.hpp>

#include "IPv4Address.hpp"
#include "WallClock.hpp"

using std::string;
using std::unique_ptr;

using network::IPv4Address;
using network::Port;
using network::FullIPv4;

namespace syncprotocol
{
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
        uint8_t client_id = 0;
        FullIPv4 server_known_fullip;
    };
    ostream& operator<< (ostream&, const ClientToken&);

    namespace udp
    {

        struct TestMessage
        {
        char m [28] = "This is a udp test message.";
            uint16_t nonce = 42;
        };

        sf::Packet& operator<<(sf::Packet&, const TestMessage&);
        sf::Packet& operator>>(sf::Packet&, TestMessage&);

        enum Topic
        {
            None = 0,
            Test,
            NUM_TOPICS
        };

        unique_ptr<sf::Packet> newUdpPacket(Topic t);
        Topic extractTopic(sf::Packet& p);
    };
}/* namespace syncmessages */

#endif // SYNCPROTOCOL_HPP
