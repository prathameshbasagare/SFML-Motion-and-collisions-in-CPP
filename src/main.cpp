#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui-sfml/imgui-SFML.h"
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML + ImGui Demo");
    window.setFramerateLimit(60);
    
    // Initialize ImGui with SFML
    if (!ImGui::SFML::Init(window)) {
        std::cerr << "Failed to initialize ImGui with SFML!" << std::endl;
        return -1;
    }
    
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(350, 250);
    
    // ImGui state variables
    bool show_demo_window = true;
    bool show_another_window = false;
    sf::Color bg_color = sf::Color(114, 144, 154);
    float f = 0.0f;
    int counter = 0;
    
    sf::Clock deltaClock;
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        // Calculate delta time properly
        sf::Time deltaTime = deltaClock.restart();
        float dt = deltaTime.asSeconds();
        
        // Ensure minimum delta time to prevent ImGui errors
        if (dt <= 0.0f) {
            dt = 1.0f / 60.0f; // Assume 60 FPS if delta time is invalid
        }
        
        // Update ImGui
        ImGui::SFML::Update(window, sf::seconds(dt));
        
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;
            
            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
            
            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);
            
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&bg_color); // Edit 3 floats representing a color
            
            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
            
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        
        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
        
        // Set the background color
        window.clear(bg_color);
        
        // Draw SFML content
        window.draw(shape);
        
        // Render ImGui
        ImGui::SFML::Render(window);
        
        window.display();
    }
    
    // Cleanup
    ImGui::SFML::Shutdown();
    
    return 0;
}