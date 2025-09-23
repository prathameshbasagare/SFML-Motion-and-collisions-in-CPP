#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui-sfml/imgui-SFML.h"
#include <iostream>
#include<vector>
#include<memory>
#include<fstream>
#include<sstream>

enum class ShapeType {
    Rectangle,
    Circle
};

class Shape
{
    private:
    std::shared_ptr<sf::Shape> shape_;
    ShapeType type_;
    sf::Vector2f velocity_;
    sf::Text name_;

    public:
    // Constructor for Rectangle
    Shape(float x, float y, float r, float g, float b, float width, float height)
        : type_(ShapeType::Rectangle)
    {
        shape_ = std::make_shared<sf::RectangleShape>(sf::Vector2f(width, height));
        shape_->setFillColor(sf::Color(r, g, b, 255));
        shape_->setPosition(x, y);
    }

    // Constructor for Circle
    Shape(float x, float y, float r, float g, float b, float radius)
        : type_(ShapeType::Circle)
    {
        shape_ = std::make_shared<sf::CircleShape>(radius);
        shape_->setFillColor(sf::Color(r, g, b, 255));
        shape_->setPosition(x, y);
    }

    sf::Shape& getShape()
    {
        return *shape_;
    }

    void setSpeed(float vx, float vy)
    {
        velocity_.x = vx;
        velocity_.y = vy;
    }

    void setName(const std::string& nameStr, sf::Font& font, float size, float r, float g, float b)
    {
        float x = shape_->getPosition().x;
        float y = shape_->getPosition().y;

        sf::Text text(nameStr, font, size);
        text.setFillColor(sf::Color(r, g, b));
        sf::FloatRect bounds = text.getLocalBounds();

        if (type_ == ShapeType::Rectangle) {
            // Rectangle centering logic
            sf::Vector2f dim = static_cast<sf::RectangleShape*>(shape_.get())->getSize();
            float posX = x + dim.x / 2.f - (bounds.width / 2.f + bounds.left);
            float posY = y + dim.y / 2.f - (bounds.height / 2.f + bounds.top);
            text.setPosition(posX, posY);
        } else {
            // Circle centering logic
            float radius = static_cast<sf::CircleShape*>(shape_.get())->getRadius();
            float posX = x + radius - (bounds.width / 2.f + bounds.left);
            float posY = y + radius - (bounds.height / 2.f + bounds.top);
            text.setPosition(posX, posY);
        }

        name_ = text;
    }

    sf::Text& getName()
    {
        return name_;
    }

    void move()
    {
        shape_->setPosition(shape_->getPosition().x + velocity_.x,
                           shape_->getPosition().y + velocity_.y);
        name_.setPosition(name_.getPosition().x + velocity_.x,
                         name_.getPosition().y + velocity_.y);
    }

    void invertXSpeed()
    {
        velocity_.x *= -1.0f;
    }

    void invertYSpeed()
    {
        velocity_.y *= -1.0f;
    }

    void handleCollision(int wWidth, int wHeight)
    {
        float x = shape_->getPosition().x;
        float y = shape_->getPosition().y;

        if (type_ == ShapeType::Rectangle) {
            sf::Vector2f dim = static_cast<sf::RectangleShape*>(shape_.get())->getSize();
            if (x <= 0 || x + dim.x >= wWidth) {
                invertXSpeed();
            }
            if (y <= 0 || y + dim.y >= wHeight) {
                invertYSpeed();
            }
        } else {
            float radius = static_cast<sf::CircleShape*>(shape_.get())->getRadius();
            if (x <= 0 || x + 2 * radius >= wWidth) {
                invertXSpeed();
            }
            if (y <= 0 || y + 2 * radius >= wHeight) {
                invertYSpeed();
            }
        }
    }

    ShapeType getType() const
    {
        return type_;
    }
};


int main()
{
    int wWidth = 1280;
    int wHeight = 720;
    // Load shapes from config.txt
    std::ifstream configFile("../config.txt");
    std::string line;
    

    std::vector<Shape> shapes;
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
            float x, y, vx, vy, r, g, b, radius;
            iss >> name >> x >> y >> vx >> vy >> r >> g >> b >> radius;

            Shape shape(x, y, r, g, b, radius);
            shape.setSpeed(vx, vy);
            shape.setName(name, myFont, font_size, font_r, font_g, font_b);
            shapes.push_back(std::move(shape));
        }
        else if (type == "Rectangle") {
            std::string name;
            float x, y, vx, vy, r, g, b, width, height;
            iss >> name >> x >> y >> vx >> vy >> r >> g >> b >> width >> height;

            Shape shape(x, y, r, g, b, width, height);
            shape.setSpeed(vx, vy);
            shape.setName(name, myFont, font_size, font_r, font_g, font_b);
            shapes.push_back(std::move(shape));
        }
    }

    
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML Works");

    window.setFramerateLimit(1000); // call it once, after creating the window
   
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
                    for(auto &shape:shapes)
                    {
                        shape.invertXSpeed();
                        shape.invertYSpeed();
                    }
                }
            }
        }

        for(auto &shape : shapes)
        {
            shape.handleCollision(wWidth, wHeight);
            shape.move();
        }

        window.clear();
        for(auto &shape : shapes)
        {
            window.draw(shape.getShape());
            window.draw(shape.getName());
        }
        // window.draw(text);
        window.display();
    }
    return 0;
}