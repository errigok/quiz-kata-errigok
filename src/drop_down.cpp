#include "drop_down.hpp"
#include <iostream>
#include <locale>
#include <cstring>

//bool scrollTest = false;

DropDown::DropDown(sf::Vector2f size, sf::Vector2f position, sf::Color default_color, sf::Color click_color, sf::Font &font, std::vector<std::string> menuOptions, int defaultOption, sf::Vector2u windowSize, int maxOptionsShown, sf::Text defaultOptionText)
{
    this->textFont = font;
    defaultOptionText.setFont(this->textFont);
    defaultOptionText.setFillColor(sf::Color::Black);
    this->dropDown = new Button(sf::Vector2f(size.x+size.y, size.y), position, default_color, click_color, defaultOptionText);
    this->isDropDownOpen = false;
    sf::FloatRect textRect = dropDown->getGlobalBoundsText();
    sf::FloatRect textRectLocal = dropDown->text.getLocalBounds();
    sf::Vector2f buttonSize = dropDown->button.getSize();
    sf::Vector2f buttonPos = dropDown->button.getPosition();
    this->ResizeDropDown(textRect, textRectLocal, buttonSize, buttonPos, dropDown);
    
    sf::Vector2f size2 = dropDown->button.getSize();
    sf::FloatRect maxTextSize = dropDown->text.getLocalBounds();
    for(int i = 0; i < menuOptions.size(); i++)
    {
        Button button(size2, sf::Vector2f(position.x, position.y + ((size2.y + 2) * (i+1))), sf::Color::White, sf::Color::Blue, font, menuOptions[i]);
        if(maxTextSize.width < button.text.getLocalBounds().width)
        {
            maxTextSize.width = button.text.getLocalBounds().width;
        }
        if(maxTextSize.height < button.text.getLocalBounds().height)
        {
            maxTextSize.height = button.text.getLocalBounds().height;
        }
        choices.push_back(button);
    }
    if(maxTextSize.width + 30 + size2.y > size2.x) {
        //dropDown.changeSize(sf::Vector2f(maxTextSize.width + 10, size.y));
        for(int i = 0; i < choices.size(); i++)
        {
            choices[i].changeSize(sf::Vector2f(maxTextSize.width + 30 + size2.y, size2.y));
        }
        size2.x = maxTextSize.width + 30 + size2.y;
        dropDown->changeSize(sf::Vector2f(size2.x, size2.y));
    }
    if(maxTextSize.height + 10 > size2.y) {
        //dropDown.changeSize(sf::Vector2f(size.x, maxTextSize.height + 10));
        for(int i = 0; i < choices.size(); i++)
        {
            choices[i].changePosition(sf::Vector2f(position.x, position.y + ((maxTextSize.height + 10 + 2) * (i+1))));
            choices[i].changeSize(sf::Vector2f(size2.x, maxTextSize.height + 10));
        }
        size2.y = maxTextSize.height + 10;
        dropDown->changeSize(sf::Vector2f(size2.x + size2.y, size2.y));
    }
    this->defaultOption = defaultOption;
    dropDownArrow = createDropDownArrow(dropDown->button.getPosition(), dropDown->button.getSize());
    this->selectedOption = defaultOption;
    if(choices.size() > maxOptionsShown)
    {
        this->dropDownBounds = sf::RectangleShape(sf::Vector2f(size2.x, size2.y * maxOptionsShown));
        dropDownBounds.setPosition(position.x, position.y + size2.y + 2);
        maxShownEntries = maxOptionsShown;
    }
    else
    {
        this->dropDownBounds = sf::RectangleShape(sf::Vector2f(size2.x, size2.y * menuOptions.size()));
        dropDownBounds.setPosition(position.x, position.y + size2.y + 2);
        maxShownEntries = choices.size();
    }
    if((position.y + (size2.y * (menuOptions.size() + 1)) > windowSize.y) || (position.y + (size2.y * (maxShownEntries)) > windowSize.y))
    {
        int newMaxShownEntries = 0;
        float y = position.y + size2.y + 2;
        while((y < windowSize.y) && (newMaxShownEntries < maxShownEntries)) {
            y += size2.y;
            newMaxShownEntries++;
        }
        if(newMaxShownEntries == 0)
        {
            newMaxShownEntries = 1;
        }
        maxShownEntries = newMaxShownEntries;
        this->dropDownBounds = sf::RectangleShape(sf::Vector2f(size2.x, size2.y * maxShownEntries));
        dropDownBounds.setPosition(position.x, position.y + size2.y + 2);
        //std::cout << "Max shown entries readjusted" << std::endl;
    }
    dropDownBounds.setFillColor(sf::Color::White);
    scrollBar = new ScrollBar(sf::Vector2f(20, ((size2.y+2) * maxShownEntries)-2), sf::Vector2f(position.x + size2.x - 20, position.y + size2.y + 2), choices.size(), maxShownEntries);
    if(maxShownEntries < choices.size())
    {
        for (int i = 0; i < choices.size(); i++)
        {
            choices[i].changeSize(sf::Vector2f(size2.x - 20-2, size2.y));
        }
       
    }
    clickedOption = 0;
    //isPressed = false;
    pressedButton = nullptr;
    wasLastClicked = false;
}

