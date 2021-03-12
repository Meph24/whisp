#include "ClientConnection.hpp"

#include "Simulation.hpp"

void SyncUser::operateSimulation(Simulation& simulation)
{
    if(this->simulation) return;

    this->simulation = &simulation;
    simulation.registerUser(this);
    if(!avatar()) { std::cerr << "User could not be registered in Simulation!" << std::endl; }
}
void SyncUser::disconnectSimulation()
{
    simulation->kickUser(this);
}

void ClientConnection::sendUdp(shared_ptr<syncprotocol::udp::Packet>& p)
{
    if(!p->getDataSize()) return;
    std::lock_guard<std::mutex> lockguard (udp_outbox_lock);
    udp_outbox.emplace_back(p);
}

unique_ptr<sf::Packet> ClientConnection::receiveUdp()
{
    if(udp_inbox.empty()) return nullptr;
    
    std::lock_guard<std::mutex> lockguard (udp_inbox_lock);
    unique_ptr<sf::Packet> newp = std::move(udp_inbox.front());
    udp_inbox.pop_front();
    return newp;
}

FullIPv4 ClientConnection::remoteUdpFullip() const
{
    return FullIPv4( tcpsocket.getRemoteAddress().toInteger(), udpport );
}

const WallClock::duration& ClientConnection::latency() const { return latency_; }