#ifndef UDPSERVERPROCESSOR_HPP
#define UDPSERVERPROCESSOR_HPP

#include <map>
#include <thread>

#include <SFML/Network/UdpSocket.hpp>

#include "ClientConnection.hpp"
#include "WallClock.hpp"

using std::map;
using std::thread;

struct ClientConnectionListing;

struct UdpServerProcessor
{
    ClientConnectionListing& clients;
    sf::UdpSocket socket;

    map<ClientConnection*, uint8_t> lock_fail_counters;
    uint8_t lock_fail_tolerance = 1;

    const WallClock& wc;

    UdpServerProcessor( ClientConnectionListing& clients, Cfg& cfg, const WallClock& wc);

    bool running = true;
    void stopThreads();

    thread send_thread;
    thread receive_thread;
    void sendProcess();
    void receiveProcess();
};

#endif // UDPSERVERPROCESSOR_HPP
