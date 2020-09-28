#include <gtest/gtest.h>

#include "../src/NetworkConnection.hpp"

#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono;

using namespace network;

TEST(test_NetworkConnection, single_connection)
{
    ConnectionReception reception;

    reception.listen( 55555 );
    std::this_thread::sleep_for( 50ms );

    TcpSocket remote_socket;
    remote_socket.connect( "127.0.0.1", 55555 );

    std::this_thread::sleep_for( 10ms );
    EXPECT_TRUE( reception.processNextIncoming() );
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}