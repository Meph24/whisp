#include "UdpServerProcessor.hpp"

#include <mutex>

#include "ClientConnectionListing.hpp"

using std::lock_guard;

UdpServerProcessor::UdpServerProcessor( ClientConnectionListing& clients, Cfg& cfg, const WallClock& wc )
    : clients(clients)
    , wc(wc)
{
    socket.bind( *cfg.getInt("server", "default_udp_port") );
    socket.setBlocking(true);
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
    while(running)
    {
        for(auto& c : clients.connections)
        {
            ClientConnection& connection = *c;
            if( connection.udp_outbox.empty() ) continue;
            
            if( !connection.udp_outbox_lock.try_lock() )
            {
                ClientConnection* cptr = c.get();
                if(lock_fail_counters.find(cptr)==lock_fail_counters.end()) lock_fail_counters[cptr] = 0;
                lock_fail_counters[cptr]++;
                if(lock_fail_counters[cptr] < lock_fail_tolerance) continue;
                else connection.udp_outbox_lock.lock();
            }

            syncprotocol::udp::Header header { wc.now(), c->last_client_time };
            connection.udp_outbox.front()->setHeader(header);

            socket.send( *connection.udp_outbox.front(), connection.tcpsocket.getRemoteAddress(), connection.udpport);
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
        newpacket.reset(new sf::Packet);
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