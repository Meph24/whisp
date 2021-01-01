#pragma once

#include <memory>
#include <stdexcept>
#include <string>

#include "commandline.hpp"
#include "GraphicsRessources.hpp"
#include "SFMLClock.hpp"
#include "User.hpp"
#include "WallClock.hpp"

using std::unique_ptr;

struct App
{
	virtual void run() = 0;
};

struct DefaultApp : public App
{
	WallClock& wallclock;
	Cfg& cfg;

	unique_ptr<Simulation> sim;
	LocalUser op;

	DefaultApp( WallClock& wallclock, Cfg& cfg );
	void run();
};

class Main
{
	static unordered_map<string, unsigned int> command_line_argument_flags;
	CommandLineArgumentInterpreter commandline_argument_interpreter;

	SFMLClock base_clock;
	WallClock clock;
	Cfg cfg;

	unique_ptr<App> app;
	
public:
	Main(int argc, char** argv);
	void run();
};
