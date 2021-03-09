#include "server.hpp"

#include <iostream> //replace with logging #56

#include "protocol.hpp"
#include "Simulation.hpp"
#include "Simulation_World.h"
#include "SyncableManager.h"
#include "Zombie_World.h"

HostApp::HostApp(WallClock& wc, Cfg& cfg, Port port)
    : server(wc, cfg, port)
	, local_user(cfg, *cfg.getStr("general", "application_name") + string(" - Host"),
        *cfg.getInt("graphics", "resolutionX"), *cfg.getInt("graphics", "resolutionY"))
{
    std::cout << "Initializing Host Application !\n";

	int zombie_mode = *cfg.getInt("test", "zombie_mode");
	if(zombie_mode) 
	{
		simulation = std::make_unique<Zombie_World>(wc, cfg);
	}
	else 
	{
		simulation = std::make_unique<Simulation_World>(wc, cfg);
	}
    if(!simulation) throw std::runtime_error("Simulation could not be initialized!");

    local_user.operateSimulation(*simulation);
    syncman = std::make_unique<SyncableManager>(*simulation);
    server.setup(*syncman);
}

void HostApp::run()
{
    if(!simulation || !syncman) 
		throw std::runtime_error( "Application cannot be run! Initialization Failure!" );

    simulation->init();

    while(local_user.window.isOpen())
    {
        simulation->step();

        server.process(); 

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        local_user.draw();
        local_user.window.display();

        local_user.pollEvents();
    }
}

