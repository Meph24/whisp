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

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
