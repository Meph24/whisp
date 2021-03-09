#include "client.hpp"

#include <iostream> //TODO replace with logging #56

#include "protocol.hpp"

using std::cout; //TODO replace with logging #56

SimulationClient::SimulationClient(WallClock& wc, Cfg& cfg)
    : wc(wc)
    , name(*cfg.getStr("client", "name"))
    , connection(*this, cfg)
    , syncman(wc, cfg)
{
    connection.tcpsocket.setBlocking(false);
}

SimulationClient::~SimulationClient()
{
    connection.disconnect();
}

bool SimulationClient::connected() const { return connection.connected(); }
bool SimulationClient::initialized() const { return syncman.getSim(); }

bool SimulationClient::processInitialSync()
{
    if(initialized()) return true;
    if(!connected()) return false;

    sf::Packet incoming_tcp;

    if(connection.tcpsocket.receive(incoming_tcp) != sf::Socket::Done)
        return false;
    syncman.applyEventPacket(incoming_tcp);
    return initialized();
}

void SimulationClient::processCyclicSync()
{
    if(!connected() || !initialized()) return;

    //tcp ; game events
    unique_ptr<sf::Packet> incoming;
    if(connection.tcpsocket.receive(*incoming) == sf::Socket::Done && incoming->getDataSize())
    {
        syncman.applyEventPacket(*incoming);
    }

    //udp ; update packets
    incoming->clear();
    incoming = connection.receiveUdp();
    if(incoming)
        syncman.applyUpdatePacket(*incoming);
}

EntityPlayer* SimulationClient::avatar() const
{
    //TODO implement
    //the avatar must be sampled from the syncable manager
    //the syncable manager must however be updated, so that
    // the entity player can be probed
    return nullptr;
}

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

    while(user.window.isOpen())
    {
        if(!client.avatar())
        {
            //Switch the color to give visual feedback of no avatar
            glClearColor(0.5f, 0.0f, 0.0f, 0.0f);
            continue;
        }
        else
        {
            glClearColor(0.0f, 0.0f, 0.25f, 0.0f);
        }
        client.processCyclicSync();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        user.draw();
        user.window.display();

        user.pollEvents();
    }
}