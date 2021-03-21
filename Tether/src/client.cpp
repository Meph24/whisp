#include "client.hpp"

#include <iostream> //TODO replace with logging #56

#include "protocol.hpp"

using std::cout; //TODO replace with logging #56

ClientApp::ClientApp(WallClock& wc, Cfg& cfg, IPv4Address& addr, Port port)
    : client(wc, cfg)
	, user(cfg, "Dwengine - Client", *cfg.getInt("graphics", "resolutionX"), *cfg.getInt("graphics", "resolutionY"))
{
    if( !client.connection.tryConnect(cfg, addr, port) )
        cout << "Connection failed on " << addr << ':' << port << " !\n";
    else
    {
       cout << "Connection established !\n";
       cout << "Client Name : " << client.name << '\n';
       cout << "Server : " << client.connection.server_info.name << '\n';
    }
}

void ClientApp::run()
{
    cout << "Waiting on Simulation to be transmitted from server ...\n";
    {
        //for visual feedback of processing on terminal
        char wheel[4] = {'-', '\\', '|', '/' };
        int wheelindex = 0;
        
        glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
        while(user.window.isOpen() && !client.processInitialSync())
        {
            wheelindex = (wheelindex + 1) % sizeof(wheel);
            cout << '\r' << wheel[wheelindex];
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            user.window.display();
            InputEvent e; while(user.window.pollEvent(e)); //keep window open
        }
        cout << "\rSimulation received!\n";
    }

    if(!client.syncman.getSim()) throw std::runtime_error("Simulation expected on client!");

    user.perspective = std::make_unique<Perspective>( user, *client.syncman.getSim(), nullptr);
    while(user.window.isOpen() && client.syncman.getSim())
    {
        if(!client.avatar())
        {
            //Switch the color to give visual feedback of no avatar
            glClearColor(0.5f, 0.0f, 0.0f, 0.0f);
        }
        else
        {
            glClearColor(0.0f, 0.0f, 0.25f, 0.0f);
        }
        user.perspective->setAvatar(client.avatar());

        client.processCyclicSync();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        user.draw();
        user.window.display();

        user.pollEvents();
        client.sendInput(&user);
    }
}

