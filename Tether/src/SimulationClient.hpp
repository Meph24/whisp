#ifndef SIMULATIONCLIENT_HPP
#define SIMULATIONCLIENT_HPP

#include <string>

#include "ServerConnection.hpp"
#include "SyncableManager.h"
#include "WallClock.hpp"

struct SimulationClient
{
    WallClock& wc;
    string name;
    ServerConnection connection;

    SyncableManager syncman;

    bool connected() const;
    bool initialized() const;
    bool processInitialSync();
    void processCyclicSync();
    EntityPlayer* avatar() const;

    SimulationClient(WallClock& wc, Cfg& cfg);
    ~SimulationClient();
};

#endif // SIMULATIONCLIENT_HPP
