#include "question.hpp"
// #include <cstdlib>
// #include <algorithm>

void QuizToString(QuizData *quiz)
{
    cout << "Response Code: " << quiz->responseCode << endl;
    cout << "Question Count: " << quiz->questionCount << endl;
    for (int i = 0; i < quiz->questionCount; i++)
    {
        cout << "Question " << i + 1 << endl;
        switch(quiz->questions[i].type)
        {
            case MULTIPLE_CHOICE:
                cout << "Type: Multiple Choice" << endl;
                break;
            case TRUE_FALSE:
                cout << "Type: True/False" << endl;
                break;
            default:
                cout << "Type: Unknown" << endl;
                break;
        }
        //cout << "Type: " << quiz->questions[i].type << endl;
        cout << "Difficulty: " << quiz->questions[i].difficulty << endl;
        cout << "Category: " << quiz->questions[i].category << endl;
        cout << "Question: " << quiz->questions[i].question << endl;
        cout << "Correct Answer: Answer #" << quiz->questions[i].correctAnswer+1 << ": " << quiz->questions[i].choices[quiz->questions[i].correctAnswer] << endl;
        cout << "Choices:" << endl;
        for (int j = 0; j < quiz->questions[i].choices.size(); j++)
        {
            cout << "    " << quiz->questions[i].choices[j] << endl;
        }
        cout << endl;
    }
}

void ConvertHTMLCodes(string* str) {
    for(int i = 0; i < str->size(); i++) {
        if((*str)[i] == '&') {
            if(str->substr(i, 5) == "&amp;") {
                (*str)[i] = '&';
                str->erase(i + 1, 4);
            } else if(str->substr(i, 4) == "&lt;") {
                (*str)[i] = '<';
                str->erase(i + 1, 3);
            } else if(str->substr(i, 4) == "&gt;") {
                (*str)[i] = '>';
                str->erase(i + 1, 3);
            } else if(str->substr(i, 6) == "&quot;") {
                (*str)[i] = '"';
                str->erase(i + 1, 5);
            } else if(str->substr(i, 6) == "&apos;") {
                (*str)[i] = '\'';
                str->erase(i + 1, 5);
            } else if((str->substr(i, 7) == "&rsquo;") || (str->substr(i, 7) == "&lsquo;")) {
                (*str)[i] = '\'';
                str->erase(i + 1, 6);
            } else if((str->substr(1, 7) == "&rdquo;") || (str->substr(1, 7) == "&ldquo;")) {
                (*str)[i] = '\"';
                str->erase(i + 1, 6);
            } else if((*str)[i+1] == '#') {
                int j = i + 2;
                while((*str)[j] != ';') {
                    j++;
                }
                string code = str->substr(i + 2, j - i - 2);
                if(code[0] == 'x') {
                    code = code.substr(1);
                    int value = stoi(code, nullptr, 16);
                    (*str)[i] = value;
                    str->erase(i + 1, j - i);
                } else {
                    int value = stoi(code);
                    (*str)[i] = value;
                    str->erase(i + 1, j - i);
                }
            }
        }
    }
}

vector<string> GetCategoriesString(CategoryJsonData* categories)
{
    vector<string> categoryNames;
    for(CategoryData category : categories->categories)
    {
        categoryNames.push_back(category.name);
    }
    return categoryNames;
}

QuestionDisplay::QuestionDisplay(QuestionData questionData, sf::Font &font, sf::Vector2f position, sf::Vector2f size)
{
    //this->questionData = questionData;
    this->font = font;
    numQuestion = 0;
    categoryText.setFont(font);
    categoryText.setCharacterSize(20);
    categoryText.setFillColor(sf::Color::Black);
    categoryText.setString("Category: " + questionData.category);
    categoryText.setOrigin(sf::Vector2f(categoryText.getLocalBounds().getPosition().x, categoryText.getGlobalBounds().getSize().y / 2.f + categoryText.getLocalBounds().getPosition().y));
    categoryText.setPosition(position.x + 10, position.y + 15);

    difficultyText.setFont(font);
    difficultyText.setCharacterSize(20);
    difficultyText.setFillColor(sf::Color::Black);
    difficultyText.setString("Difficulty: " + questionData.difficulty);
    difficultyText.setOrigin(sf::Vector2f(difficultyText.getLocalBounds().getPosition().x, difficultyText.getGlobalBounds().getSize().y / 2.f + difficultyText.getLocalBounds().getPosition().y));
    difficultyText.setPosition(position.x + 10, categoryText.getPosition().y + categoryText.getGlobalBounds().getSize().y + 5);

    sf::Text qText;

    qText.setFont(font);
    qText.setCharacterSize(24);
    qText.setFillColor(sf::Color::Black);
    std::string full = "Question #" + std::to_string(numQuestion + 1) + ": " + questionData.question;
    qText.setString(full);
    qText.setOrigin(sf::Vector2f(qText.getLocalBounds().getPosition()));
    qText.setPosition(position.x + 10, difficultyText.getPosition().y + difficultyText.getGlobalBounds().getSize().y + 5);
    int line = 0;
    while(qText.getGlobalBounds().width > size.x - 10) {
        int max_strlen = ((size.x - 10) / qText.getGlobalBounds().width) * full.length();
        while(max_strlen > 0 && full[max_strlen] != ' ') {
            max_strlen--;
        }
        if(max_strlen == 0) {
            break;
        }
        bool newLine = false;
        std::string str = qText.getString().substring(0, max_strlen);
        full = str;
        std::string str2;
        if(max_strlen < qText.getString().getSize()) {
            str2 = qText.getString().substring(max_strlen+1, qText.getString().getSize());
            newLine = true;
        }
        
        //std::string str2 = qText.getString().substring(max_strlen+1, qText.getString().getSize());
        qText.setString(str);
        qText.setOrigin(sf::Vector2f(qText.getLocalBounds().getPosition()));
        qText.setPosition(position.x + 10, difficultyText.getPosition().y + difficultyText.getGlobalBounds().getSize().y + 5 + line * (qText.getGlobalBounds().getSize().y + 5));
        questionText.push_back(qText);
        line++;

        if(newLine) {
            qText.setString(str2);
            qText.setOrigin(sf::Vector2f(qText.getLocalBounds().getPosition()));
            qText.setPosition(position.x + 10, difficultyText.getPosition().y + difficultyText.getGlobalBounds().getSize().y + 5 + line * (qText.getGlobalBounds().getSize().y + 5));
            full = str2;
            line++;
        }
    }
    questionText.push_back(qText);

    radioButtons = new RadioButtonSet(sf::Vector2f(size.x, size.y - (questionText.size() * (difficultyText.getGlobalBounds().getSize().y + 5)) - difficultyText.getGlobalBounds().getSize().y - categoryText.getGlobalBounds().getSize().y - 30), sf::Vector2f(position.x + 10, questionText[questionText.size()-1].getPosition().y + difficultyText.getGlobalBounds().getSize().y + 5), COLOR_RELEASE, COLOR_CLICK, sf::Color::Blue, questionData.choices, font);
    allowAnswerChange = true;
    correctAnswer = questionData.correctAnswer;
    this->size = size;
}

