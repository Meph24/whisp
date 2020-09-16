#include "Operator.hpp"

#include <GL/glew.h>
#include <iostream>

#include <string>

using std::string;

#define MOUSE_WHEEL_OFFSET 2
#define MOUSE_BUTTON_OFFSET 4 
#define JOYSTICK_ID_OFFSET 48
#define JOYSTICK_BUTTON_OFFSET 8


#include "EventDefines.h"

void Operator::operateSimulation(IGameMode* simulation)
{
	SimulationInputStatusSet& input_status = *simulation->input_status;
	event_mapper.reset(new EventMapper(input_status));
	event_handler.reset(new EventHandler(simulation, event_mapper.get()));

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
		Act::Toggle(&input_status.debug_screen_active),
		Cond::keyPressed
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_R,
		Act::AccumulateValue(&input_status.restart)
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_T,
		Act::AccumulateValue(&input_status.benchmark)
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_E,
		Act::AccumulateValue(&input_status.inventory)
	);
	event_mapper->registerMapping( 
		EVENT_ID_KEY_ARROW_UP,
		Act::AccumulateValue(&input_status.selection_up),
		Cond::StatusAsCondition(&input_status.selection_active, true)
	);

	event_mapper->registerMapping( 
		EVENT_ID_KEY_ARROW_DOWN,
		Act::AccumulateValue(&input_status.selection_down),
		Cond::StatusAsCondition(&input_status.selection_active, true)
	);

	event_mapper->registerMapping(
		EVENT_ID_KEY_B,
		Act::Toggle(&input_status.draw_aabbs),
		Cond::keyPressed
	);

	event_mapper->registerMapping(
		EVENT_ID_KEY_Z,
		Act::Toggle(&input_status.slomo), 
		Cond::keyPressed
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_P,
		Act::Toggle(&input_status.pause),
		Cond::keyPressed
	);
	event_mapper->registerMapping(
		EVENT_ID_MOUSE_RMB,
		Act::Toggle(&input_status.zoom),
		Cond::keyPressed
	);
	event_mapper->registerMapping(
		EVENT_ID_MOUSE_WHEEL,
		Act::AccumulateValue(&input_status.weapon_switch)
	);
	event_mapper->registerMapping(
		EVENT_ID_MOUSE_LMB,
		Act::Combinate(&input_status.trigger)
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_C,
		Act::Toggle(&input_status.clip),
		Cond::keyPressed
	);
	event_mapper->registerMapping(
		EVENT_ID_KEY_V,
		Act::Toggle(&input_status.verbose),
		Cond::keyPressed
	);
}

void Operator::disconnectSimulation()
{
	event_handler.reset(nullptr);
}

Operator::Operator(	WallClock&		wallclock,
						string		name, 
						int			reswidth, 
						int			resheight, 
						IMediaHandle::ContextSettings& settings) 
	: wallclock(&wallclock)
	, contextSettings(24, 8, 0, 3, 3)
	, window(sf::VideoMode(reswidth, resheight), name, sf::Style::None, contextSettings)
{
	//important because of the ability to activate the context in another thread
}

void Operator::createWindow(std::string name, int reswidth, int resheight, IMediaHandle::ContextSettings& settings)
{
	contextSettings.depthBits = settings.depth;
	contextSettings.stencilBits = settings.stencil;
	contextSettings.antialiasingLevel = settings.antialiasing;
	contextSettings.majorVersion = settings.openglmajor;
	contextSettings.minorVersion = settings.openglminor;

	
	window.create(sf::VideoMode(reswidth, resheight), name, sf::Style::None, contextSettings);

	//important because of the ability to activate the context in another thread
	window.setActive(false);
}

