#include "button.hpp"
#include <iostream>
#include <string>

Button::Button()
{
    /*sf::Font font;
    if (!font.loadFromFile(DEFAULT_FONT))
    {
        std::cout << "Error loading font" << std::endl;
    }*/
    this->default_color = COLOR_RELEASE;
    this->click_color = COLOR_CLICK;
    this->hover_color = calcHoverColor(click_color);
    sf::RectangleShape button(sf::Vector2f(200, 50));
    button.setPosition(sf::Vector2f(400, 300));
    button.setFillColor(this->default_color);
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::Black);
    //sf::Text text;
    //sf::Font font;
    if (!textFont.loadFromFile(DEFAULT_FONT))
    {
        std::cout << "Error loading font" << std::endl;
    }
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setString("Hello");
    text.setFont(textFont);
    text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
    text.setPosition(button.getPosition() + (button.getSize() / 2.f));
    callback_data = nullptr;
    event_callback = nullptr;
    clicked = false;
    
    //Button(sf::Vector2f(200, 50), sf::Vector2f(400, 300), COLOR_RELEASE, COLOR_CLICK, font, "Hello");
}

Button::~Button()
{
    if((callback_data != nullptr) && (callback_data->size() > 0)) {
        std::cout << "Vector size: " << callback_data->size() << std::endl;
        std::cout << "Vector capacity: " << callback_data->capacity() << std::endl << std::endl;
        delete callback_data;
    }
}

bool Button::isMouseOver(sf::Event &event)
{
    if((event.type == sf::Event::MouseButtonPressed) || (event.type == sf::Event::MouseButtonReleased))
    {
        if((event.mouseButton.x > button.getPosition().x && event.mouseButton.x < button.getPosition().x + button.getSize().x) && (event.mouseButton.y > button.getPosition().y && event.mouseButton.y < button.getPosition().y + button.getSize().y))
        {
            return true;
        }
        return false;
    }
    if((event.mouseMove.x > button.getPosition().x && event.mouseMove.x < button.getPosition().x + button.getSize().x) && (event.mouseMove.y > button.getPosition().y && event.mouseMove.y < button.getPosition().y + button.getSize().y))
    {
        return true;
    }
    return false;
}

void Button::draw(sf::RenderWindow &window)
{
    window.draw(button);
    if((text.getString() != " ") && (text.getString() != "")) {
        window.draw(text);
    }
}

void Button::eventHandler(sf::Event &event)
{
    // std::cout << "Mouse y: " << event.mouseMove.y << std::endl;
    // std::cout << "Mouse x: " << event.mouseMove.x << std::endl;
    if(event_callback != nullptr) {
        if(isMouseOver(event) && (event.type == sf::Event::MouseButtonReleased) && (event.mouseButton.button == sf::Mouse::Left) && clicked) {
            event_callback(callback_data);
        }
    }
    sf::Color tempColor = button.getFillColor();
    if((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left))
    {
        if(isMouseOver(event))
        {
            clicked = true;
            //this->color = COLOR_CLICK;
            tempColor = this->click_color;
            //button.setFillColor(this->click_color);
            // std::string buttonText = text.getString();
            // std::cout << "Clicked button " << buttonText << std::endl;
        } else {
            clicked = false;
        }
    }
    else if(event.type == sf::Event::MouseButtonReleased)
    {
        clicked = false;
        if(isMouseOver(event))
        {
            tempColor = this->hover_color;
            //tempColor.a = tempalpha;
        } else {
            tempColor = this->default_color;
            //std::cout << "Mouse out" << std::endl;
            //tempColor.a = tempalpha;
        }
    } else if(event.type == sf::Event::MouseMoved)
    {
        //sf::Color tempColor = button.getFillColor();
        //int tempalpha = tempColor.a;
        if(isMouseOver(event))
        {
            tempColor = this->hover_color;
            
        } else {
            tempColor = this->default_color;
        }
    }
    button.setFillColor(tempColor);
    
}

void Button::changePosition(sf::Vector2f position)
{
    button.setPosition(position);
    text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
    text.setPosition(position + (button.getSize() / 2.f));
}

void Button::changeSize(sf::Vector2f size)
{
    button.setSize(size);
    text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
    text.setPosition(button.getPosition() + (button.getSize() / 2.f));
}

void Button::resizeBoxToText()
{
    sf::FloatRect textRect = text.getLocalBounds();
    if(textRect.width + 25 > button.getSize().x) {
        changeSize(sf::Vector2f(textRect.width + 25, button.getSize().y));
    }
    if(textRect.height + 10 > button.getSize().y) {
        changeSize(sf::Vector2f(button.getSize().x, textRect.height + 10));
    }
}

sf::Color Button::calcHoverColor(sf::Color color)
{
    sf::Color tempColor = color;
    if(tempColor.r * 1.4 > 255) {
        tempColor.r = 255;
    } else if(tempColor.r <= 50) {
        tempColor.r += 100;
    } else {
        tempColor.r = tempColor.r * 1.4;
    }
    if(tempColor.g * 1.4 > 255) {
        tempColor.g = 255;
    } else if(tempColor.g <= 50) {
        tempColor.g += 100;
    } else {
        tempColor.g = tempColor.g * 1.4;
    }
    if(tempColor.b * 1.4 > 255) {
        tempColor.b = 255;
    } else if(tempColor.b <= 50) {
        tempColor.b += 100;
    } else {
        tempColor.b = tempColor.b * 1.4;
    }
    return tempColor;
}
