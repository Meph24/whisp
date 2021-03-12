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
    connection.tcpsocket.setBlocking(false);
    unique_ptr<sf::Packet> incoming = std::make_unique<sf::Packet>();
    if(connection.tcpsocket.receive(*incoming) == sf::Socket::Done && incoming->getDataSize())
    {
        syncman.applyEventPacket(*incoming);
    }

    //udp ; update packets
    std::cout << "1";
    incoming->clear();
    std::cout << "2";
    incoming = connection.receiveUdp();
    if(incoming)
    {
        std::cout << "3";
        syncman.applyUpdatePacket(*incoming);
        std::cout << "4";
    }
}

EntityPlayer* SimulationClient::avatar() const
{
    auto f = syncman.syncMap.find(connection.client_token.avatar_syncid);
    return ( f == syncman.syncMap.end() )? nullptr : (EntityPlayer*) f->second;
}