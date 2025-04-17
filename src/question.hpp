#ifndef QUESTION_HPP
#define QUESTION_HPP

#include "button.hpp"
#include "radio_button.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

#define MULTIPLE_CHOICE 4
#define TRUE_FALSE 2

/*
enum QuestionType {
    MULTIPLE_CHOICE = 4,
    TRUE_FALSE = 2,
    UNKNOWN_TYPE = 0
};

enum QuestionDifficulty {
    EASY,
    MEDIUM,
    HARD,
    UNKNOWN
};*/

struct QuestionData {
    int type;
    string difficulty;
    string category;
    string question;
    int correctAnswer;
    vector<string> choices;
};

struct QuizData {
    int responseCode;
    int questionCount;
    vector<QuestionData> questions;
};

struct CategoryData {
    int id;
    string name;
};

struct CategoryJsonData {
    vector<CategoryData> categories;
};

enum QuizState {
    QUIZ_SELECTION,
    QUIZ_CONFIRMATION,
    QUIZ_IN_PROGRESS,
    QUIZ_COMPLETED
};

void QuizToString(QuizData *quiz);
void ConvertHTMLCodes(string* str);
vector<string> GetCategoriesString(CategoryJsonData *categories);

class QuestionDisplay {
    public:
        //QuestionDisplay();
        QuestionDisplay(QuestionData questionData, sf::Font &font, sf::Vector2f position, sf::Vector2f size);
        ~QuestionDisplay();
        bool isAnySelected();
        bool submitAnswer();
        void resetQuestionData(QuestionData questionData, int numQuestion);
        void eventHandler(sf::Event &event);
        void draw(sf::RenderWindow &window);
        sf::Vector2f getSize() { return size; }
        sf::Vector2f getPosition() { return sf::Vector2f(categoryText.getPosition().x, categoryText.getPosition().y - (categoryText.getGlobalBounds().getPosition().y / 2.f)); }
    protected:
    private:
        int numQuestion;
        int correctAnswer;
        sf::Vector2f size;
        //QuestionData questionData;
        sf::Text categoryText;
        sf::Text difficultyText;
        std::vector<sf::Text> questionText;
        RadioButtonSet *radioButtons;
        bool allowAnswerChange;
        sf::Font font;
        //Button confirmButton;
        //sf::Text correctText;
        //sf::Text incorrectText;
};


#endif // QUESTION_HPP