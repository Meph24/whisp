#include "remoteControl.hpp"

#include "Zombie_World.h"
#include "Simulation_World.h"

#include "EventDefines.h"

#include <iostream>

using std::cout;

void RemoteControlReceiverUser::operateSimulation(Simulation* simulation) 
{
	if(!simulation) return;

	this->simulation = simulation;
	simulation->makeAvatarFor(this);
	if(!avatar()) { std::cerr << "User could not be registered in Simulation!" << std::endl; }
}

void RemoteControlReceiverUser::disconnectSimulation()
{
	if(!this->simulation) return;

	perspective.reset();
	simulation->disconnectAvatarFrom(this);
	simulation = nullptr;
}

void RemoteControlReceiverUser::processNetworkInput() 
{ 
    sf::IpAddress addr; Port port;
    size_t this_received = 1;
    char * data = (char*)&current_receive + received;
    while(this_received)
    {
        this_received = 0;
        udpsocket.receive(data, sizeof(SimulationInputStatusSet) - received, this_received, addr, port);
        if(!this_received) break;
        received += this_received;
        if(received == sizeof(SimulationInputStatusSet))
        {
            if(input_status.timestamp < current_receive.timestamp)
            {
                input_status = current_receive;
            }
            received = 0;
        }
    }
}

RemoteControlReceiverUser::RemoteControlReceiverUser( Cfg& cfg,
	std::string window_name, int reswidth, int resheight, Port try_port )
	: LocalUser(cfg, window_name, reswidth, resheight)
	, received(0)
{
	udpsocket.setBlocking(false);
	
	udpsocket.bind(try_port);
	std::cout << "RemoteControlReceiverUser : Bound at port <" << udpsocket.getLocalPort() << ">\n";
}

RemoteControlReceiverApp::RemoteControlReceiverApp(WallClock& wallclock, Cfg& cfg, Port port)
	: wallclock(wallclock)
	, cfg(cfg)
	, user( cfg, *cfg.getStr("general","application_name") + " - remote controlled",  *cfg.getInt("graphics", "resolutionX"), *cfg.getInt("graphics", "resolutionY"), port )
{

	int zombie_mode = *cfg.getInt("test", "zombie_mode");
	if(zombie_mode) 
	{
		sim = std::make_unique<Zombie_World>(wallclock, cfg);
		user.input_status.clip = true;
	}
	else 
	{
		sim = std::make_unique<Simulation_World>(wallclock, cfg);
	}
	user.operateSimulation( sim.get() );
}

void RemoteControlReceiverApp::run()
{
	cout << "Running RemoteControlReceiver Application !\n";

	if(!sim) 
	{
		throw std::runtime_error( "App: Failure in application initialization < No Simulation to simulate >!" );
	}

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "GLEW failed to initialize !" << std::endl;
	}

	glClearDepth(1.0f);
	glClearColor(0.0f, 0.0f, 0.25f, 0.0f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glAlphaFunc(GL_GREATER, 0.02f);
	glEnable(GL_ALPHA_TEST);

	sim->init();
	srand(time(0));

	while (user.window.isOpen())
	{
		//render
		sim->step();



		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		user.draw();
		user.window.display();

		//handle events
		user.processNetworkInput();

		InputEvent e;
		while(user.window.pollEvent(e))	// is currently not used here, but must be polled to keep the window open
									// the window will handle essential events concerning it by itself
		{}; 		
	}
	user.disconnectSimulation();
}

RemoteControlSenderApp::RemoteControlSenderApp(WallClock& wallclock, Cfg& cfg, IPv4Address& addr, Port port)
		: rc_sender(wallclock, cfg)
{
	cout << "Initializing RemoteControlSender Application !\n";
	rc_sender.window.setPos(sf::Vector2i(0,0));
	rc_sender.tunein(addr, port);
	rc_sender.operateRemote();
	rc_sender.setMouseMode( InputDeviceConfigurator::MouseMode::diff );
}

void RemoteControlSenderApp::run()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "GLEW failed to initialize !" << std::endl;
	}

	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
	while(rc_sender.window.isOpen())
	{
		glClear( GL_COLOR_BUFFER_BIT );
		rc_sender.window.display();
		rc_sender.processEvents();
		rc_sender.sync();
	}
}



void RemoteControlSender::tunein(IPv4Address& addr, Port port)
{
	std::cout << "Tuning in to " << (string) addr << ':' << port << '\n';
	receiver_addr = addr; receiver_port = port;
}