void Operator::mapSFEventToEventHandlerEvent(sf::Event& e, Buffer<EventHandler::event, 4>& eventBuffer)
{
	if(!event_handler) return;

	switch (e.type)
	{
	case sf::Event::EventType::Closed:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::System, 0, 1));
		break;

	case sf::Event::EventType::Resized:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::System, 1, e.size.width));
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::System, 2, e.size.height));
		break;

	case sf::Event::EventType::LostFocus:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::System, 3, 1));
		break;

	case sf::Event::EventType::GainedFocus:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::System, 3, 0));
		break;

	case sf::Event::EventType::TextEntered:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::System, 4, e.text.unicode));
		break;

	case sf::Event::EventType::MouseEntered:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::System, 5, 1));
		break;

	case sf::Event::EventType::MouseLeft:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::System, 5, 0));
		break;

	case sf::Event::EventType::JoystickDisconnected:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::System, 6 + e.joystickConnect.joystickId, 1));
		break;

	case sf::Event::EventType::JoystickConnected:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::System, 6 + e.joystickConnect.joystickId, 0));
		break;

	case sf::Event::EventType::KeyPressed:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::Keyboard, e.key.code, 1));
		break;

	case sf::Event::EventType::KeyReleased:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::Keyboard, e.key.code, 0));
		break;

	case sf::Event::EventType::MouseMoved:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::Mouse, 0, e.mouseMove.x));
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::Mouse, 1, e.mouseMove.y));
		break;

	case sf::Event::EventType::MouseWheelScrolled:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::Mouse, MOUSE_WHEEL_OFFSET + e.mouseWheelScroll.wheel, e.mouseWheelScroll.delta));
		break;

	case sf::Event::EventType::MouseButtonPressed:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::Mouse, MOUSE_BUTTON_OFFSET + e.key.code, 1));
		break;

	case sf::Event::EventType::MouseButtonReleased:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::Mouse, MOUSE_BUTTON_OFFSET + e.key.code, 0));
		break;

	case sf::Event::EventType::JoystickMoved:
		eventBuffer.write(event_handler->createEvent(EventHandler::Joystick, e.joystickMove.joystickId*JOYSTICK_ID_OFFSET + e.joystickMove.axis, e.joystickMove.position));
		break;

	case sf::Event::EventType::JoystickButtonPressed:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::Joystick, e.joystickButton.joystickId*JOYSTICK_ID_OFFSET + JOYSTICK_BUTTON_OFFSET + e.joystickButton.button, 1));
		break;

	case sf::Event::EventType::JoystickButtonReleased:
		eventBuffer.write(event_handler->createEvent(EventHandler::eventType::Joystick, e.joystickButton.joystickId *JOYSTICK_ID_OFFSET + JOYSTICK_BUTTON_OFFSET + e.joystickButton.button, 0));
		break;
	
	default :
        break;

	}
}

void Operator::pollEvents()
{

	if(!event_handler) return;
	sf::Event e;
	Buffer < EventHandler::event, 4 >  eventBuffer;
	//handle events

	EventHandler::event retEvent;
	while (window.pollEvent(e))
	{
		preHandleEvent(e);
		
		mapSFEventToEventHandlerEvent(e, eventBuffer);
		//handle mapped events
		while (!eventBuffer.nodata())
		{
			if (eventBuffer.read(retEvent))
			{
				event_handler->handle(retEvent);
			}
		}
		eventBuffer.empty();

		postHandleEvent(e);
	}
}

void Operator::preHandleEvent(sf::Event& e)
{

	//switch (e.type)
	//{
	//	/*
	//	case sf::Event::EventType::MouseMoved:
	//	//hardcursorhandle.cursorMoved(e);
	//	break;
    //
	//	case sf::Event::EventType::Resized:
	//	//hardcursorhandle.updateLockPosition();
	//	std::cout << Operator.window.getSize().x << "/" << Operator.window.getSize().y;
	//	break;
    //
	//	case sf::Event::EventType::GainedFocus:
	//	//hardcursorhandle.updateLockPosition();	// no way to check if window was moved
	//	std::cout<< "Stuff !!";
	//	break;
    //
	//	// _test_begin
	//	case sf::Event::EventType::KeyPressed:
	//	if (e.key.code == sf::Keyboard::Key::L) hardcursorhandle.setLocked(true);
	//	if (e.key.code == sf::Keyboard::Key::U) hardcursorhandle.setLocked(false);
	//	if (e.key.code == sf::Keyboard::Key::R) Operator.window.setSize (sf::Vector2u(400, 400));
	//	break;
    //
	//	*/
	//	// _test_end
    //
	//}


}

void Operator::postHandleEvent(sf::Event& e)
{
	switch (e.type)
	{
		case sf::Event::EventType::Closed:
		disconnectSimulation();
		window.close();
		break;

		// for testing
		case sf::Event::EventType::MouseButtonPressed:
		break;

		default: break;//warning suppression

	}
}

void Operator::setContextToMyThread()
{
	window.setActive(true);
}

void Operator::display()
{
	window.display();
}
