#include "SimulationInputStatusSet.hpp"

void SimulationInputStatusSet::toggleCondition(bool b) { b = (b) ? false : true; }
void SimulationInputStatusSet::setConditionTrue(bool b) { b = true; }
void SimulationInputStatusSet::setConditionFalse(bool b) { b = false; }

sf::Packet& operator<< (sf::Packet& p, const SimulationInputStatusSet& s)
{
	return
	p 	<< s.timestamp << s.debug_screen_active << s.restart << s.inventory << s.selection_up
		<< s.selection_down << s.draw_aabbs << s.benchmark << s.slomo << s.pause << s.menu
		<< s.weapon_selection << s.clip << s. walk << s.turn << s.trigger;
}


sf::Packet& operator>> (sf::Packet& p, SimulationInputStatusSet& s)
{
	return
	p 	>> s.timestamp >> s.debug_screen_active >> s.restart >> s.inventory >> s.selection_up
		>> s.selection_down >> s.draw_aabbs >> s.benchmark >> s.slomo >> s.pause >> s.menu
		>> s.weapon_selection >> s.clip >> s. walk >> s.turn >> s.trigger;
}

