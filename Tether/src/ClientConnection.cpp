#include "ClientConnection.hpp"

#include "Simulation.hpp"

void SyncUser::operateSimulation(Simulation& simulation)
{
    if(this->simulation) return;

    this->simulation = &simulation;
    simulation.makeAvatarFor(this);
    if(!avatar()) { std::cerr << "User could not be registered in Simulation!" << std::endl; }
}
void SyncUser::disconnectSimulation()
{
    if(simulation) simulation->disconnectAvatarFrom(this);
}

FullIPv4 ClientConnection::remoteUdpFullip() const
{
    return FullIPv4( tcpsocket.getRemoteAddress().toInteger(), udpport );
}

const WallClock::duration& ClientConnection::latency() const { return latency_; }