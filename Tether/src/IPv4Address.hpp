#ifndef IPV4ADDRESS_HPP
#define IPV4ADDRESS_HPP

#include <cstdint>
#include <iostream>
#include <regex>
#include <string>
#include "error.hpp"

using std::string;
using std::regex;
using std::ostream;

namespace network {

class IPv4Address
{
    uint32_t v;
public:
    static regex re_ipv4;
    static uint32_t parseIPv4String(const string&);
    static uint32_t combineBytesToIPv4(uint8_t, uint8_t, uint8_t, uint8_t);
    IPv4Address();
    IPv4Address(uint8_t, uint8_t, uint8_t, uint8_t);
    IPv4Address(uint32_t);
    IPv4Address(const string&);
    IPv4Address(const IPv4Address&) = default;
    IPv4Address& operator=(const IPv4Address&) = default;

    bool operator<(const IPv4Address& other) const;
    bool operator>(const IPv4Address& other) const;
    bool operator<=(const IPv4Address& other) const;
    bool operator>=(const IPv4Address& other) const;
    bool operator==(const IPv4Address& other) const;
    bool operator!=(const IPv4Address& other) const;

    uint32_t value() const;

    explicit operator uint32_t () const;
    explicit operator string () const;

    static const IPv4Address any;

    friend ostream& operator<<(ostream& os, const IPv4Address& addr);
};

ostream& operator<<(ostream& os, const IPv4Address& addr);

using Port = uint16_t;

} /* namespace network */

#endif // IPV4ADDRESS_HPP
