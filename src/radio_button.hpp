#ifndef RADIO_BUTTON_HPP
#define RADIO_BUTTON_HPP

#include "button.hpp"
#include <vector>
#include <iostream>

class RadioButton : public Button
{
    public:
        RadioButton(sf::Vector2f preferred_size, sf::Vector2f position, sf::Color default_color, sf::Color click_color, sf::Color sel_click_color, sf::Font &font, std::string buttonText, bool outline) : RadioButton(preferred_size, position, default_color, click_color, sel_click_color, outline) {
            this->text.setFont(font);
            this->text.setCharacterSize(24);
            this->text.setFillColor(sf::Color::Black);
            this->text.setString(buttonText);
            this->text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
            this->text.setPosition(sf::Vector2f(text.getPosition().x, button.getPosition().y + (button.getSize().y / 2)));
            this->textFont = font;
        };
        RadioButton(sf::Vector2f preferred_size, sf::Vector2f position, sf::Color default_color, sf::Color click_color, sf::Color sel_click_color, sf::Text buttonText, bool outline) : RadioButton(preferred_size, position, default_color, click_color, sel_click_color, outline) {
            this->text = buttonText;
            this->text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
            this->text.setPosition(sf::Vector2f(text.getPosition().x, button.getPosition().y + (button.getSize().y / 2)));
            this->textFont = *buttonText.getFont();
        };
        RadioButton(sf::Vector2f preferred_size, sf::Vector2f position, sf::Color default_color, sf::Color click_color, sf::Color sel_click_color, bool outline) : Button(preferred_size, position, default_color, click_color) {
            //std::cout << "RadioButton constructor called" << std::endl;
            this->button.setFillColor(sf::Color::White);
            if(outline) {
                this->button.setOutlineThickness(2);
                this->button.setOutlineColor(sf::Color::Black);
            } else {
                this->button.setOutlineThickness(0);
            }
            this->sel_click_color = sel_click_color;
            this->sel_hover_color = calcHoverColor(this->sel_click_color);
            this->sel_default_color = calcHoverColor(this->sel_hover_color);
            createCircles();
            this->isSelected = false;
        };
        RadioButton() : Button() {
            //std::cout << "RadioButton default constructor called" << std::endl;
            this->button.setFillColor(sf::Color::White);
            this->button.setOutlineThickness(0);
            this->sel_click_color = sf::Color::Blue;
            this->sel_hover_color = calcHoverColor(this->sel_click_color);
            this->sel_default_color = calcHoverColor(this->sel_hover_color);
            createCircles();
            this->isSelected = false;
        };
        ~RadioButton();
        void draw(sf::RenderWindow &window);
        bool eventHandler(sf::Event &event);
        //void changePosition(sf::Vector2f position);
        void setSelected(bool selected);
        bool getSelected() { return isSelected; }
        void setTextColor(sf::Color color);
    protected:
        void createCircles();
    private:
        sf::CircleShape circleOuter;
        sf::CircleShape circleInner;
        sf::Color sel_default_color;
        sf::Color sel_click_color;
        sf::Color sel_hover_color;
        bool clicked;
        bool isSelected;
        //bool radioSelected;
        //void radioButtonColorHandler(sf::Event &event);
};

class RadioButtonSet
{
    public:
        RadioButtonSet(sf::Vector2f size, sf::Vector2f position, sf::Color default_color, sf::Color click_color, sf::Color sel_click_color, std::vector<std::string> buttonText, sf::Font &font);
        ~RadioButtonSet();
        void draw(sf::RenderWindow &window);
        void eventHandler(sf::Event &event);
        int getSelectedButtonIndex() { return selectedButtonIndex; }
        std::string getSelectedButtonText() { return radioButtons[selectedButtonIndex].text.getString(); }
        void reveal_correct_answer(int correct_answer);
        //void setSelectedButtonIndex(int index);
    protected:
    private:
        std::vector<RadioButton> radioButtons;
        int selectedButtonIndex;
};

#endif // RADIO_BUTTON_HPP