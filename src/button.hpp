#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <any>
#include <iostream>

#define COLOR_OVER sf::Color(0, 0, 0, 255)
#define COLOR_OUT sf::Color(0, 0, 0, 100)
#define COLOR_RELEASE sf::Color(203, 203, 203, 255)
#define COLOR_CLICK sf::Color(128, 128, 128, 255)
#if defined(_WIN32)
    #define DEFAULT_FONT "C:/Windows/Fonts/arial.ttf"
#else
    #define DEFAULT_FONT "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
#endif

class Button
{
    public:
        sf::RectangleShape button;
        sf::Text text;
        Button(sf::Vector2f preferred_size, sf::Vector2f position, sf::Color default_color, sf::Color click_color, sf::Font &font, std::string buttonText) : Button(preferred_size, position, default_color, click_color) {
            this->text.setFont(font);
            this->text.setCharacterSize(24);
            this->text.setFillColor(sf::Color::Black);
            this->text.setString(buttonText);
            this->text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
            //this->text.setPosition(position.x + (preferred_size.x / 2) - (this->text.getGlobalBounds().width / 2), position.y + (preferred_size.y / 2) - (this->text.getGlobalBounds().height / 2));
            this->text.setPosition(button.getPosition() + (button.getSize() / 2.f));
            this->textFont = font;
        };
        Button(sf::Vector2f preferred_size, sf::Vector2f position, sf::Color default_color, sf::Color click_color, sf::Text buttonText) : Button(preferred_size, position, default_color, click_color) {
            this->text = buttonText;
            this->text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
            //this->text.setPosition(position.x + (preferred_size.x / 2) - (this->text.getGlobalBounds().width / 2), position.y + (preferred_size.y / 2) - (this->text.getGlobalBounds().height / 2));
            this->text.setPosition(button.getPosition() + (button.getSize() / 2.f));
            this->textFont = *buttonText.getFont();
        };
        Button(sf::Vector2f preferred_size, sf::Vector2f position, sf::Color default_color, sf::Color click_color) : Button() {
            //std::cout << "Button constructor called" << std::endl;
            this->button.setSize(preferred_size);
            this->button.setPosition(position);
            this->default_color = default_color;
            this->click_color = click_color;
            this->hover_color = calcHoverColor(click_color);
            this->button.setFillColor(default_color);
            this->button.setOutlineColor(sf::Color::Black);
            this->button.setOutlineThickness(2);
            this->text.setString(" ");
        };
        Button();
        ~Button();
        bool isMouseOver(sf::Event &event);
        void draw(sf::RenderWindow &window);
        void eventHandler(sf::Event &event);
        void changePosition(sf::Vector2f position);
        void changeSize(sf::Vector2f size);
        void resizeBoxToText();
        void setEventHandlerCallback(void (*event_callback)(std::vector<std::any>*), std::vector<std::any>* data)
        {
            this->callback_data = data;
            this->event_callback = event_callback;
        }
        sf::FloatRect getGlobalBoundsText() { return this->text.getGlobalBounds(); }
        //void changeResize(bool resize) { this->resize = resize; }
    protected:
        sf::Color default_color;
        sf::Color click_color;
        sf::Color hover_color;
        sf::Color calcHoverColor(sf::Color color);
        sf::Font textFont;
    private:
        void (*event_callback)(std::vector<std::any>*);
        std::vector<std::any>* callback_data;
        bool clicked;
};

#endif // BUTTON_HPP