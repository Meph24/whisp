#include "gui.hpp"

#include <chrono>
#include <SFML/Window/Mouse.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Simulation.hpp"

namespace gui {

ImGuiController::ImGuiController(LocalUser& lu) : lu(lu)
{
    ImGui::SFML::Init(lu.window.w, static_cast<sf::Vector2f>(lu.window.w.getSize()));
    ImGui_ImplOpenGL3_Init();//const char* glslversion?
}
ImGuiController::~ImGuiController() { ImGui::SFML::Shutdown(); }

bool ImGuiController::processEvent(const InputEvent& e) const { return ImGui::SFML::ProcessEvent(e); }

void ImGuiController::newFrame()
{
	if(lu.mousemode == InputDeviceConfigurator::MouseMode::pointer) 
        ImGui::SFML::Update(sf::Mouse::getPosition(lu.window.w),static_cast<sf::Vector2f>(lu.window.w.getSize()), last_imgui_update_clock.restart());

	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
}
void ImGuiController::render()
{
	ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MainMenu::draw()
{
    ImGui::Begin("MainMenu", nullptr, ImGuiWindowFlags_MenuBar);
    if(ImGui::BeginMenuBar())
    {
        if(ImGui::BeginMenu("View"))
        {
            ImGui::Checkbox("ImGui Demo", &showimguidemo);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();
    if(showimguidemo) ImGui::ShowDemoWindow();
}

} // namespace gui