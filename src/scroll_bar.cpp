#include "scroll_bar.hpp"
#include <iostream>

ScrollBar::~ScrollBar()
{
    //delete scrollBar;
}

void ScrollBar::draw(sf::RenderWindow &window)
{
    window.draw(background);
    this->Button::draw(window);
}

int ScrollBar::getTopPosition()
{
    return this->topPosition;
}

void ScrollBar::eventHandler(sf::Event &event, int* topPos)
{
    this->Button::eventHandler(event);
    if(this->Button::isMouseOver(event))
    {
        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            isPressed = true;
            yLocation = event.mouseButton.y-this->button.getPosition().y;
            //std::cout << "Scroll bar clicked" << std::endl;
        }
    }
    if(isPressed)
    {
        if(event.type == sf::Event::MouseMoved)
        {
            if((event.mouseMove.y + this->button.getSize().y - yLocation) > background.getPosition().y + background.getSize().y)
            {
                this->button.setPosition(this->button.getPosition().x, background.getPosition().y + background.getSize().y - this->button.getSize().y);
                topPosition = fullScrollSize - unitsShown;
                //std::cout << "Scroll bar at bottom" << std::endl;
            } else if((event.mouseMove.y - yLocation) < background.getPosition().y)
            {
                this->button.setPosition(this->button.getPosition().x, background.getPosition().y);
                topPosition = 0;
                //std::cout << "Scroll bar at top" << std::endl;
            } else {
                this->button.setPosition(this->button.getPosition().x, event.mouseMove.y - yLocation);
                topPosition = (int)(((this->button.getPosition().y - background.getPosition().y) / background.getSize().y * fullScrollSize) + 0.5);
               // std::cout << "Scroll bar in the middle" << std::endl;
            }
        }
        if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
            isPressed = false;
            this->button.setPosition(this->button.getPosition().x, ((topPosition * 1.f) / fullScrollSize) * background.getSize().y + background.getPosition().y);
            //std::cout << "Scroll bar released" << std::endl;
        }
    } else if (event.type == sf::Event::MouseWheelScrolled)
    {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            if(event.mouseWheelScroll.delta > 0) {
                if (topPosition > 0) {
                    topPosition--;
                    this->button.setPosition(this->button.getPosition().x, ((topPosition * 1.f) / fullScrollSize) * background.getSize().y + background.getPosition().y);
                    //std::cout << "Scroll bar scrolled up" << std::endl;
                }
            } else if(event.mouseWheelScroll.delta < 0) {
                if (topPosition < fullScrollSize - unitsShown) {
                    topPosition++;
                    this->button.setPosition(this->button.getPosition().x, ((topPosition * 1.f) / fullScrollSize) * background.getSize().y + background.getPosition().y);
                    //std::cout << "Scroll bar scrolled down" << std::endl;
                }
            }
        }
        //std::cout << "Top position: " << topPosition << std::endl;
    }
    *topPos = topPosition;
}

void ScrollBar::reset_top()
{
    this->topPosition = 0;
}

bool ScrollBar::isMouseOverAll(sf::Event &event)
{
    if((event.type == sf::Event::MouseButtonPressed) || (event.type == sf::Event::MouseButtonReleased))
    {
        if((event.mouseButton.x > background.getPosition().x && event.mouseButton.x < background.getPosition().x + background.getSize().x) && (event.mouseButton.y > background.getPosition().y && event.mouseButton.y < background.getPosition().y + background.getSize().y))
        {
            return true;
        }
        return false;
    }
    if((event.mouseMove.x > background.getPosition().x && event.mouseMove.x < background.getPosition().x + background.getSize().x) && (event.mouseMove.y > background.getPosition().y && event.mouseMove.y < background.getPosition().y + background.getSize().y))
    {
        return true;
    }
    return false;
}
