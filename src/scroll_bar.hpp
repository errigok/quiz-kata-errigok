#ifndef SCROLL_BAR_HPP
#define SCROLL_BAR_HPP

#include "button.hpp"
#include <iostream>

#define DEFAULT_FULLSCROLL 8
#define DEFAULT_UNITS_SHOWN 4

class ScrollBar : public Button
{
    public:
        ScrollBar(sf::Vector2f size, sf::Vector2f position, int fullScrollSize, int unitsShown) : ScrollBar() {
            this->fullScrollSize = fullScrollSize;
            this->unitsShown = unitsShown;
            this->yLocation = position.y;
            this->background.setSize(sf::Vector2f(size.x, size.y));
            this->background.setPosition(position);
            this->changeSize(sf::Vector2f(size.x, (background.getSize().y / fullScrollSize * unitsShown)));
            this->changePosition(position);
        };
        ScrollBar() : Button(sf::Vector2f(20, (200 / DEFAULT_FULLSCROLL * DEFAULT_UNITS_SHOWN)), sf::Vector2f(100, 100), COLOR_RELEASE, COLOR_CLICK) {
            //std::cout << "ScrollBar constructor called" << std::endl;
            sf::RectangleShape background(sf::Vector2f(20, 200));
            this->background.setPosition(sf::Vector2f(100, 100));
            this->background.setFillColor(sf::Color::White);
            this->background.setOutlineThickness(2);
            this->background.setOutlineColor(sf::Color::Black);
            this->fullScrollSize = DEFAULT_FULLSCROLL;
            this->unitsShown = DEFAULT_UNITS_SHOWN;
            this->button.setOutlineThickness(0);
            this->isPressed = false;
            this->topPosition = 0;
            this->yLocation = 100;
        };
        ~ScrollBar();
        void draw(sf::RenderWindow &window);
        int getTopPosition();
        void eventHandler(sf::Event &event, int* topPos);
        void reset_top();
        bool isMouseOverAll(sf::Event &event);
        //Button* scrollBar;
    protected:
    private:
        int fullScrollSize;
        int unitsShown;
        int topPosition;
        
        sf::RectangleShape background;
        bool isPressed;
        int yLocation;
        //sf::Font font;
};

#endif // SCROLL_BAR_HPP