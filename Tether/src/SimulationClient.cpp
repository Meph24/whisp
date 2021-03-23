#include "SimulationClient.hpp"

#include "EntityPlayer.h"

SimulationClient::SimulationClient(WallClock& wc, Cfg& cfg)
    : wc(wc)
    , name(*cfg.getStr("client", "name"))
    , connection(*this, cfg)
    , syncman(wc, cfg)
    , upload_budget(wc, 50000)
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
    upload_budget.reset();
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
    incoming->clear();
    incoming = connection.receiveUdp();
    if(incoming)
    {
        syncman.applyUpdatePacket(*incoming);
    }
}

EntityPlayer* SimulationClient::avatar() const
{
    auto f = syncman.syncMap.find(connection.client_token.avatar_syncid);

    return 
        ( f == syncman.syncMap.end() )? 
        nullptr : static_cast<EntityPlayer*>(f->second);
}

#include "FloatSeconds.hpp"
#include "User.hpp"

void SimulationClient::sendInput(SimulationUser* user)
{
    if(!user || !connection.connected()) return;

    if( upload_budget.current() < (int) sizeof(user->input_status) ) return;

    syncprotocol::udp::Header header {connection.latestServerTime(), wc.now()};
    syncprotocol::udp::Packet new_packet( header );

    //this timestamp is redundant
    //it is in, bc InputStatusSet had and has its own timestamp for remotecontrol.
    // we need to evaluate how we remove this redundancy at some point
    user->input_status.timestamp = wc.now().time_since_epoch().count();
    new_packet << user->input_status;
    connection.sendUdp( new_packet );

    upload_budget.used(new_packet.getDataSize());
}