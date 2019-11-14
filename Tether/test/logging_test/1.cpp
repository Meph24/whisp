#include "../../src/Logging.hpp"

#include <chrono>
#include <fstream>
#include <iostream>

using std::cout;
using log::Logging;

template<typename clock_type>
unsigned int fib (unsigned int x, Logging<clock_type>& l)
{
	l.info() << "calculating fib " << x;
	cout << "calculating fib" << x << '\n';
	return (x < 3)? 1 : fib(x-1, l) + fib(x-2, l);
}

using this_clock = std::chrono::system_clock;

int main (int argc , char** argv)
{
	Logging<this_clock> logging("This is a test logging INFO at the start of the log!", std::ofstream("log1.log"));

	logging.setMinLevel(Logging<this_clock>::TYPE::INFO, 100);

	for ( int i = 25; i < 28; ++i )
	{
		if(i == 26)
		{
			logging.addOfstream(std::ofstream("log2.log"));
		}
		logging.warning() << "Fib " << i << " start.";
		logging.info() << "processing ...";
		fib(i, logging);
		logging.error() << "Fib" << i << "ended.";
	}

	logging.info(99) << "This Log should not be displayed!";
	
	return 0;
}
