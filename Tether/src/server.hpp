#ifndef SERVER_HPP
#define SERVER_HPP

#include <memory>

#include "MainApp.hpp"
#include "Simulation.hpp"
#include "SimulationServer.hpp"
#include "User.hpp"

using std::unique_ptr;

struct HostApp : public App
{
    unique_ptr<Simulation> simulation;

    SimulationServer server;
    unique_ptr<SyncableManager> syncman;

    LocalUser local_user;

    HostApp(WallClock& wc, Cfg& cfg, Port port);
    ~HostApp();
    void run();
};

#endif // SERVER_HPP
