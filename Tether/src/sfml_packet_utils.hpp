#ifndef SFML_PACKET_UTILS_HPP
#define SFML_PACKET_UTILS_HPP

#include <SFML/Network/Packet.hpp>

#include <chrono>

using sf::Packet;

template<typename rep, typename period>
Packet& operator<<(Packet& p, const std::chrono::duration<rep, period>& d) 
{ return p << d.count(); }

template<typename rep, typename period>
Packet& operator>>(Packet& p, std::chrono::duration<rep, period>& d) 
{ 
    rep r;
    p >> r;
    d = std::chrono::duration<rep, period>(r);
    return p;
}

template<typename clock_t, typename duration_t>
Packet& operator<<(Packet& p, const std::chrono::time_point<clock_t, duration_t>& tp) 
{ return p << tp.time_since_epoch(); }


template<typename clock_t, typename duration_t>
Packet& operator>>(Packet& p, std::chrono::time_point<clock_t, duration_t>& tp)
{
    duration_t d;
    p >> d;
    tp = std::chrono::time_point<clock_t, duration_t>(d);
    return p;
}


#endif // SFML_PACKET_UTILS_HPP
