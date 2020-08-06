#include "../src/event.hpp"

#include <gtest/gtest.h>

#include "../src/WallClock.hpp"

TEST(test_InputEvent_Spec, properties)
{
	using namespace InputEventSpec;
	EXPECT_EQ(1024, event_ids_per_type);
	EXPECT_EQ(0,mouse_movement_offset);
	EXPECT_EQ(event_ids_per_type / 4, mouse_wheel_offset);
	EXPECT_EQ(event_ids_per_type / 2, mouse_button_offset);
	EXPECT_EQ(16, joystick_number_supported);
	EXPECT_EQ(event_ids_per_type / joystick_number_supported, joystick_id_offset);
	EXPECT_EQ(0, joystick_movement_offset);
	EXPECT_EQ(16, joystick_button_offset);



	//some further properties from the top of my head
	EXPECT_LT(mouse_movement_offset, event_ids_per_type);
	EXPECT_LT(mouse_wheel_offset, event_ids_per_type);
	EXPECT_LT(mouse_button_offset, event_ids_per_type);
	EXPECT_LT(joystick_movement_offset, joystick_id_offset);
	EXPECT_LT(joystick_button_offset, joystick_id_offset);
}

TEST(test_InputEvent_Spec, eventID_calculation_helper)
{
	using namespace InputEventSpec;

	EXPECT_EQ(0, 	eventId(EventType::Window, 0));
	EXPECT_EQ(10, 	eventId(EventType::Window, 10));
	
	//offsets higher than the allocated portion of a type are invalid (aka <0)
	EXPECT_LT(eventId(EventType::Window, event_ids_per_type), 0);

	//types are shifted in order by event_ids_per_type
	EXPECT_EQ(event_ids_per_type + 10, eventId(EventType::Keyboard, 10));
}

/*

TEST(Test_SFMLInputEventSource_mapSFEventToInputEvent, TimePointEquality)
{
	vector<InputEvent> ret;

	sf::Event sfe;

	sfe.type = sf::Event::EventType::Closed;
	ret = SFMLInputEventSource::mapSFEventToInputEvents(sfe, WallClock::time_point(1s));


	ASSERT_EQ(1, ret.size());
	EXPECT_EQ(WallClock::time_point(1s), ret[0].time );

}

TEST(Test_SFMLInputEventSource_mapSFEventToInputEvent, Closed)
{
	vector<InputEvent> ret;

	sf::Event sfe;

	sfe.type = sf::Event::EventType::Closed;
	ret = SFMLInputEventSource::mapSFEventToInputEvents(sfe, WallClock::time_point());

	ASSERT_EQ(1, ret.size());
	EXPECT_EQ(0, ret[0].id);
}


TEST(Test_SFMLInputEventSource_mapSFEventToInputEvent, Resized)
{
	vector<InputEvent> ret;

	sf::Event sfe;

	sfe.type = sf::Event::EventType::Resized;
	sfe.size.width = 3.0f;
	sfe.size.height = 4.0f;
	ret = SFMLInputEventSource::mapSFEventToInputEvents(sfe, WallClock::time_point());

	EXPECT_EQ(2, ret.size());

	EXPECT_EQ(1, ret[0].id);
	EXPECT_EQ(3.0f, ret[0].value);
	EXPECT_EQ(2, ret[1].id);
	EXPECT_EQ(4.0f, ret[1].value);
}

TEST(Test_SFMLInputEventSource_mapSFEventToInputEvent, Keyboard)
{
	vector<InputEvent> ret;

	sf::Event sfe;

	sfe.type = sf::Event::EventType::KeyPressed;
	sfe.key.code = sf::Keyboard::Key::A;
	ret = SFMLInputEventSource::mapSFEventToInputEvents(sfe, WallClock::time_point());

	EXPECT_EQ(1, ret.size());

	EXPECT_EQ(InputEventSpec::eventId(InputEventSpec::EventType::Keyboard, sf::Keyboard::Key::A), ret[0].id);
	EXPECT_EQ(1.0f, ret[0].value);




	sfe.type = sf::Event::EventType::KeyReleased;
	sfe.key.code = sf::Keyboard::Key::A;
	ret = SFMLInputEventSource::mapSFEventToInputEvents(sfe, WallClock::time_point());

	EXPECT_EQ(1, ret.size());

	EXPECT_EQ(InputEventSpec::eventId(InputEventSpec::EventType::Keyboard, sf::Keyboard::Key::A), ret[0].id);
	EXPECT_EQ(0.0f, ret[0].value);

}


TEST(Test_SFMLInputEventSource_mapSFEventToInputEvent, Mouse)
{

	vector<InputEvent> ret;

	sf::Event sfe;

	sfe.type = sf::Event::EventType::MouseButtonPressed;
	sfe.mouseButton.button = sf::Mouse::Button::Left;
	sfe.mouseButton.x = 12;
	sfe.mouseButton.y = 14;
	ret = SFMLInputEventSource::mapSFEventToInputEvents(sfe, WallClock::time_point());

	ASSERT_EQ(3, ret.size());

	InputEvent expected0 (
			InputEventSpec::eventId(InputEventSpec::EventType::Mouse, InputEventSpec::mouse_movement_offset + 0), 12.0f, WallClock::time_point()
			);

	EXPECT_EQ(expected0.id, ret[0].id);
	EXPECT_EQ(expected0.value, ret[0].value);

	InputEvent expected1 (
			InputEventSpec::eventId(InputEventSpec::EventType::Mouse, InputEventSpec::mouse_movement_offset + 1), 14.0f, WallClock::time_point()
			);

	EXPECT_EQ(expected1.id, ret[1].id);
	EXPECT_EQ(expected1.value, ret[1].value);

	InputEvent expected2 (
			InputEventSpec::eventId(InputEventSpec::EventType::Mouse, InputEventSpec::mouse_button_offset + sf::Mouse::Button::Left), 1.0f, WallClock::time_point()
			);

	EXPECT_EQ(expected2.id, ret[2].id);
	EXPECT_EQ(expected2.value, ret[2].value);

}

//these mapping tests are getting stupid
*/

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
