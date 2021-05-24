#include <GL/glew.h>
#include <SFML/Window.hpp>

#include "../src/imgui/imgui.h"
#include "../src/imgui/imgui-SFML.h"
#include "../src/imgui/imgui_impl_opengl3.h"

#include <iostream>

bool setUpGlew()
{
    GLenum error = glewInit();
    if(error != GLEW_OK)
    {
        std::cerr << "Error: " << glewGetErrorString(error) << std::endl;
        return false;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << "\n";
    return true;
}

int main()
{    
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    sf::Window window(sf::VideoMode(800,600),"IMGUI?", 7U, settings);

    if(!setUpGlew())
        return -1;

    ImGui::SFML::Init(window, static_cast<sf::Vector2f>(window.getSize()));
    ImGui_ImplOpenGL3_Init();//const char* glslversion?

    glClearColor(1,0,1,1);

    sf::Event event;
    bool running = true;
    sf::Clock clock;
    while (running)
    {
        ImGui::SFML::Update(sf::Mouse::getPosition(window),static_cast<sf::Vector2f>(window.getSize()),clock.restart());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.display();

        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                running = false;
            ImGui::SFML::ProcessEvent(event);
        }
    }

    ImGui::SFML::Shutdown();
    window.close();
    
    return 0;
}