#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui-sfml/imgui-SFML.h"
#include <iostream>

int main()
{
    const int wWidth = 1280;
    const int wHeight = 720;
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML Works");
    
    // window.setVerticalSyncEnabled(true); // call it once, after creating the window
    sf:: CircleShape circle(50);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(300.0f, 300.0f);
    float circleMoveSpeed = 0.5f;


    sf::Font myFont;
    if (!myFont.loadFromFile("../fonts/Roboto-Black.ttf")) 
    {
        std::cerr << "Error: Failed to load font from 'fonts/Roboto-Black.ttf'" << std::endl;
        exit(-1);
    }
    window.setFramerateLimit(60); // call it once, after creating the window

    sf::Text text("Sample text", myFont, 24);

    text.setPosition(0, wHeight - (float)text.getCharacterSize());


    while(window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
            {
                window.close();
            }
            if(e.type == sf::Event::KeyPressed)
            {
                std::cout<<"Key pressed with code = "<<e.key.code << std::endl;

                if(e.key.code == sf::Keyboard::X)
                {
                    std::cout<<"Direction changed\n";
                    circleMoveSpeed *= -1.0f;
                }
            }
        }
        circle.setPosition(circle.getPosition().x - circleMoveSpeed, circle.getPosition().y - circleMoveSpeed);

        window.clear();
        window.draw(circle);
        window.draw(text);
        window.display();
    }
    return 0;
}