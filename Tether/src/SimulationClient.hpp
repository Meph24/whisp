#ifndef SIMULATIONCLIENT_HPP
#define SIMULATIONCLIENT_HPP

#include <string>

#include "ServerConnection.hpp"
#include "SyncableManager.h"
#include "UploadBudget.hpp"
#include "WallClock.hpp"

struct SimulationUser;

struct SimulationClient
{
    WallClock& wc;
    string name;
    ServerConnection connection;

    SyncableManager syncman;
    UploadBudget upload_budget;

    bool connected() const;
    bool initialized() const;
    bool processInitialSync();
    void processCyclicSync();
    EntityPlayer* avatar() const;

    void sendInput(SimulationUser* user);
    
    SimulationClient(WallClock& wc, Cfg& cfg);
    ~SimulationClient();
};

#endif // SIMULATIONCLIENT_HPP
