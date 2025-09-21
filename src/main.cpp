#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui-sfml/imgui-SFML.h"
#include <iostream>
#include<vector>
#include<memory>
#include<fstream>
#include<sstream>
int main()
{
    int wWidth = 1280;
    int wHeight = 720;
    // Load shapes from config.txt
    std::ifstream configFile("../config.txt");
    std::string line;
    
    std::vector<std::shared_ptr<sf::Shape>> shapes;
    std::vector<sf::Vector2f> velocities;
    std::string font_path;
    float font_size, font_r, font_g, font_b;

    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if(type == "Window")
        {
            iss>> wWidth >> wHeight;
        }
        if(type == "Font")
        {
            iss>>font_path>>font_size>>font_r>>font_g>>font_b;
        }
        if (type == "Circle") {
            std::string name;
            float x, y, vx, vy, r, g, b, R;
            iss >> name >> x >> y >> vx >> vy >> r >> g >> b >> R;
            
            auto circle = std::make_shared<sf::CircleShape>(R);
            circle->setFillColor(sf::Color(r, g, b));
            circle->setPosition(x, y);
            shapes.push_back(circle);
            velocities.push_back(sf::Vector2f(vx, vy));
        }
        else if (type == "Rectangle") {
            std::string name;
            float x, y, vx, vy, r, g, b, width, height;
            iss >> name >> x >> y >> vx >> vy >> r >> g >> b >> width >> height;
            
            auto rect = std::make_shared<sf::RectangleShape>(sf::Vector2f(width, height));
            rect->setFillColor(sf::Color(r, g, b, 255));
            rect->setPosition(x, y);
            shapes.push_back(rect);
            velocities.push_back(sf::Vector2f(vx, vy));
        }
    }

    
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML Works");
    
    // // window.setVerticalSyncEnabled(true); // call it once, after creating the window
    // sf:: CircleShape circle(50);
    // circle.setFillColor(sf::Color::Green);
    // circle.setPosition(300.0f, 300.0f);
    // float circleMoveSpeed = 0.5f;


    
    sf::Font myFont;
    if (!myFont.loadFromFile(font_path)) 
    {
        std::cerr << "Error: Failed to load font from '" << font_path << "'" << std::endl;
        exit(-1);
    }
    window.setFramerateLimit(60); // call it once, after creating the window

    sf::Text text("Sample text", myFont, font_size);
    text.setFillColor(sf::Color(font_r, font_g, font_b));

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
                    // circleMoveSpeed *= -1.0f;
                }
            }
        }
        for(size_t i = 0; i < shapes.size(); i++)
        {
            auto& shape = shapes[i];
            auto& velocity = velocities[i];
            shape->setPosition(shape->getPosition().x + velocity.x, shape->getPosition().y + velocity.y);
        }

        window.clear();
        for(auto &shape:shapes)
        {
            window.draw(*shape);
        }
        window.draw(text);
        window.display();
    }
    return 0;
}