#include "number_text_box.hpp"
#include <iostream>

NumberTextBox::~NumberTextBox()
{
}

void NumberTextBox::eventHandler(sf::Event &event)
{
    Button::eventHandler(event);
    if((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left)) {
        if(isMouseOver(event)) {
            wasClicked = true;
        } else {
            wasClicked = false;
        }
    } else if((event.type == sf::Event::MouseButtonReleased) && (event.mouseButton.button == sf::Mouse::Left)) {
        wasClicked = false;
        if(isMouseOver(event)) {
            isSelected = !isSelected;
        } else {
            isSelected = false;
        }
        sf::Vector2f pos = button.getPosition();
        if(!isSelected) {
            if(inputText == "") {
                text = defaultText;
                text.setPosition(sf::Vector2f(pos.x + 10, pos.y + button.getSize().y / 2.f));
            } else {
                text.setString(inputText);
                text.setPosition(sf::Vector2f(pos.x + 10, pos.y + button.getSize().y / 2.f));
            }
        } else {
            text.setString(inputText + '|');
            textPlace = inputText.length();
            text.setPosition(sf::Vector2f(pos.x + 10, pos.y + button.getSize().y / 2.f));
        }
    } else if(isSelected && event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::BackSpace) {
            if((inputText.length() > 0) && (textPlace > 0)) {
                if(inputText.length() == textPlace) {
                    inputText.pop_back();
                    text.setString(inputText + '|');
                    textPlace--;
                } else {
                    inputText.erase(textPlace - 1, 1);
                    textPlace--;
                    text.setString(inputText.substr(0, textPlace) + '|' + inputText.substr(textPlace));
                }
            }
        } else if(event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9) {
            // Handle number input
            if(inputText.length() < 10) {
                if(inputText.length() == textPlace) {
                    inputText += std::to_string(event.key.code - sf::Keyboard::Num0);
                    textPlace++;
                    text.setString(inputText + '|');
                } else {
                    inputText.insert(textPlace, std::to_string(event.key.code - sf::Keyboard::Num0));
                    textPlace++;
                    text.setString(inputText.substr(0, textPlace) + '|' + inputText.substr(textPlace));
                }
            }
        } else if((event.key.code == sf::Keyboard::Enter) || (event.key.code == sf::Keyboard::Escape)) {
            // Handle enter
            if(inputText == "") {
                text = defaultText;
            } else {
                text.setString(inputText);
            }
            sf::Vector2f pos = button.getPosition();
            text.setPosition(sf::Vector2f(pos.x + 10, pos.y + button.getSize().y / 2.f));
            isSelected = false;
        } else if(event.key.code == sf::Keyboard::Left) {
            // Handle left arrow
            if(textPlace > 0) {
                textPlace--;
                //inputText = inputText.substr(0, textPlace);
                text.setString(inputText.substr(0, textPlace) + '|' + inputText.substr(textPlace));
            }
        } else if(event.key.code == sf::Keyboard::Right) {
            // Handle right arrow
            if(textPlace < inputText.length()) {
                textPlace++;
                //inputText = inputText.substr(0, textPlace);
                text.setString(inputText.substr(0, textPlace) + '|' + inputText.substr(textPlace));
            }
        }
    }
}

int NumberTextBox::getNumber()
{
    if(inputText == "") {
        return -1;
    } else {
        return std::stoi(inputText);
    }
}
