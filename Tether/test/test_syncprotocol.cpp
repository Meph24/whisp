#include<gtest/gtest.h>

#include "../src/protocol.hpp"

#include <cstdlib> //for rand()

//HACK TEST
//we test the hack to write a header inside a sfml packet, that already has other data in it.
TEST(test_syncprotocol_udp_header, set_header)
{
    using namespace syncprotocol::udp;

    Packet p;

    //we made the assumption that buffer size and size of the input should match
    EXPECT_EQ(p.getDataSize(), sizeof(Header));

    //payload gets inserted
    for(size_t i = 0; i<42; ++i) 
        p << i;

    //after payload is in we set the header
    srand(time(NULL));
    WallClock::time_point server_time( duration_cast<WallClock::duration>(std::chrono::microseconds(rand())) );
    WallClock::time_point client_time( duration_cast<WallClock::duration>(std::chrono::microseconds(rand())) );
    Header h { server_time, client_time };
    p.setHeader(h);

    //retrieve header
    Header received;
    p >> received;

    EXPECT_EQ( h.server_time, received.server_time );
    EXPECT_EQ( h.client_time, received.client_time );

    size_t payload;
    for(size_t i = 0; i<42; ++i)
    {
        p >> payload; EXPECT_EQ(i, payload);
    }
}

TEST(test_syncprotocol_udp_header, payload_assumptions)
{
    using namespace syncprotocol::udp;

    Packet p;

    //we made the assumption that buffer size and size of the input should match
    EXPECT_EQ(sizeof(Header), p.getDataSize());

    //and that payload is added in the back
    size_t buffer_size = p.getDataSize();
    char * buffer = new char[buffer_size];

    //save contents from before adding payload
    std::memcpy(buffer, p.getData(), p.getDataSize());

    for(size_t i = 0; i<42; ++i) 
        p << i;

    //sizes added add up to the complete thing
    EXPECT_EQ(sizeof(Header) + 42*sizeof(size_t), p.getDataSize());

    //header information is still in the front
    EXPECT_EQ(0, std::memcmp(buffer, p.getData(), buffer_size));

    delete[] buffer;
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}