void RemoteControlSender::operateRemote()
{
	namespace Act = eventmapping::actions;
	namespace Cond = eventmapping::conditions;

	event_mapper.registerMapping(
		EVENT_ID_KEY_W, 
		Act::Combinate(&input_status.walk.z, 1.0f)
	);
	event_mapper.registerMapping(
		EVENT_ID_KEY_S, 
		Act::Combinate(&input_status.walk.z, -1.0f)
	);
	event_mapper.registerMapping(
		EVENT_ID_KEY_A, 
		Act::Combinate(&input_status.walk.x, -1.0f)
	);
	event_mapper.registerMapping(
		EVENT_ID_KEY_D, 
		Act::Combinate(&input_status.walk.x, 1.0f)
	);
	event_mapper.registerMapping(
		EVENT_ID_KEY_SHIFT,
		Act::Combinate(&input_status.walk.y, 1.0)
	);
	event_mapper.registerMapping(
		EVENT_ID_KEY_CTRL,
		Act::Combinate(&input_status.walk.y, -1.0)
	);
	event_mapper.registerMapping(
		EVENT_ID_KEY_F3,
		Act::Toggle(&input_status.debug_screen_active)
	);
	event_mapper.registerMapping(
		EVENT_ID_KEY_R,
		Act::SendSignal(&input_status.restart),
		Cond::keyPressed
	);
	event_mapper.registerMapping(
		EVENT_ID_KEY_T,
		Act::SendSignal(&input_status.benchmark),
		Cond::keyPressed
	);
	event_mapper.registerMapping(
		EVENT_ID_KEY_E,
		Act::SendSignal(&input_status.inventory),
		Cond::keyPressed
	);
	event_mapper.registerMapping( 
		EVENT_ID_KEY_ARROW_UP,
		Act::SendSignal(&input_status.selection_up),
		Cond::statusAsCondition(&input_status.selection_active, true) && Cond::keyPressed
	);

	event_mapper.registerMapping( 
		EVENT_ID_KEY_ARROW_DOWN,
		Act::SendSignal(&input_status.selection_down),
		Cond::StatusAsCondition(&input_status.selection_active, true) && Cond::keyPressed
	);

	event_mapper.registerMapping(
		EVENT_ID_KEY_B,
		Act::Toggle(&input_status.draw_aabbs)
	);

	event_mapper.registerMapping(
		EVENT_ID_KEY_Z,
		Act::Toggle(&input_status.slomo)
	);
	event_mapper.registerMapping(
		EVENT_ID_KEY_P,
		Act::Toggle(&input_status.pause)
	);

	event_mapper.registerMapping(
		EVENT_ID_MOUSE_WHEEL,
		Act::AccumulateValue<Index<8>>(&input_status.weapon_selection)
	);
	event_mapper.registerMapping(
		EVENT_ID_MOUSE_LMB,
		Act::Toggle(&input_status.trigger, Act::KeyTrigger::on_key_press_or_release)
	);
	event_mapper.registerMapping(
		EVENT_ID_KEY_C,
		Act::Toggle(&input_status.clip)
	);

	auto& container = mouse_mode_mappings[ InputDeviceConfigurator::MouseMode::diff ];
	container.emplace_back(
		EVENT_ID_MOUSE_X,
		EventMapping(Act::MouseDiffInput( window, *this, &input_status.turn.y, Act::MouseDiffInput::Axis::X ), Cond::alwaysTrue)
	);
	container.emplace_back(
		EVENT_ID_MOUSE_Y,
		EventMapping(Act::MouseDiffInput( window, *this, &input_status.turn.x, Act::MouseDiffInput::Axis::Y ), Cond::alwaysTrue)
	);

	event_mapper.registerMapping(
		EVENT_ID_KEY_ESCAPE,
		Act::ToggleMouseMode( *this )
	);
} 
void RemoteControlSender::sync()
{ 
	auto ts = clock.now();
	if(ts - last_synced < 20ms) return;
	input_status.timestamp = ts.time_since_epoch().count(); 
	udpsocket.send( &input_status, sizeof(SimulationInputStatusSet), sf::IpAddress((uint32_t)receiver_addr), receiver_port );
	last_synced = ts;
}

void RemoteControlSender::processEvents()
{
	InputEvent e;
	while(window.pollEvent(e))	// is currently not used here, but must be polled to keep the window open
								// the window will handle essential events concerning it by itself
	{
		event_mapper.mapEvent(e);
	}; 		
}

RemoteControlSender::RemoteControlSender( WallClock& wallclock, Cfg& cfg )
	: InputDeviceConfigurator(cfg)
	, clock(wallclock)
	, window( 200, 200, string("remote control device") , sf::Style::None, sf::ContextSettings())
	, event_mapper( input_status )
	, event_source( &window )
	, mousemode(MouseMode::pointer)
	, last_synced(clock.now())
{
	udpsocket.setBlocking(true);	
}

InputDeviceConfigurator::MouseMode RemoteControlSender::mouseMode() const { return mousemode; }
void RemoteControlSender::setMouseMode( MouseMode mode )
{
	if(mousemode == mode) return;

	for( const auto& e : mouse_mode_mappings )
	{
		if(e.first == mode) continue;

		for(const pair<int, EventMapping>& p: e.second)
		{
			event_mapper.clearMappings(p.first);
		}
	}

	//add this modes mappings
	for(pair<int, EventMapping>& m : mouse_mode_mappings[mode])
	{
		event_mapper.registerMapping(m.first, m.second);
	}

	switch( mode )
	{ 
		case MouseMode::diff:
			window.setMouseCursorVisible(false); 
			window.lockMouse(true);
		break;
		default:
		case MouseMode::pointer:
			window.setMouseCursorVisible(true);
			window.lockMouse(false);
		break;
	}

	mousemode = mode;
}
