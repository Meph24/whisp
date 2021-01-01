#include "User.hpp"

#include <GL/glew.h>
#include <iostream>

#include <string>

#include "EventDefines.h"

#include "SFMLWindow.hpp"
#include "IPv4Address.hpp"

#include "IGameMode.h"

using std::string;

User::User(		
						string		name, 
						int			reswidth, 
						int			resheight
					) 
	: contextSettings(24, 8, 0, 3, 3)
	, name(name)
	, window(nullptr)
{}

LocalUser::LocalUser(
						const Cfg& cfg,
						string		name, 
						int			reswidth, 
						int			resheight
						)	
	: User( name, reswidth, resheight)
	, InputDeviceConfigurator(cfg)
	, mousemode(MouseMode::pointer)
	, sfmlwindow( reswidth, resheight, name, sf::Style::None, contextSettings )
	, event_source( &sfmlwindow )
{	
	window = &sfmlwindow;
	window->setActive(true);

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
}

InputDeviceConfigurator::MouseMode LocalUser::mouseMode() const { return mousemode; }

void LocalUser::setMouseMode( MouseMode mode )
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

vec2 LocalUser::turnSensitivity() const
{
		float modifier = 1.0f;
		if( perspective && perspective->zoomed ) modifier = 1.0f / 8; 
		return turn_sensitivity * modifier; 
}

void LocalUser::operateSimulation(IGameMode* simulation)
{
	if(!simulation) return;

	this->simulation = simulation;
	simulation->registerUser(this);
	perspective = simulation->getPerspective(this);

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
		Act::Toggle(&perspective->zoomed)
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

void LocalUser::disconnectSimulation()
{
	if(!perspective) return;

	perspective->simulation->kickUser(this);
	perspective.reset();
	event_mapper.reset();
}

void LocalUser::pollEvents()
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

void LocalUser::preHandleEvent(sf::Event& e){}
void LocalUser::postHandleEvent(sf::Event& e)
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

void User::setContextToMyThread(){ window->setActive(true); }
void User::draw()
{
	perspective->enableAABBDrawing(input_status.draw_aabbs);
	perspective->enableDebugScreen(input_status.debug_screen_active);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	perspective->draw();
}

void User::display()
{
	window->display();
}
