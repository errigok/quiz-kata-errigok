#ifndef DROP_DOWN_HPP
#define DROP_DOWN_HPP

#include "button.hpp"
#include "scroll_bar.hpp"

class DropDown : public Button
{
    public:
        DropDown(sf::Vector2f size, sf::Vector2f position, sf::Color default_color, sf::Color click_color, sf::Font &font, std::vector<std::string> menuOptions, int defaultOption, sf::Vector2u windowSize, int maxOptionsShown, sf::Text defaultOptionText);
        ~DropDown();
        void draw(sf::RenderWindow &window);
        void eventHandler(sf::Event &event);
        std::string getSelectedOptionText() { return choices[selectedOption].text.getString(); }
        bool isMouseOverAny(sf::Event &event);
        void manualClose();
        bool wasThisLastClicked() { return wasLastClicked; }
        Button* getDropDown() { return dropDown; }
    protected:
        sf::ConvexShape createDropDownArrow(sf::Vector2f position, sf::Vector2f size);
    private:
        std::vector<Button> choices;
        bool isDropDownOpen;
        int selectedOption;
        int defaultOption;
        sf::RectangleShape dropDownBounds;
        sf::ConvexShape dropDownArrow;
        int maxShownEntries;
        ScrollBar* scrollBar;
        int clickedOption;
        //bool isPressed;
        Button* pressedButton;
        void ResizeDropDown(sf::FloatRect textRect, sf::FloatRect textRectLocal, sf::Vector2f buttonSize, sf::Vector2f buttonPos, Button* drop);
        //void resizeBoxToText(Button *button);
        bool wasLastClicked;
        Button* dropDown;
        //sf::Font font;
};

#endif // DROP_DOWN_HPP