QuestionDisplay::~QuestionDisplay()
{
    delete radioButtons;
}

bool QuestionDisplay::isAnySelected()
{
    int sel = radioButtons->getSelectedButtonIndex();
    if(sel == -1)
    {
        return false;
    }
    return true;
}

bool QuestionDisplay::submitAnswer()
{
    allowAnswerChange = false;
    int sel = radioButtons->getSelectedButtonIndex();
    radioButtons->reveal_correct_answer(correctAnswer);
    return (sel == correctAnswer);
}

void QuestionDisplay::resetQuestionData(QuestionData questionData, int numQuestion)
{
    delete radioButtons;
    radioButtons = nullptr;
    this->numQuestion = numQuestion;
    categoryText.setString("Category: " + questionData.category);
    difficultyText.setString("Difficulty: " + questionData.difficulty);
    questionText.clear();
    
    sf::Text qText;

    qText.setFont(font);
    qText.setCharacterSize(24);
    qText.setFillColor(sf::Color::Black);
    std::string full = "Question #" + std::to_string(numQuestion + 1) + ": " + questionData.question;
    qText.setString(full);
    qText.setOrigin(sf::Vector2f(qText.getLocalBounds().getPosition()));
    qText.setPosition(difficultyText.getPosition().x + 10, difficultyText.getPosition().y + difficultyText.getGlobalBounds().getSize().y + 5);
    int line = 0;
    while(qText.getGlobalBounds().width > size.x - 10) {
        int max_strlen = ((size.x - 10) / qText.getGlobalBounds().width) * full.length();
        while(max_strlen > 0 && full[max_strlen] != ' ') {
            max_strlen--;
        }
        if(max_strlen == 0) {
            break;
        }
        bool newLine = false;
        std::string str = qText.getString().substring(0, max_strlen);
        full = str;
        std::string str2;
        if(max_strlen < qText.getString().getSize()) {
            str2 = qText.getString().substring(max_strlen+1, qText.getString().getSize());
            newLine = true;
        }
        
        qText.setString(str);
        qText.setOrigin(sf::Vector2f(qText.getLocalBounds().getPosition()));
        qText.setPosition(difficultyText.getPosition().x + 10, difficultyText.getPosition().y + difficultyText.getGlobalBounds().getSize().y + 5 + line * (qText.getGlobalBounds().getSize().y + 5));
        questionText.push_back(qText);
        line++;

        if(newLine) {
            qText.setString(str2);
            qText.setOrigin(sf::Vector2f(qText.getLocalBounds().getPosition()));
            qText.setPosition(difficultyText.getPosition().x + 10, difficultyText.getPosition().y + difficultyText.getGlobalBounds().getSize().y + 5 + line * (qText.getGlobalBounds().getSize().y + 5));
            full = str2;
            line++;
        }
    }
    questionText.push_back(qText);

    radioButtons = new RadioButtonSet(sf::Vector2f(size.x, size.y - (questionText.size() * (difficultyText.getGlobalBounds().getSize().y + 5)) - difficultyText.getGlobalBounds().getSize().y - categoryText.getGlobalBounds().getSize().y - 30), sf::Vector2f(difficultyText.getPosition().x + 10, questionText[questionText.size()-1].getPosition().y + difficultyText.getGlobalBounds().getSize().y + 5), COLOR_RELEASE, COLOR_CLICK, sf::Color::Blue, questionData.choices, font);
    
    allowAnswerChange = true;
    correctAnswer = questionData.correctAnswer;
}

void QuestionDisplay::eventHandler(sf::Event &event)
{
    if(allowAnswerChange)
    {
        radioButtons->eventHandler(event);
    }
}

void QuestionDisplay::draw(sf::RenderWindow &window)
{
    window.draw(categoryText);
    window.draw(difficultyText);
    //window.draw(questionText);
    for(int i = 0; i < questionText.size(); i++)
    {
        window.draw(questionText[i]);
    }
    radioButtons->draw(window);
}
