#include "protocol.hpp"

#include "sfml_packet_utils.hpp"

namespace syncprotocol
{
    bool receiveInTime(WallClock& wc, sf::TcpSocket& socket, char* buffer, size_t size, WallClock::duration timeout_duration)
    {
        WallClock::time_point call_time = wc.now();
        size_t received = 0;
        socket.setBlocking(false);
        while( received < size &&  wc.now() - call_time < timeout_duration )
        {
            socket.receive(buffer + received, size - received, received);
        }
        return received == size;
    }

    ClientIntroduction::ClientIntroduction(string client_name, Port client_udp_port)
        : client_udp_port(client_udp_port)
    {
        setClientName(client_name);
    }


    void ClientIntroduction::setClientName(string input_client_name)
    {
        std::fill(clientname, clientname+sizeof(clientname), '\0');
        if(input_client_name.size() < 0 || input_client_name.size() > sizeof(clientname) - 1 )
            throw std::runtime_error("Input client name is wrongly sized [max " + std::to_string(sizeof(clientname)-1) + "bit]");
        input_client_name.copy(clientname, input_client_name.size());
    }

    ostream& operator<< (ostream& os, const ClientIntroduction& ci)
    {
        return os   << "Name: " << ci.clientname << ", Port UDP: " << ci.client_udp_port;
    }

    ostream& operator<< (ostream& os, const syncprotocol::ClientToken& t)
    {
        return os << "IPv4: " << t.server_known_fullip << "| SyncID: " << t.avatar_syncid;
    }

    ServerInfo::ServerInfo(string server_name, Port udpport)
        : udpport(udpport)
    {
        setName(server_name);
    }
    void ServerInfo::setName(string server_name)
    {
        std::fill(name, name+sizeof(name), '\0');
        if(server_name.size() < 0 || server_name.size() > sizeof(name)-1)
            throw std::runtime_error("Input server name is wrongly sized [max " + std::to_string(sizeof(name)-1) + " bit]");
        server_name.copy(name, server_name.size());
    }

    ostream& operator<< (ostream& os, const syncprotocol::ServerInfo& server_info)
    {
        return os   << "Dwengine Server Information:\n"
                    << "Name: " << server_info.name << '\n'
                    << "UdpPort: " << server_info.udpport << '\n';
    }

    namespace udp {
        Packet::Packet() : Packet(Header()) {}
        Packet::Packet( const Header& h ){ *this << h; }
        sf::Packet& operator<<(sf::Packet& p, const Header& h){ return p << h.server_time << h.client_time; }
        sf::Packet& operator>>(sf::Packet& p, Header& h){ return p >> h.server_time >> h.client_time; }
        void Packet::setHeader(const Header& h)
        {
            Packet dummy(h);
            std::memcpy(const_cast<void*>(getData()), dummy.getData(), dummy.getDataSize());
        }
    } // namespace udp

} // namespace syncprotocol