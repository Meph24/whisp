#include "User.hpp"

#include <iostream>
#include <string>

#include <GL/glew.h>

#include "EventDefines.h"
#include "IPv4Address.hpp"
#include "SFMLWindow.hpp"
#include "Simulation.hpp"

using std::string;

LocalUser::LocalUser(
						const Cfg& cfg,
						string window_name,
						int			reswidth, 
						int			resheight
						)	
	: InputDeviceConfigurator(cfg)
	, context_settings(24, 8, 0, 3, 3)
	, window( reswidth, resheight, window_name, sf::Style::None, context_settings )
	, mousemode(MouseMode::pointer)
	, event_source( &window )
{	
	window.setActive(true);

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

void LocalUser::operateSimulation(Simulation& simulation)
{
	if(!this->simulation) return;

	this->simulation = &simulation;
	simulation.registerUser(this);
	perspective = simulation.getPerspective(this);

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
	event_mapper->registerMapping(
		EVENT_ID_KEY_F2,
		Act::Toggle(&perspective->enable_third_person)
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
	InputEvent e;
	// are problems with the mousemode interpretation, which requires different components to work in tandem
	// sf::Events are paired in coordinates regarding mouse movement (x,y) is a tuple and not
	// what I actually want to do :
	while (event_source->pollEvent(e))
	{
		if(event_mapper)event_mapper->mapEvent(e);
	}
	//what I must do currently instead:
}

InputDeviceConfigurator::MouseMode LocalUser::mouseMode() const { return mousemode; }
void LocalUser::setMouseMode( InputDeviceConfigurator::MouseMode mode )
{
	//set EventMapping accordingly
	if(event_mapper)
	{
		for( const auto& e : mouse_mode_mappings )
		{
			if(e.first == mode) continue;

			for(const pair<int, EventMapping>& p: e.second)
			{
				event_mapper->clearMappings(p.first);
			}
		}

		for(pair<int, EventMapping>& m : mouse_mode_mappings[mode])
		{
			event_mapper->registerMapping(m.first, m.second);
		}
	}

	//set Window properties accordingly
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
}

vec2 LocalUser::turnSensitivity() const
{
		float modifier = 1.0f;
		if( perspective && perspective->zoomed ) modifier = 1.0f / 8; 
		return turn_sensitivity * modifier; 
}

void LocalUser::draw()
{
	if (! perspective) return;
	perspective->enableAABBDrawing(input_status.draw_aabbs);
	perspective->enableDebugScreen(input_status.debug_screen_active);
	perspective->draw();
}