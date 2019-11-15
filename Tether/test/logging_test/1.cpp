#include "../../src/Logging.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

using std::unique_ptr;

using std::cout;
using log::Logging;

template<typename clock_type>
unsigned int fib (unsigned int x, Logging<clock_type>& l)
{
	l.info() << "calculating fib " << x;
	return (x < 3)? 1 : fib(x-1, l) + fib(x-2, l);
}

using this_clock = std::chrono::system_clock;

int main (int argc , char** argv)
{
	Logging<this_clock> logging("This is a test logging INFO at the start of the log!", ofstream("log1.log"));

	logging.setMinLevel(log::TYPE::INFO, 100);
	logging.activateStdOut();
	logging.setStdOutMapping(log::TYPE::WARNING, std::clog);
	

	for ( int i = 25; i < 28; ++i )
	{
		if(i == 26)
		{
			logging.addOfstream(ofstream("log2.log"));
		}
		logging.warning() << "Fib " << i << " start.";
		logging.info() << "processing ...";
		fib(i, logging);
		logging.error() << "Fib" << i << "ended.";
	}

	logging.info(99) << "This Log should not be displayed!";
	
	return 0;
}
