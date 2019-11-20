#include "../../src/SessionLog.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

namespace fs = std::filesystem;
using std::unique_ptr;

using std::cout;
using log::SessionLog;

template<typename clock_type>
unsigned int fib (unsigned int x, Logging<clock_type>& l)
{
	l.info() << "calculating fib " << x;
	return (x < 3)? 1 : fib(x-1, l) + fib(x-2, l);
}

using this_clock = std::chrono::high_resolution_clock;
using other_clock = std::chrono::system_clock;

int main (int argc , char** argv)
{
	log::Session session(string("This is the info to a Session Log of APP testxy!"), fs::path("."));
	SessionLog<this_clock> logging(session, "test this", "This is a test logging INFO at the start of the log!");

	SessionLog<other_clock>logging2(session, "second", "The second logging with same session!");

	logging.setMinLevel(log::TYPE::INFO, 100);
	logging.activateStdOut();
	logging.setStdOutMapping(log::TYPE::WARNING, std::clog);
	
	logging.log("custom tag") << "Meh no type here. :)";

	for ( int i = 25; i < 28; ++i )
	{
		if(i == 26)
		{
			logging.addOfstream(ofstream("log2.log"));
		}
		logging2.info() << "Logging the second!";
		logging.warning() << "Fib " << i << " start.";
		logging.info() << "processing ...";
		fib(i, logging);
		logging.error() << "Fib" << i << "ended.";
	}

	logging.info(99) << "This Log should not be displayed!";
	
	return 0;
}
