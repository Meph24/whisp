#include "IPv4Address.hpp"

#include "error.hpp"

using std::smatch;
using std::to_string;

namespace network {

regex IPv4Address::re_ipv4 = regex(R"x(^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)x");
uint32_t IPv4Address::parseIPv4String(const string& addr)
{
    smatch sm;
    if(! regex_search(addr, sm, re_ipv4) )
    {
        throw input_format_error("Ipv4Address: Input string was not recognized as a valid ipv4-address!"); 
    }
    return combineBytesToIPv4( stoi(sm[1]), stoi(sm[2]), stoi(sm[3]), stoi(sm[4]) );
}
uint32_t IPv4Address::combineBytesToIPv4(uint8_t b3, uint8_t b2, uint8_t b1, uint8_t b0)
{
    return  (uint32_t) 0 + ((uint32_t) b3 << 24) + ((uint32_t) b2 << 16) + ((uint32_t) b1 << 8) + ((uint32_t) b0);
}
IPv4Address::IPv4Address() : v(0) {}
IPv4Address::IPv4Address(uint8_t b3, uint8_t b2, uint8_t b1, uint8_t b0)
: v (combineBytesToIPv4(b3, b2, b1, b0)) {}
IPv4Address::IPv4Address(uint32_t addr) : v(addr) {}
IPv4Address::IPv4Address(const string& addr) : v(parseIPv4String(addr)) {}

bool IPv4Address::operator<(const IPv4Address& other) const { return v < other.v; }
bool IPv4Address::operator>(const IPv4Address& other) const { return v > other.v; }
bool IPv4Address::operator<=(const IPv4Address& other) const { return v <= other.v; }
bool IPv4Address::operator>=(const IPv4Address& other) const { return v >= other.v; }
bool IPv4Address::operator==(const IPv4Address& other) const { return v == other.v; }
bool IPv4Address::operator!=(const IPv4Address& other) const { return v != other.v; }
uint32_t IPv4Address::value() const { return v; }
IPv4Address::operator uint32_t () const { return v; }
IPv4Address::operator string () const { return to_string(v >> 24 & 0xFF) + string(".") + to_string(v >> 16 & 0xFF) + string(".") + to_string(v >> 8 & 0xFF) + string(".") + to_string(v & 0xFF); }
const IPv4Address IPv4Address::any = IPv4Address(0,0,0,0);

} /* namespace network */