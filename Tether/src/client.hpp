#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "IPv4Address.hpp"
#include "MainApp.hpp"
#include "SimulationClient.hpp"
#include "SyncableManager.h"
#include "WallClock.hpp"

using network::Port;

struct Cfg;

struct ClientApp : public App
{
    SimulationClient client;
    LocalUser user;

    ClientApp(WallClock& wc, Cfg& cfg, IPv4Address& addr, Port port);
    void run();
};

#endif // CLIENT_HPP
