#include "radio_button.hpp"
#include <iostream>

// RadioButton::RadioButton(sf::Vector2f preferred_size, sf::Vector2f position, sf::Color default_color, sf::Color click_color, sf::Color sel_click_color, sf::Font &font, std::string buttonText, bool outline) : Button(preferred_size, position, default_color, click_color, font, buttonText)
// {
//     // this->default_color = default_color;
//     // this->click_color = click_color;
//     // this->default_color.a = 255;
//     // this->click_color.a = 255;
//     button.setFillColor(sf::Color::White);
//     if(outline) {
//         button.setOutlineThickness(2);
//         button.setOutlineColor(sf::Color::Black);
//     } else {
//         button.setOutlineThickness(0);
//     }
//     this->sel_click_color = sel_click_color;
//     this->sel_hover_color = calcHoverColor(this->sel_click_color);
//     this->sel_default_color = calcHoverColor(this->sel_hover_color);
//     createCircles();
//     sf::Vector2f pos = text.getPosition();
//     text.setPosition(sf::Vector2f(pos.x + button.getSize().y / 2, pos.y));
//     // button.setOutlineThickness(2);
//     // button.setOutlineColor(sf::Color::Black);
//     //button.setOrigin(size.x / 2, size.y / 2);
//     // text = sf::Text(buttonText, font);
//     // text.setCharacterSize(24);
//     // text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
//     // text.setPosition(button.getPosition() + (button.getSize() / 2.f));
//     // text.setFillColor(sf::Color::Black);
//     //resize = true;
//     //clicked = false;
//     isSelected = false;
// }

// RadioButton::RadioButton(sf::Vector2f preferred_size, sf::Vector2f position, sf::Color default_color, sf::Color click_color, sf::Color sel_click_color, sf::Text buttonText, bool outline) : Button(preferred_size, position, default_color, click_color, buttonText)
// {
//     // this->default_color = default_color;
//     // this->click_color = click_color;
//     // this->default_color.a = 255;
//     // this->click_color.a = 255;
//     button.setFillColor(sf::Color::White);
//     if(outline) {
//         button.setOutlineThickness(2);
//         button.setOutlineColor(sf::Color::Black);
//     } else {
//         button.setOutlineThickness(0);
//     }
//     this->sel_click_color = sel_click_color;
//     this->sel_hover_color = calcHoverColor(this->sel_click_color);
//     this->sel_default_color = calcHoverColor(this->sel_hover_color);
//     createCircles();
//     sf::Vector2f pos = text.getPosition();
//     text.setPosition(sf::Vector2f(pos.x + button.getSize().y / 2, pos.y));
//     isSelected = false;
// }

// RadioButton::RadioButton(sf::Vector2f preferred_size, sf::Vector2f position, sf::Color default_color, sf::Color click_color, sf::Color sel_click_color, bool outline) : Button(preferred_size, position, default_color, click_color)
// {
//     // this->default_color = default_color;
//     // this->click_color = click_color;
//     // this->default_color.a = 255;
//     // this->click_color.a = 255;
//     button.setFillColor(sf::Color::White);
//     if(outline) {
//         button.setOutlineThickness(2);
//         button.setOutlineColor(sf::Color::Black);
//     } else {
//         button.setOutlineThickness(0);
//     }
//     this->sel_click_color = sel_click_color;
//     this->sel_hover_color = calcHoverColor(this->sel_click_color);
//     this->sel_default_color = calcHoverColor(this->sel_hover_color);
//     createCircles();
//     //button.setOutlineThickness(2);
//     //button.setOutlineColor(sf::Color::Black);
//     //button.setOrigin(size.x / 2, size.y / 2);
//     // text = sf::Text(buttonText, font);
//     // text.setCharacterSize(24);
//     // text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
//     // text.setPosition(button.getPosition() + (button.getSize() / 2.f));
//     // text.setFillColor(sf::Color::Black);
//     //resize = true;
//     isSelected = false;
// }

// RadioButton::RadioButton() : Button()
// {
//     button.setFillColor(sf::Color::White);
//     button.setOutlineThickness(0);
//     this->sel_click_color = sf::Color::Blue;
//     this->sel_hover_color = calcHoverColor(this->sel_click_color);
//     this->sel_default_color = calcHoverColor(this->sel_hover_color);
//     createCircles();
//     isSelected = false;
// }

RadioButton::~RadioButton()
{
}