DropDown::~DropDown()
{
    //delete pressedButton;
    pressedButton = nullptr;
    delete dropDown;
    delete scrollBar;
}

void DropDown::draw(sf::RenderWindow &window)
{
    dropDown->draw(window);
    window.draw(dropDownArrow);
    if(isDropDownOpen)
    {
        window.draw(dropDownBounds);
        if(maxShownEntries >= choices.size())
        {
            this->scrollBar->reset_top();
        } 
        int top = this->scrollBar->getTopPosition();
        int temp = (choices.size() < maxShownEntries + top) ? choices.size() : maxShownEntries + top;
        for(int i = 0 + top; i < temp; i++)
        {
            choices[i].draw(window);
        }
        window.draw(dropDownArrow);
        if(maxShownEntries < choices.size())
        {
            this->scrollBar->draw(window);
        }
    }
}

void DropDown::eventHandler(sf::Event &event)
{
    this->dropDown->eventHandler(event);
    if(this->dropDown->isMouseOver(event) && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        //isPressed = true;
        pressedButton = this->dropDown;
        wasLastClicked = true;
        //isDropDownOpen = !isDropDownOpen;
    } else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && pressedButton != scrollBar)
    {
        if(this->dropDown->isMouseOver(event) && pressedButton == this->dropDown)
        {
            isDropDownOpen = !isDropDownOpen;
            pressedButton = nullptr;
            //isPressed = false;
        } else {
            int top = scrollBar->getTopPosition();
            int temp = (choices.size() < maxShownEntries + top) ? choices.size() : maxShownEntries + top;
            //bool anyOver = false;
            for(int i = 0 + top; i < temp; i++) {
                if(choices[i].isMouseOver(event))
                {
                    //anyOver = true;
                    if(pressedButton == &choices[i])
                    {
                        //isDropDownOpen = false;
                        selectedOption = i;
                        this->dropDown->text.setString(choices[i].text.getString());
                        sf::Vector2f pos = this->dropDown->button.getPosition() + (this->dropDown->button.getSize() / 2.f);
                        this->dropDown->text.setPosition(sf::Vector2f(pos.x - (this->dropDown->button.getSize().y / 2), pos.y));   
                        this->dropDown->text.setOrigin(this->dropDown->text.getGlobalBounds().getSize() / 2.f + this->dropDown->text.getLocalBounds().getPosition());    
                    }
                    
                }
                choices[i].eventHandler(event);
            }
            isDropDownOpen = false;

        }
        pressedButton = nullptr;
    } else if(isDropDownOpen) {
        if(scrollBar->isMouseOverAll(event) || event.type == sf::Event::MouseWheelScrolled || (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && pressedButton == scrollBar)) {
            int top = 0;
            //scrollBar.eventHandler(event, &top);
            if(maxShownEntries < choices.size())
            {
                scrollBar->eventHandler(event, &top);
            } else {
                scrollBar->reset_top();
            }
            if(((event.type == sf::Event::MouseButtonPressed) && event.mouseButton.button == sf::Mouse::Left && scrollBar->isMouseOver(event)))
            {
                pressedButton = scrollBar;
                //scrollTest = true;
                wasLastClicked = true;
            } else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                pressedButton = nullptr;
            } 
            int temp = (choices.size() < maxShownEntries + top) ? choices.size() : maxShownEntries + top;
            for(int i = 0 + top; i < temp; i++)
            {
                choices[i].changePosition(sf::Vector2f(this->dropDown->button.getPosition().x, this->dropDown->button.getPosition().y + ((this->dropDown->button.getSize().y + 2) * (i+1-top))));
                choices[i].eventHandler(event);
            }
        } else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            int top = 0;
            //scrollBar.eventHandler(event, &top);
            if(maxShownEntries < choices.size())
            {
                scrollBar->eventHandler(event, &top);
            } else {
                scrollBar->reset_top();
            }
            int temp = (choices.size() < maxShownEntries + top) ? choices.size() : maxShownEntries + top;
            for(int i = 0 + top; i < temp; i++)
            {
                if(choices[i].isMouseOver(event))
                {
                    pressedButton = &choices[i];
                    clickedOption = i;
                    //choices[i].eventHandler(event);
                }
                choices[i].eventHandler(event);
            }
            wasLastClicked = true;
            
        } else if(event.type == sf::Event::MouseMoved) {
            int top = 0;
            scrollBar->eventHandler(event, &top); 
            if(pressedButton == nullptr)
            {
                //int top = scrollBar.getTopPosition();
                int temp = (choices.size() < maxShownEntries + top) ? choices.size() : maxShownEntries + top;
                for(int i = 0 + top; i < temp; i++)
                {
                    //choices[i].changePosition(sf::Vector2f(dropDown.button.getPosition().x, dropDown.button.getPosition().y + ((dropDown.button.getSize().y + 2) * (i+1-top))));
                    choices[i].eventHandler(event);
                }
                //std::cout << "Handling" << std::endl;
            }
            else {
                int temp = (choices.size() < maxShownEntries + top) ? choices.size() : maxShownEntries + top;
                for(int i = 0 + top; i < temp; i++)
                {
                    choices[i].changePosition(sf::Vector2f(this->dropDown->button.getPosition().x, this->dropDown->button.getPosition().y + ((this->dropDown->button.getSize().y + 2) * (i+1-top))));
                    choices[i].eventHandler(event);
                }
                //std::cout << "Other Handling" << std::endl;
            }
        } else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && pressedButton == scrollBar) {
            pressedButton = nullptr;
        }
    } else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if(!isMouseOverAny(event)) {
            wasLastClicked = false;
        }
    }

}

