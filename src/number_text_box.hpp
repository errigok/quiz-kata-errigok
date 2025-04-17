#ifndef NUMBER_TEXT_BOX_HPP
#define NUMBER_TEXT_BOX_HPP

#include "button.hpp"
#include <iostream>

class NumberTextBox : public Button
{
    public:
        NumberTextBox(sf::Vector2f preferred_size, sf::Vector2f position, sf::Color default_color, sf::Color click_color, sf::Font &font) : Button(preferred_size, position, default_color, click_color, font, "") {
            //std::cout << "NumberTextBox constructor called" << std::endl;
            this->defaultText.setFont(font);
            this->defaultText.setCharacterSize(24);
            this->defaultText.setString("Enter a number");
            this->defaultText.setFillColor(sf::Color::Black);
            this->defaultText.setStyle(sf::Text::Italic);
            this->defaultText.setOrigin(sf::Vector2f(defaultText.getLocalBounds().getPosition().x, defaultText.getGlobalBounds().getSize().y / 2.f + defaultText.getLocalBounds().getPosition().y));        
            this->text = defaultText;
            this->text.setOrigin(sf::Vector2f(text.getLocalBounds().getPosition().x, text.getGlobalBounds().getSize().y / 2.f + text.getLocalBounds().getPosition().y));
            this->text.setPosition(sf::Vector2f(button.getPosition().x + 10, button.getPosition().y + button.getSize().y / 2.f));
            this->isSelected = false;
            this->wasClicked = false;
            this->textPlace = 0;
            this->inputText = "";
        };
        ~NumberTextBox();
        void eventHandler(sf::Event &event);
        int getNumber();
        std::string getNumberString() { return inputText; }
    protected:
    private:
        bool isSelected;
        bool wasClicked;
        int textPlace;
        std::string inputText;
        sf::Text defaultText;
};

#endif // NUMBER_TEXT_BOX_HPP