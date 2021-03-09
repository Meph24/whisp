#include "SimulationClient.hpp"

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