bool DropDown::isMouseOverAny(sf::Event &event)
{
    if(dropDown->isMouseOver(event))
    {
        return true;
    }
    if(isDropDownOpen)
    {
        if((event.type == sf::Event::MouseButtonPressed) || (event.type == sf::Event::MouseButtonReleased))
        {
            if((event.mouseButton.x > dropDownBounds.getPosition().x && event.mouseButton.x < dropDownBounds.getPosition().x + dropDownBounds.getSize().x) && (event.mouseButton.y > dropDownBounds.getPosition().y && event.mouseButton.y < dropDownBounds.getPosition().y + dropDownBounds.getSize().y))
            {
                return true;
            }
        }
        if((event.mouseMove.x > dropDownBounds.getPosition().x && event.mouseMove.x < dropDownBounds.getPosition().x + dropDownBounds.getSize().x) && (event.mouseMove.y > dropDownBounds.getPosition().y && event.mouseMove.y < dropDownBounds.getPosition().y + dropDownBounds.getSize().y))
        {
            return true;
        }
        //dropDownBounds
        if(scrollBar->isMouseOverAll(event))
        {
            return true;
        }
    }
    return false;
}

void DropDown::manualClose()
{
    isDropDownOpen = false;
    pressedButton = nullptr;
}

sf::ConvexShape DropDown::createDropDownArrow(sf::Vector2f position, sf::Vector2f size)
{
    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setFillColor(sf::Color::Black);
    float topLeft = position.x + size.x - size.y;
    
    triangle.setPoint(0, sf::Vector2f(topLeft+(size.y/4.f), position.y+(size.y/4.f)));
    triangle.setPoint(1, sf::Vector2f(topLeft+(3.f * size.y/4.f), position.y+(size.y/4.f)));
    triangle.setPoint(2, sf::Vector2f(topLeft+(size.y/2.f), position.y+(3.f * size.y/4.f)));
    return triangle;
}

void DropDown::ResizeDropDown(sf::FloatRect textRect, sf::FloatRect textRectLocal, sf::Vector2f buttonSize, sf::Vector2f buttonPos, Button* drop)
{
    if(textRect.width + 20 + buttonSize.y > buttonSize.x) {
        drop->button.setSize(sf::Vector2f(textRect.width + 20 + buttonSize.y, buttonSize.y));
        //text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
        sf::Vector2f pos = buttonPos + (buttonSize / 2.f);
        drop->text.setPosition(sf::Vector2f(pos.x - (buttonSize.y / 2), pos.y));
        drop->text.setOrigin(textRect.getSize() / 2.f + textRectLocal.getPosition());
        //std::cout << "Adjusting X" << std::endl;
    }
    if(textRect.height + 10 > buttonSize.y) {
        drop->button.setSize(sf::Vector2f(buttonSize.x, textRect.height + 10));
        //text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
        sf::Vector2f pos = buttonPos + (buttonSize / 2.f);
        drop->text.setPosition(sf::Vector2f(pos.x - (buttonSize.y / 2), pos.y));  
        drop->text.setOrigin(textRect.getSize() / 2.f + textRectLocal.getPosition());  
        //std::cout << "Adjusting Y" << std::endl;
    } 
    if(!(textRect.width + 10 + buttonSize.y > buttonSize.x) && !(textRect.height + 10 > buttonSize.y)) {
        sf::Vector2f pos = buttonPos + (buttonSize / 2.f);
        drop->text.setPosition(sf::Vector2f(pos.x - (buttonSize.y / 2), pos.y));   
        drop->text.setOrigin(textRect.getSize() / 2.f + textRectLocal.getPosition()); 
        //std::cout << "Adjusting Neither" << std::endl;
    }
}

