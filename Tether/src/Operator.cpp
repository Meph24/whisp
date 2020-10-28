#include "Operator.hpp"

#include <GL/glew.h>
#include <iostream>

#include <string>

using std::string;

#include "EventDefines.h"

#include "SFMLWindow.hpp"

Operator::Operator(		
						string		name, 
						int			reswidth, 
						int			resheight
					) 
	: contextSettings(24, 8, 0, 3, 3)
	, window(nullptr)
{}

LocalOperator::LocalOperator(
						const Cfg& cfg,
						string		name, 
						int			reswidth, 
						int			resheight
						)	
	: Operator( name, reswidth, resheight)
	, InputDeviceConfigurator(cfg)
	, mousemode(MouseMode::pointer)
	, sfmlwindow( reswidth, resheight, name, sf::Style::None, contextSettings )
	, event_source( &sfmlwindow )
{	
	window = &sfmlwindow;
}

InputDeviceConfigurator::MouseMode LocalOperator::mouseMode() const { return mousemode; }

void LocalOperator::setMouseMode( MouseMode mode )
{
	if(mousemode == mode) return;

	if(event_mapper)
	//remove all other mouse related mappings
	{
		for( const auto& e : mouse_mode_mappings )
		{
			if(e.first == mode) continue;

			for(const pair<int, EventMapping>& p: e.second)
			{
				event_mapper->clearMappings(p.first);
			}
		}

		//add this modes mappings
		for(pair<int, EventMapping>& m : mouse_mode_mappings[mode])
		{
			event_mapper->registerMapping(m.first, m.second);
		}
	}

	switch( mode )
	{ 
		case MouseMode::diff:
			window->setMouseCursorVisible(false); 
		break;
		default:
		case MouseMode::pointer:
			window->setMouseCursorVisible(true);
		break;
	}

	mousemode = mode;
}

vec2 LocalOperator::turnSensitivity() const
{
		float modifier = 1.0f;
		if( event_mapper && event_mapper->managed_stati->zoom ) modifier = 1.0f / 8; 
		return turn_sensitivity * modifier; 
}

void LocalOperator::operateSimulation(IGameMode* simulation)
{
	SimulationInputStatusSet& input_status = *simulation->input_status;
	event_mapper.reset(new EventMapper(input_status));

	namespace Act = eventmapping::actions;
	namespace Cond = eventmapping::conditions;

	event_mapper->registerMapping(
		EVENT_ID_KEY_W, 
		Act::Combinate(&input_status.walk.z, 1.0f)
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_S, 
		Act::Combinate(&input_status.walk.z, -1.0f)
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_A, 
		Act::Combinate(&input_status.walk.x, -1.0f)
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_D, 
		Act::Combinate(&input_status.walk.x, 1.0f)
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_SHIFT,
		Act::Combinate(&input_status.walk.y, 1.0)
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_CTRL,
		Act::Combinate(&input_status.walk.y, -1.0)
	);

	event_mapper->registerMapping(
		EVENT_ID_KEY_F3,
		Act::Toggle(&input_status.debug_screen_active)
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_R,
		Act::SendSignal(&input_status.restart)
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_T,
		Act::SendSignal(&input_status.benchmark),
		Cond::keyPressed
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_E,
		Act::SendSignal(&input_status.inventory),
		Cond::keyPressed
	);
	event_mapper->registerMapping( 
		EVENT_ID_KEY_ARROW_UP,
		Act::SendSignal(&input_status.selection_up),
		Cond::statusAsCondition(&input_status.selection_active, true) && Cond::keyPressed
	);

	event_mapper->registerMapping( 
		EVENT_ID_KEY_ARROW_DOWN,
		Act::SendSignal(&input_status.selection_down),
		Cond::StatusAsCondition(&input_status.selection_active, true) && Cond::keyPressed
	);

	event_mapper->registerMapping(
		EVENT_ID_KEY_B,
		Act::Toggle(&input_status.draw_aabbs)
	);

	event_mapper->registerMapping(
		EVENT_ID_KEY_Z,
		Act::Toggle(&input_status.slomo)
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_P,
		Act::Toggle(&input_status.pause)
	);
	event_mapper->registerMapping(
		EVENT_ID_MOUSE_RMB,
		Act::Toggle(&input_status.zoom)
	);
	event_mapper->registerMapping(
		EVENT_ID_MOUSE_WHEEL,
		Act::AccumulateValue<Index<8>>(&input_status.weapon_selection)
	);
	event_mapper->registerMapping(
		EVENT_ID_MOUSE_LMB,
		Act::Toggle(&input_status.trigger, Act::KeyTrigger::on_key_press_or_release)
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_C,
		Act::Toggle(&input_status.clip)
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_V,
		Act::Toggle(&input_status.verbose),
		Cond::keyPressed
	);

	auto& container = mouse_mode_mappings[ InputDeviceConfigurator::MouseMode::diff ];
	container.emplace_back(
		EVENT_ID_MOUSE_X,
		EventMapping(Act::MouseDiffInput( *window, *this, &input_status.turn.y, Act::MouseDiffInput::Axis::X ), Cond::alwaysTrue)
	);
	container.emplace_back(
		EVENT_ID_MOUSE_Y,
		EventMapping(Act::MouseDiffInput( *window, *this, &input_status.turn.x, Act::MouseDiffInput::Axis::Y ), Cond::alwaysTrue)
	);

	event_mapper->registerMapping(
		EVENT_ID_KEY_ESCAPE,
		Act::ToggleMouseMode( *this )
	);

	setMouseMode( MouseMode::diff );
}

