#ifndef SIMULATIONCLIENT_HPP
#define SIMULATIONCLIENT_HPP

#include <string>

#include "ServerConnection.hpp"
#include "SyncableManager.h"
#include "WallClock.hpp"

struct SimulationUser;

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

    void sendInput(SimulationUser* user);
    
    WallClock::time_point last_sent;
    float max_send_bytes_per_second = 3000;

    SimulationClient(WallClock& wc, Cfg& cfg);
    ~SimulationClient();
};

#endif // SIMULATIONCLIENT_HPP