void RadioButton::createCircles()
{
    // std::cout << "Click color: " << (int)this->click_color.r << ", " << (int)this->click_color.g << ", " << (int)this->click_color.b << ", " << (int)this->click_color.a << std::endl;
    // std::cout << "Hover color: " << (int)this->hover_color.r << ", " << (int)this->hover_color.g << ", " << this->hover_color.b << ", " << (int)this->hover_color.a << std::endl;
    // std::cout << "Default color: " << (int)this->default_color.r << ", " << (int)this->default_color.g << ", " << this->default_color.b << ", " << (int)this->default_color.a << std::endl;
    // std::cout << "Selected click color: " << (int)this->sel_click_color.r << ", " << (int)this->sel_click_color.g << ", " << (int)this->sel_click_color.b << ", " << (int)this->sel_click_color.a << std::endl;
    // std::cout << "Selected hover color: " << (int)this->sel_hover_color.r << ", " << (int)this->sel_hover_color.g << ", " << (int)this->sel_hover_color.b << ", " << (int)this->sel_hover_color.a << std::endl;
    // std::cout << "Selected default color: " << (int)this->sel_default_color.r << ", " << (int)this->sel_default_color.g << ", " << (int)this->sel_default_color.b << ", " << (int)this->sel_default_color.a << std::endl;
    circleOuter.setRadius((button.getSize().y * 0.5)/2);
    circleOuter.setOrigin(circleOuter.getRadius(), circleOuter.getRadius());
    circleOuter.setPosition(button.getPosition().x + button.getSize().y / 2, button.getPosition().y + button.getSize().y / 2);
    circleOuter.setFillColor(this->default_color);
    circleOuter.setOutlineThickness(0);

    circleInner.setRadius(circleOuter.getRadius() * 0.5);
    circleInner.setOrigin(circleInner.getRadius(), circleInner.getRadius());
    circleInner.setPosition(circleOuter.getPosition());
    circleInner.setFillColor(this->default_color);
    circleInner.setOutlineThickness(0);
    //return sf::CircleShape();
}

bool RadioButton::eventHandler(sf::Event &event)
{
    bool wasClicked = false;
    sf::Color tempColor = circleOuter.getFillColor();
    if((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left))
    {
        if(isMouseOver(event))
        {
            clicked = true;
            if(isSelected) {
                tempColor = this->sel_click_color;
            } else {
                tempColor = this->click_color;
            }
        } else {
            clicked = false;
            if(isSelected) {
                tempColor = this->sel_default_color;
            } else {
                tempColor = this->default_color;
            }
        }
    }
    else if(event.type == sf::Event::MouseButtonReleased)
    {
        if(isMouseOver(event))
        {
            //tempColor = this->hover_color;
            if(clicked) {
                isSelected = true;
                tempColor = this->sel_hover_color;
                wasClicked = true;
            } else {
                if(isSelected) {
                    tempColor = this->sel_hover_color;
                } else {
                    tempColor = this->hover_color;
                }
                //tempColor = this->hover_color;
            }
            //clicked = false;
        } else {
            if(isSelected) {
                tempColor = this->sel_default_color;
            } else {
                tempColor = this->default_color;
            }
            //tempColor = this->default_color;
        }
        clicked = false;
    } else if(event.type == sf::Event::MouseMoved)
    {
        if(isMouseOver(event))
        {
            if(isSelected) {
                tempColor = this->sel_hover_color;
            } else {
                tempColor = this->hover_color;
            }
            //tempColor = this->hover_color;
        } else {
            if(isSelected) {
                tempColor = this->sel_default_color;
            } else {
                tempColor = this->default_color;
            }
            //tempColor = this->default_color;
        }
    }
    circleOuter.setFillColor(tempColor);
    return wasClicked;
}

void RadioButton::setSelected(bool selected)
{
    isSelected = selected;
    // if(isSelected) {
    //     circleOuter.setFillColor(this->sel_hover_color);
    // } else {
    //     circleOuter.setFillColor(this->default_color);
    // }
    if(!isSelected) {
        circleOuter.setFillColor(this->default_color);
    }
}

void RadioButton::setTextColor(sf::Color color)
{
    text.setFillColor(color);
}

void RadioButton::draw(sf::RenderWindow &window)
{
    Button::draw(window);
    window.draw(circleOuter);
    window.draw(circleInner);
}

RadioButtonSet::RadioButtonSet(sf::Vector2f size, sf::Vector2f position, sf::Color default_color, sf::Color click_color, sf::Color sel_click_color, std::vector<std::string> buttonText, sf::Font &font)
{
    sf::Vector2f buttonSize(size.x, size.y / buttonText.size());
    for(int i = 0; i < buttonText.size(); i++)
    {
        radioButtons.push_back(RadioButton(buttonSize, sf::Vector2f(position.x, position.y + (i * buttonSize.y)), default_color, click_color, sel_click_color, font, buttonText[i], false));
    }
    //std::cout << std::endl;
    selectedButtonIndex = -1;
}

RadioButtonSet::~RadioButtonSet()
{
}

void RadioButtonSet::draw(sf::RenderWindow &window)
{
    for(int i = 0; i < radioButtons.size(); i++)
    {
        radioButtons[i].draw(window);
    }
    //window.draw(radioButtons);
}

void RadioButtonSet::eventHandler(sf::Event &event)
{
    bool newSelected = false;
    for(int i = 0; i < radioButtons.size(); i++)
    {
        if(radioButtons[i].eventHandler(event))
        {
            newSelected = true;
            selectedButtonIndex = i;
            radioButtons[selectedButtonIndex].setSelected(true);
            for(int j = 0; j < i; j++)
            {
                radioButtons[j].setSelected(false);
            }
        } else if(newSelected) {
            radioButtons[i].setSelected(false);
        }
    }
}

void RadioButtonSet::reveal_correct_answer(int correct_answer)
{
    if((correct_answer >= 0) && (correct_answer < radioButtons.size()) && (selectedButtonIndex >= 0))
    {
        radioButtons[correct_answer].setTextColor(sf::Color::Green);

        if(correct_answer != selectedButtonIndex) {
            radioButtons[selectedButtonIndex].setTextColor(sf::Color::Red);
        }
    }
}
