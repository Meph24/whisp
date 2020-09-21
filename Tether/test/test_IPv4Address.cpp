#include "../src/NetworkConnection.hpp"

#include <gtest/gtest.h>

#include <numeric>

using namespace network;

TEST(test_Ipv4Address, uint8_t_construction)
{
    IPv4Address addr(255, 255, 255, 255);
    EXPECT_EQ(std::numeric_limits<uint32_t>::max(), addr.value() );
}

TEST(test_Ipv4Address, uint32_t_construction)
{   
    IPv4Address addr(1370);
    EXPECT_EQ(addr.value(), (uint32_t)1370);
}

TEST(test_Ipv4Address, string_construction)
{
    IPv4Address addr("255.255.255.255");
    EXPECT_EQ(std::numeric_limits<uint32_t>::max(), addr.value() );
}

TEST(test_Ipv4Address, default_zero)
{
    IPv4Address addr;
    EXPECT_EQ((uint32_t)0, addr.value());
}

TEST(test_Ipv4Address, copy_constructable)
{
    IPv4Address addr (255, 255, 255, 255);
    IPv4Address addr_copy (addr);
    EXPECT_EQ(addr, addr_copy);
}

TEST(test_Ipv4Address, copy_assignable)
{
    IPv4Address addr (255, 255, 255, 255);
    IPv4Address addr_copy = addr;
    EXPECT_EQ(addr, addr_copy);
}


TEST(test_Ipv4Address, equality)
{
    IPv4Address addr0 (255, 255, 255, 255);
    IPv4Address addr1 = addr0;

    EXPECT_EQ(addr0, addr1);

    addr1 = IPv4Address(1370);
    EXPECT_NE(addr0, addr1);
}

TEST(test_Ipv4Address, order)
{
    IPv4Address addr0, addr1, addr2;
    addr0 = IPv4Address(0,0,0,0);
    addr1 = IPv4Address(0,0,1,0);
    addr2 = IPv4Address(0,200,0,0);

    EXPECT_LT(addr0, addr1);
    EXPECT_LT(addr1, addr2);
    EXPECT_GT(addr1, addr0);
    EXPECT_GT(addr2, addr1);

    IPv4Address addr1_1 = addr1;

    EXPECT_LE(addr1, addr1_1);
    EXPECT_GE(addr1, addr1_1);
}

TEST(test_Ipv4Address, explicitly_to_uint32_t)
{
    IPv4Address addr(255, 255, 255, 255);
    uint32_t i = (uint32_t) addr;

    EXPECT_EQ(std::numeric_limits<uint32_t>::max(), i);
}

TEST(test_Ipv4Address, explicitly_to_string)
{   
    string s = "255.255.255.255";
    IPv4Address addr(s);
    string got = (string) addr;

    EXPECT_EQ(s, got);
}

TEST(test_Ipv4Address, string_format_exception)
{
    int exception_counter = 0;
    try
    {
        IPv4Address addr("This is not an ip address");
    }
    catch(const input_format_error& e)
    {
        std::cerr << e.what() << '\n';
        exception_counter++;
    }
    try
    {
        IPv4Address addr("1111.0.0.0");
    }
    catch(const input_format_error& e)
    {
        std::cerr << e.what() << '\n';
        exception_counter++;
    }
    EXPECT_EQ(2, exception_counter);
}

TEST(test_Ipv4Address, input_number_out_of_bounds_via_string)
{
    int exception_counter = 0;
    try
    {
        IPv4Address addr("256.0.0.0");
    }
    catch(const input_format_error& e)
    {
        std::cerr << e.what() << '\n';
        exception_counter++;
    }
    EXPECT_LT(0, exception_counter);
}