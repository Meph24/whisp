#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/System/Clock.hpp>

struct LocalUser;
struct InputEvent;

namespace gui {

struct ImGuiController
{
    //gui controlled members
    LocalUser& lu;
	sf::Clock last_imgui_update_clock; //measuring time since the last imgui update

    ImGuiController(LocalUser& lu);
    ~ImGuiController();

    //surrounding control function blocks
    bool processEvent( const InputEvent& ) const;
    void init();
    void newFrame();
    void render();
};

struct MainMenu
{
	bool showimguidemo = false;

    void draw();
};

} // namespace gui

#endif // GUI_HPP
