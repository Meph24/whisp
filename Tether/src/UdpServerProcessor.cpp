#include "UdpServerProcessor.hpp"

#include <mutex>

#include "ClientConnectionListing.hpp"

using std::lock_guard;

UdpServerProcessor::UdpServerProcessor( ClientConnectionListing& clients, Cfg& cfg, const WallClock& wc )
    : clients(clients)
    , wc(wc)
{
    socket.bind( *cfg.getInt("server", "default_udp_port") );
    socket.setBlocking(false);
    send_thread = thread(&UdpServerProcessor::sendProcess, this);
    receive_thread = thread(&UdpServerProcessor::receiveProcess, this);
}

void UdpServerProcessor::stopThreads() 
{ 
    running = false;
    send_thread.join();
    receive_thread.join();
}

void UdpServerProcessor::sendProcess()
{
    std::cout << "Processing outgoing udp : " << running << "\n";
    while(running)
    {
        std::cout << "Connections: " << clients.connections.size() << '\n';
        for(auto& c : clients.connections)
        {
            assert((bool)c);
            std::cout << "Udp handling connection " << c->name << '\n';
            ClientConnection& connection = *c;

            lock_guard lg_ (c->udp_outbox_lock);

/*
            if( !connection.udp_outbox_lock.try_lock() )
            {
                ClientConnection* cptr = c.get();
                if(lock_fail_counters.find(cptr)==lock_fail_counters.end()) lock_fail_counters[cptr] = 0;
                lock_fail_counters[cptr]++;
                std::cout << c->name << " failed to lock outbox " << lock_fail_counters[cptr] << '\n';
                if(lock_fail_counters[cptr] <= lock_fail_tolerance) continue;
                else 
                {
                    std::cout << "Waiting on Lock for " << c->name << '\n';
                    connection.udp_outbox_lock.lock();
                }
            }
*/
            if( connection.udp_outbox.empty() ) 
            {
                std::this_thread::sleep_for(20ms);
                continue;
            }

            syncprotocol::udp::Header header { wc.now(), c->last_client_time };
            connection.udp_outbox.front()->setHeader(header);

            std::cout << "Send udp to " << connection.tcpsocket.getRemoteAddress() << ':' << connection.udpport << '\n'; 
            socket.send( *connection.udp_outbox.front(), connection.tcpsocket.getRemoteAddress(), connection.udpport);
            std::cout << "Sent Udp Packet to " << connection.tcpsocket.getRemoteAddress() << ':' << connection.udpport << "!\n";
            connection.udp_outbox.pop_front();
            lock_fail_counters[c.get()] = 0;

            connection.udp_outbox_lock.unlock();
        }
    }
}

void UdpServerProcessor::receiveProcess()
{
    unique_ptr<sf::Packet> newpacket;
    sf::IpAddress sender_addr; Port sender_port;

    std::cout << "Processing incoming udp.\n";
    while(running)
    {
        newpacket = std::make_unique<sf::Packet>();
        if( sf::Socket::Status::Done != socket.receive( *newpacket, sender_addr, sender_port ))
            continue;

        if( !clients.containsUdp(sender_addr, sender_port) )
        {
            std::cout << "Received from unknown sender " << sender_addr << ':' << sender_port << '\n';
            //sender not known!
            //sender not known behavior goes here ... see ticket #55
            continue;
        }
        ClientConnection& connection = *clients.atUdp(sender_addr, sender_port);
        syncprotocol::udp::Header header; *newpacket >> header;
        connection.latency_ = (header.server_time - wc.now()) / 2;
        if(connection.last_client_time < header.client_time) 
            connection.last_client_time = header.client_time;

        lock_guard lg_ (clients.atUdp(sender_addr, sender_port)->udp_inbox_lock);
        connection.udp_inbox.emplace_back(std::move(newpacket));
    }
}