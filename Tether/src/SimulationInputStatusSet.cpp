#include "SimulationInputStatusSet.hpp"

sf::Packet& operator<< (sf::Packet& p, const SimulationInputStatusSet& s)
{
	return
	p 	<< s.timestamp << s.debug_screen_active << s.restart << s.inventory << s.selection_up
		<< s.selection_down << s.draw_aabbs << s.benchmark << s.slomo << s.pause << s.menu
		<< s.weapon_selection << s.clip << s. walk << s.turn << s.trigger << s.selection_active;
}


sf::Packet& operator>> (sf::Packet& p, SimulationInputStatusSet& s)
{
	return
	p 	>> s.timestamp >> s.debug_screen_active >> s.restart >> s.inventory >> s.selection_up
		>> s.selection_down >> s.draw_aabbs >> s.benchmark >> s.slomo >> s.pause >> s.menu
		>> s.weapon_selection >> s.clip >> s. walk >> s.turn >> s.trigger >> s.selection_active;
}

