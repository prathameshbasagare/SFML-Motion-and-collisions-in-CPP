#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui-sfml/imgui-SFML.h"
#include <iostream>
#include<vector>
#include<memory>
#include<fstream>
#include<sstream>

class Rect
{
    std::shared_ptr<sf::RectangleShape> shape;
    sf::Vector2f velocity;
    sf::Text name;
    public:
    Rect(float x, float y, float r, float g, float b, float width, float height)
    {
        shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(width, height));
        shape->setFillColor(sf::Color(r, g, b, 255));
        shape->setPosition(x, y);
    }
    sf::RectangleShape get()
    {
        return *shape;
    }
    void setSpeed(float vx, float vy)
    {
        velocity.x = vx;
        velocity.y = vy;
    }
    void setName(std::string nameStr, sf::Font& font, float s, float r, float g, float b)
    {
        // name = std::make_shared<sf::Text>(nameStr, font, s);
        
        float x = shape->getPosition().x;
        float y = shape->getPosition().y;

        sf::Text text(nameStr, font, s);
        text.setFillColor(sf::Color(r, g, b));

        // Get the local bounds of the text (includes offset and size)
        sf::FloatRect bounds = text.getLocalBounds();

        // Get the size of the shape (e.g. rectangle)
        sf::Vector2f dim = shape->getSize();

        // Calculate precise centered position inside the shape
        float posX = x + dim.x / 2.f - (bounds.width / 2.f + bounds.left);
        float posY = y + dim.y / 2.f - (bounds.height / 2.f + bounds.top);

        text.setPosition(posX, posY);

        name=text;
    }
    sf::Text& getName()
    {
        return name;
    }
    void move()
    {
        shape->setPosition(shape->getPosition().x+velocity.x , shape->getPosition().y + velocity.y);
        name.setPosition(name.getPosition().x+velocity.x , name.getPosition().y + velocity.y);
    }
    void invertXSpeed()
    {
        velocity.x *= -1.0f;
    }
    void invertYSpeed()
    {
        velocity.y *= -1.0f;
    }
    void handleCollision(int wWidth, int wHeight){
        float x = shape->getPosition().x, y= shape->getPosition().y;
        sf::Vector2f dim = shape->getSize();

        if(x<=0 || x + dim.x >= wWidth){
            this->invertXSpeed();
        }
        if(y<=0 || y+ dim.y >= wHeight){
            this->invertYSpeed();
        }
    }
};
class Circle
{
    std::shared_ptr<sf::CircleShape> shape;
    sf::Vector2f velocity;
    sf::Text name;
    public:
    Circle(float x, float y, float r, float g, float b, float R)
    {
        shape = std::make_shared<sf::CircleShape>(R);
        shape->setFillColor(sf::Color(r, g, b, 255));
        shape->setPosition(x, y);
    }
    sf::CircleShape get()
    {
        return *shape;
    }
    void setSpeed(float vx, float vy)
    {
        velocity.x = vx;
        velocity.y = vy;
    }
    void setName(std::string nameStr, sf::Font& font, float s, float r, float g, float b)
    {
        float x = shape->getPosition().x, y= shape->getPosition().y;
        float w = (float)name.getLocalBounds().width, h = (float)name.getCharacterSize();
        float radius = shape->getRadius();

        sf::Text text(nameStr, font, s);
        text.setFillColor(sf::Color(r, g, b));
        sf::FloatRect bounds = text.getLocalBounds();

        text.setPosition(
            x + radius - (bounds.width / 2.f + bounds.left),
            y + radius - (bounds.height / 2.f + bounds.top)
        );
        name = text;
    }
    sf::Text& getName()
    {
        return name;
    }
    void move(){
        shape->setPosition(shape->getPosition().x+velocity.x , shape->getPosition().y + velocity.y);
        name.setPosition(name.getPosition().x+velocity.x , name.getPosition().y + velocity.y);
    }
    void invertXSpeed(){
        velocity.x *= -1.0f;
    }
    void invertYSpeed()
    {
        velocity.y *= -1.0f;
    }
    void handleCollision(int wWidth, int wHeight){
        float x = shape->getPosition().x, y= shape->getPosition().y;
        float radius = shape->getRadius();

        if(x<=0 || x + 2 * radius >= wWidth){
            this->invertXSpeed();
        }
        if(y<=0 || y+ 2 * radius >= wHeight){
            this->invertYSpeed();
        }
    }
};


int main()
{
    int wWidth = 1280;
    int wHeight = 720;
    // Load shapes from config.txt
    std::ifstream configFile("../config.txt");
    std::string line;
    

    std::vector<Circle> circles;
    std::vector<Rect> recs;
    std::string font_path;
    float font_size, font_r, font_g, font_b;
    sf::Font myFont;
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
            if (!myFont.loadFromFile(font_path)) 
            {
                std::cerr << "Error: Failed to load font from '" << font_path << "'" << std::endl;
                exit(-1);
            }
        }
        if (type == "Circle") {
            std::string name;
            float x, y, vx, vy, r, g, b, R;
            iss >> name >> x >> y >> vx >> vy >> r >> g >> b >> R;
            
            Circle c(x,y,r,g,b,R);
            c.setSpeed(vx,vy);
            c.setName(name, myFont, font_size, font_r, font_g, font_b );
            circles.push_back(c);
        }
        else if (type == "Rectangle") {
            std::string name;
            float x, y, vx, vy, r, g, b, width, height;
            iss >> name >> x >> y >> vx >> vy >> r >> g >> b >> width >> height;
            
            Rect c(x,y,r,g,b,width,height);
            c.setSpeed(vx,vy);
            c.setName(name, myFont, font_size, font_r, font_g, font_b );
            recs.push_back(c);
        }
    }

    
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML Works");

    window.setFramerateLimit(1000); // call it once, after creating the window

    // sf::Text text("Sample text", myFont, font_size);
    // text.setFillColor(sf::Color(font_r, font_g, font_b));

    // text.setPosition(0, wHeight - (float)text.getCharacterSize());

 
   
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
                if(e.key.code == sf::Keyboard::X)
                {
                    // Not necessary for now.
                }
            }
        }

        for(auto &shape:circles)
        {
            shape.handleCollision(wWidth,wHeight);
            shape.move();   
        }
        for(auto &shape:recs)
        {
            shape.handleCollision(wWidth,wHeight);
            shape.move();   
        }

        window.clear();
        for(auto &shape:circles)
        {
            window.draw(shape.get());
            window.draw(shape.getName());
        }
        for(auto &shape:recs)
        {
            window.draw(shape.get());
            window.draw(shape.getName());
        }
        // window.draw(text);
        window.display();
    }
    return 0;
}