void LocalOperator::disconnectSimulation()
{
	event_mapper.reset();
}

void LocalOperator::pollEvents()
{
	sf::Event e;
	Buffer < EventHandler::event, 4 >  eventBuffer;
	//handle events

	EventHandler::event retEvent;
	while (event_source->pollEvent(e))
	{
		preHandleEvent(e);
		
		event_source->mapSFEventToEventHandlerEvents(e, eventBuffer);
		//handle mapped events
		while (!eventBuffer.nodata())
		{
			if (eventBuffer.read(retEvent))
			{
				if(event_handler.Filter.filter(retEvent))
				{
					event_handler.Filter.update(retEvent);
					if(event_mapper) event_mapper->event(retEvent);
				}
			}
		}
		eventBuffer.clear();

		postHandleEvent(e);
	}
}

void LocalOperator::preHandleEvent(sf::Event& e){}
void LocalOperator::postHandleEvent(sf::Event& e)
{
	switch (e.type)
	{
		case sf::Event::EventType::Closed:
			disconnectSimulation();
			window->close();
		break;

		case sf::Event::MouseMoved:
			if(mouseMode() == MouseMode::diff) 
			{
				sf::Vector2i to_set ( (window->pos()) + ((sf::Vector2i)window->size())/2 );
				if( sf::Mouse::getPosition() == to_set ) break;
				sf::Mouse::setPosition( to_set );
			}
		break;
		default: break;//warning suppression
	}
}

void Operator::setContextToMyThread(){ window->setActive(true); }
void Operator::display(){ window->display(); }
void Operator::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

RemoteControlReceiverOperator::RemoteControlReceiverOperator(
	 std::string name, int reswidth, int resheight, Port try_port
	)
	: Operator (name + string(" - remote controlled receiver"), reswidth, resheight)
	, managed_status(nullptr)
	, sfmlwindow( reswidth, resheight, name, sf::Style::None, contextSettings )
	, received(0)
{
	window = &sfmlwindow;
	udpsocket.setBlocking(false);
	
	udpsocket.bind(try_port);
	std::cout << "RemoteControlReceiverOperator : Bound at port <" << udpsocket.getLocalPort() << ">\n";
}

vec2 RemoteControlSender::turnSensitivity() const
{
		float modifier = 1.0f;
		if( event_mapper.managed_stati->zoom ) modifier = 1.0f / 8; 
		return turn_sensitivity * modifier; 
}

#include <iostream>

void RemoteControlSender::tunein(IPv4Address& addr, Port port)
{
	std::cout << "Tuning in to " << (string) addr << ':' << port << '\n';
	receiver_addr = addr; receiver_port = port;
}

void RemoteControlSender::operateRemote()
{
	namespace Act = eventmapping::actions;
	namespace Cond = eventmapping::conditions;

	SimulationInputStatusSet& input_status = status_set;

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
		Act::SendSignal(&input_status.restart)
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
		EVENT_ID_MOUSE_RMB,
		Act::Toggle(&input_status.zoom)
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
	event_mapper.registerMapping(
		EVENT_ID_KEY_V,
		Act::Toggle(&input_status.verbose),
		Cond::keyPressed
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
	status_set.timestamp = ts.time_since_epoch().count(); 
	udpsocket.send( &status_set, sizeof(SimulationInputStatusSet), sf::IpAddress((uint32_t)receiver_addr), receiver_port );
	last_synced = ts;
}

void RemoteControlSender::processEvents()
{
	sf::Event e;
	Buffer < EventHandler::event, 4 >  eventBuffer;
	//handle events

	EventHandler::event retEvent;
	while (event_source->pollEvent(e))
	{
		event_source->mapSFEventToEventHandlerEvents(e, eventBuffer);
		//handle mapped events
		while (!eventBuffer.nodata())
		{
			if (eventBuffer.read(retEvent))
			{
				if(event_handler.Filter.filter(retEvent))
				{
					event_handler.Filter.update(retEvent);
				 	event_mapper.event(retEvent);
				}
			}
		}
		eventBuffer.clear();

		switch (e.type)
		{
			case sf::Event::EventType::Closed:
				window.close();
			break;

			case sf::Event::MouseMoved:
				if(mouseMode() == MouseMode::diff) 
					sf::Mouse::setPosition( (window.pos()) + ((sf::Vector2i)window.size())/2 );
			break;

			default: break;//warning suppression
		}
	}
}

RemoteControlSender::RemoteControlSender( WallClock& wallclock, Cfg& cfg )
	: InputDeviceConfigurator(cfg)
	, clock(wallclock)
	, window( 200, 200, string("remote control device") , sf::Style::None, sf::ContextSettings())
	, event_mapper( status_set )
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
		break;
		default:
		case MouseMode::pointer:
			window.setMouseCursorVisible(true);
		break;
	}

	mousemode = mode;
}
