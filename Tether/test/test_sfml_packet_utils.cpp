#include<gtest/gtest.h>

#include "../src/sfml_packet_utils.hpp"

using namespace std::chrono_literals;

TEST(packet_stream, time_types_duration)
{
    sf::Packet p;
    using microseconds = std::chrono::microseconds;
    microseconds t(42us);

    auto size0 = p.getDataSize();
    p << t;

    EXPECT_GT(p.getDataSize(), size0);

    microseconds out_time;
    p >> out_time;

    EXPECT_EQ(t, out_time);
}

TEST(packet_stream, time_types_time_point)
{
    sf::Packet p;
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();

    auto size0 = p.getDataSize();
    p << tp;

    EXPECT_GT(p.getDataSize(), size0);

    std::chrono::system_clock::time_point out_tp;
    p >> out_tp;

    EXPECT_EQ(tp, out_tp);
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}