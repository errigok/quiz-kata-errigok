#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <fstream>
#include <codecvt>
#include <random>
#include "json.hpp"
#include "question.hpp"
#include "button.hpp"
#include "scroll_bar.hpp"
#include "drop_down.hpp"
#include "number_text_box.hpp"
#include "radio_button.hpp"
using json = nlohmann::json;

using namespace std;

int gotData = 0;
QuizState quizState = QUIZ_SELECTION;

int numCorrect = 0;
int currQuestion = 0;
bool readyForNext = false;

json GetQuizDataWeb(std::string url);

size_t CurlWrite(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size*nmemb;
    try
    {
        s->append((char*)contents, newLength);
    }
    catch(std::bad_alloc &e)
    {
        //handle memory problem
        return 0;
    }
    return newLength;
}

void from_json(const json& j, QuestionData& q)
{
    string type = j.at("type").get<string>();
    if(!type.compare("multiple"))
    {
        q.type = MULTIPLE_CHOICE;
    }
    else if(!type.compare("boolean"))
    {
        q.type = TRUE_FALSE;
    } else {
        q.type = 0;
    }
    j.at("difficulty").get_to(q.difficulty);
    j.at("category").get_to(q.category);
    j.at("question").get_to(q.question);
    string correctAnswer = j.at("correct_answer").get<string>();
    j.at("incorrect_answers").get_to(q.choices);
    shuffle(q.choices.begin(), q.choices.end(), std::mt19937(std::random_device()()));
    q.correctAnswer = std::rand() % q.type;
    q.choices.insert(q.choices.begin() + q.correctAnswer, correctAnswer);

    for(int i = 0; i < q.choices.size(); i++)
    {
        ConvertHTMLCodes(&q.choices[i]);
    }
    ConvertHTMLCodes(&q.question);
    ConvertHTMLCodes(&q.category);
}
void from_json(const json& j, QuizData& q)
{
    j.at("response_code").get_to(q.responseCode);
    if(q.responseCode == 0)
    {
        j.at("results").get_to(q.questions);
        q.questionCount = q.questions.size();
    } else
    {
        q.questionCount = 0;
    }
    
}

void from_json(const json& j, CategoryData& q)
{
    j.at("id").get_to(q.id);
    j.at("name").get_to(q.name);
}

void from_json(const json& j, CategoryJsonData& q)
{
    j.at("trivia_categories").get_to(q.categories);
}

void ChangeGameState(std::vector<std::any>* data) {
    if(data->size() < 1) {
        return;
    }
    try {
        QuizState state = std::any_cast<QuizState>((*data)[0]);
        quizState = state;
        //std::cout << "Quiz state changed to: " << state << std::endl;
    } catch(const std::bad_any_cast& e) {
        std::cout << "Error casting QuizState: " << e.what() << std::endl;
        return;
    }
}

void SubmitQuestionHandler(std::vector<std::any>* data) {
    if(data->size() < 1) {
        return;
    }
    QuestionDisplay *questionDisplay = std::any_cast<QuestionDisplay*>((*data)[0]);
    sf::Text *answerCorrectText = std::any_cast<sf::Text*>((*data)[1]);
    sf::Text *correctCountText = std::any_cast<sf::Text*>((*data)[2]);
    bool wasCorrect = questionDisplay->submitAnswer();
    if(wasCorrect)
    {
        numCorrect++;
        answerCorrectText->setString("Correct!");
        answerCorrectText->setFillColor(sf::Color::Green);
        answerCorrectText->setOrigin(answerCorrectText->getGlobalBounds().width / 2, answerCorrectText->getGlobalBounds().height / 2);
        answerCorrectText->setPosition(sf::Vector2f(500, questionDisplay->getPosition().y + questionDisplay->getSize().y + 50));
    } else {
        answerCorrectText->setString("Incorrect!");
        answerCorrectText->setFillColor(sf::Color::Red);
        answerCorrectText->setOrigin(answerCorrectText->getGlobalBounds().width / 2, answerCorrectText->getGlobalBounds().height / 2);
        answerCorrectText->setPosition(sf::Vector2f(500, questionDisplay->getPosition().y + questionDisplay->getSize().y + 50));
    }
    readyForNext = true;
    currQuestion++;
    correctCountText->setString("Correct: " + std::to_string(numCorrect) + "/" + std::to_string(currQuestion));
}

void NextQuestionHandler(std::vector<std::any>* data) {
    if(data->size() < 1) {
        return;
    }
    QuestionDisplay *questionDisplay = std::any_cast<QuestionDisplay*>((*data)[0]);
    //sf::Text *correctCountText = std::any_cast<sf::Text*>((*data)[2]);
    QuizData *quizData = std::any_cast<QuizData*>((*data)[3]);
    readyForNext = false;
    if(currQuestion < quizData->questionCount) {
        QuestionData nextQuestion = quizData->questions[currQuestion];
        questionDisplay->resetQuestionData(nextQuestion, currQuestion);
    } else {
        quizState = QUIZ_COMPLETED;
    }
}

void CreateAPIString(std::vector<std::any>* data) {
    if(data->size() < 5) {
        data->push_back("-1");
        return;
    }
    NumberTextBox *questionCount = std::any_cast<NumberTextBox*>((*data)[0]);
    int numQuestions = questionCount->getNumber();
    if(numQuestions == -1) {
        data->push_back("-2");
        return;
    } else if(numQuestions < 1) {
        data->push_back("-3");
        return;
    } else if(numQuestions > 50) {
        numQuestions = 50;
    }
    DropDown *categoryDropDown = std::any_cast<DropDown*>((*data)[1]);
    std::string category = categoryDropDown->getSelectedOptionText();
    // std::string category = std::any_cast<std::string>((*data)[1]);
    vector<CategoryData> categories = std::any_cast<std::vector<CategoryData>>((*data)[2]);
    int categoryId = -1;
    if(category.compare("Any Category") != 0)
    {
        for(int i = 0; i < categories.size(); i++)
        {
            if(categories[i].name.compare(category) == 0)
            {
                categoryId = categories[i].id;
            }
        }
    }

    DropDown *difficultyDropDown = std::any_cast<DropDown*>((*data)[3]);
    std::string difficulty = difficultyDropDown->getSelectedOptionText();
    // std::string difficulty = std::any_cast<std::string>((*data)[3]);

    DropDown *typeDropDown = std::any_cast<DropDown*>((*data)[4]);
    std::string type = typeDropDown->getSelectedOptionText();
    // std::string type = std::any_cast<std::string>((*data)[4]);

    std::string url = "https://opentdb.com/api.php?amount=" + std::to_string(numQuestions);

    if(categoryId != -1)
    {
        url += "&category=" + std::to_string(categoryId);
    }
    if(difficulty.compare("Any Difficulty") != 0)
    {
        std::transform(difficulty.begin(), difficulty.end(), difficulty.begin(), [](unsigned char c){ return std::tolower(c); } );
        url += "&difficulty=" + difficulty;
    }
    if(type.compare("Multiple Choice") == 0)
    {
        url += "&type=multiple";
    } else if(type.compare("True/False") == 0)
    {
        url += "&type=boolean";
    }
    //std::cout << "URL: " << url << std::endl;
    //data->push_back(url);
    json jdata = GetQuizDataWeb(url);
    if(jdata.empty())
    {
        std::cout << "Error getting data from API" << std::endl;
        data->push_back("-4");
        return;
    }
    QuizData quiz = jdata.get<QuizData>();
    data->push_back(quiz);
    gotData++;
}

json GetQuizDataWeb(std::string url)
{
    CURL *curl;
    CURLcode res;
    std::string s;
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    
        /* Perform the request, res gets the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
    
        /* always cleanup */
        curl_easy_cleanup(curl);
    } else {
        return json();
    }
    //std::cout << s << std::endl;
    json data = json::parse(s);
    return data;
}

int main()
{
    json data = GetQuizDataWeb("https://opentdb.com/api_category.php");

    CategoryJsonData categories = data.get<CategoryJsonData>();
    for(int i = 0; i < categories.categories.size(); i++)
    {
        ConvertHTMLCodes(&categories.categories[i].name);
    }
    
    sf::RenderWindow window(sf::VideoMode(1000, 1200), "Quiz");
    sf::Font font;
    if (!font.loadFromFile(DEFAULT_FONT))
    {
        return 1;
    }
    
    vector<string> confirmation = {"Begin quiz with these settings?", "Number of questions: ", "Category: ", "Difficulty: ", "Type: "};
    sf::Text selectText[4];
    for(int i = 0; i < 4; i++)
    {
        selectText[i].setFont(font);
        selectText[i].setCharacterSize(20);
        selectText[i].setFillColor(sf::Color::Black);
        selectText[i].setPosition(100, 65 + (i * 100));
        selectText[i].setString(confirmation[i+1]);
    }

    NumberTextBox questionCount(sf::Vector2f(200, 50), sf::Vector2f(100, 100), COLOR_RELEASE, COLOR_CLICK, font);

    vector<string> categoriesString = GetCategoriesString(&categories);
    categoriesString.insert(categoriesString.begin(), "Any Category");

    sf::Text categoryText(categoriesString[0], font, 24);
    DropDown categoryDropDown(sf::Vector2f(200, 50), sf::Vector2f(100, 200), COLOR_RELEASE, COLOR_CLICK, font, categoriesString, 0, window.getSize(), 4, categoryText);

    vector<string> difficulties = {"Any Difficulty", "Easy", "Medium", "Hard"};
    sf::Text difficultyText(difficulties[0], font, 24);
    DropDown difficultyDropDown(sf::Vector2f(200, 50), sf::Vector2f(100, 300), COLOR_RELEASE, COLOR_CLICK, font, difficulties, 0, window.getSize(), 4, difficultyText);
    
    vector<string> types = {"Any Type", "Multiple Choice", "True/False"};
    sf::Text typeText(types[0], font, 24);
    DropDown typeDropDown(sf::Vector2f(200, 50), sf::Vector2f(100, 400), COLOR_RELEASE, COLOR_CLICK, font, types, 0, window.getSize(), 4, typeText);
    Button startButton(sf::Vector2f(200, 50), sf::Vector2f(100, 600), COLOR_RELEASE, COLOR_CLICK, font, "Start");

    vector<std::any> callbackDataVec;
    callbackDataVec.push_back(&questionCount);
    callbackDataVec.push_back(&categoryDropDown);
    callbackDataVec.push_back(categories.categories);
    callbackDataVec.push_back(&difficultyDropDown);
    callbackDataVec.push_back(&typeDropDown);
    
    startButton.setEventHandlerCallback(&CreateAPIString, &callbackDataVec);

    QuizData* quiz = nullptr;

    sf::Text messageText;
    messageText.setFont(font);
    messageText.setCharacterSize(20);
    messageText.setFillColor(sf::Color::Black);
    messageText.setPosition(100, 700);
    bool showMessage = false;

    Button beginQuizButton(sf::Vector2f(300, 50), sf::Vector2f(100, 750), COLOR_RELEASE, COLOR_CLICK, font, "Begin Quiz");
    vector<std::any> beginQuizData;
    beginQuizData.push_back(QUIZ_IN_PROGRESS);
    beginQuizButton.setEventHandlerCallback(&ChangeGameState, &beginQuizData);

    Button beginQuizBackButton(sf::Vector2f(300, 50), sf::Vector2f(500, 750), COLOR_RELEASE, COLOR_CLICK, font, "Back");
    vector<std::any> beginQuizBackData;
    beginQuizBackData.push_back(QUIZ_SELECTION);
    beginQuizBackButton.setEventHandlerCallback(&ChangeGameState, &beginQuizBackData);

    sf::Text confirmText[5];
    for(int i = 0; i < 5; i++)
    {
        confirmText[i].setFont(font);
        confirmText[i].setCharacterSize(24);
        confirmText[i].setFillColor(sf::Color::Black);
        confirmText[i].setString(confirmation[i]);
        confirmText[i].setOrigin(confirmText[i].getGlobalBounds().getSize() / 2.f + confirmText[i].getLocalBounds().getPosition());
        confirmText[i].setPosition(500, 65 + (i * 100));
    }

    QuestionDisplay* questionDisplay;
    sf::Text correctCountText;
    correctCountText.setFont(font);
    correctCountText.setCharacterSize(20);
    correctCountText.setFillColor(sf::Color::Black);
    correctCountText.setPosition(10, 10);

    Button confirmAnswer(sf::Vector2f(200, 50), sf::Vector2f(200, 800), COLOR_RELEASE, COLOR_CLICK, font, "Confirm");
    confirmAnswer.setEventHandlerCallback(&SubmitQuestionHandler, &callbackDataVec);

    Button nextQuestion(sf::Vector2f(200, 50), sf::Vector2f(700, 900), COLOR_RELEASE, COLOR_CLICK, font, "Next");
    nextQuestion.setEventHandlerCallback(&NextQuestionHandler, &callbackDataVec);

    sf::Text answerCorrectText;
    answerCorrectText.setFont(font);
    answerCorrectText.setCharacterSize(25);

    sf::Text score2;
    score2.setFont(font);
    score2.setCharacterSize(55);
    score2.setFillColor(sf::Color::Black);

    int topPos = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                callbackDataVec.clear();
                beginQuizData.clear();
                beginQuizBackData.clear();

                window.close();
                if(quiz != nullptr) {
                    delete quiz;
                }
                if(questionDisplay != nullptr) {
                    delete questionDisplay;
                }
                return 0;
            }
            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
            bool stopHandle = false;

            if(quizState == QUIZ_SELECTION) {
                questionCount.eventHandler(event);

                categoryDropDown.eventHandler(event);
                stopHandle = categoryDropDown.isMouseOverAny(event);
                if(stopHandle) {
                    if(categoryDropDown.wasThisLastClicked()) {
                        difficultyDropDown.manualClose();
                        typeDropDown.manualClose();
                    }
                    break;
                }

                difficultyDropDown.eventHandler(event);
                stopHandle = difficultyDropDown.isMouseOverAny(event);
                if(stopHandle) {
                    if(difficultyDropDown.wasThisLastClicked()) {
                        typeDropDown.manualClose();
                    }
                    break;
                }

                typeDropDown.eventHandler(event);
                stopHandle = typeDropDown.isMouseOverAny(event);
                if(stopHandle) {
                    break;
                }

                startButton.eventHandler(event);
                if((gotData == 1) && (callbackDataVec.size() > 5)) {
                    try {
                        if(quiz != nullptr) {
                            delete quiz;
                            quiz = nullptr;
                        }
                        quiz = new QuizData(std::any_cast<QuizData>((callbackDataVec[5])));
                    }
                    catch (const std::bad_any_cast& e) {
                        std::cout << "Error casting to QuizData: " << e.what() << std::endl;
                        break;
                    }
                    if(quiz->responseCode == 1) {
                        showMessage = true;
                        messageText.setString("Invalid parameters - not enough questions for the specified categories");
                    } else if(quiz->responseCode == 5) {
                        showMessage = true;
                        messageText.setString("You must wait 5 seconds between requests");
                    } else if(quiz->responseCode == 0) {
                        showMessage = false;
                        messageText.setString("");
                        vector<std::any> tempState;
                        tempState.push_back(QUIZ_CONFIRMATION);
                        ChangeGameState(&tempState);
                        confirmText[1].setString(confirmation[1] + questionCount.getNumberString());
                        confirmText[2].setString(confirmation[2] + categoryDropDown.getSelectedOptionText());
                        confirmText[3].setString(confirmation[3] + difficultyDropDown.getSelectedOptionText());
                        confirmText[4].setString(confirmation[4] + typeDropDown.getSelectedOptionText());
                        for(int i = 1; i < 5; i++)
                        {
                            confirmText[i].setOrigin(confirmText[i].getGlobalBounds().getSize() / 2.f + confirmText[i].getLocalBounds().getPosition());
                            confirmText[i].setPosition(500, 65 + (i * 100));
                        }
                    }
                    //gotData = false;
                }
                while(callbackDataVec.size() > 5) {
                    callbackDataVec.pop_back();
                }
            }
            else if(quizState == QUIZ_CONFIRMATION) {
                beginQuizButton.eventHandler(event);
                beginQuizBackButton.eventHandler(event);
                currQuestion = 0;
                if(questionDisplay != nullptr) {
                    delete questionDisplay;
                    questionDisplay = nullptr;
                }
                questionDisplay = new QuestionDisplay(quiz->questions[currQuestion], font, sf::Vector2f(50, 50), sf::Vector2f(850, 700));
                correctCountText.setString("Correct: 0/0");
                readyForNext = false;
                if(quizState == QUIZ_IN_PROGRESS) {
                    callbackDataVec.clear();
                    callbackDataVec.push_back(questionDisplay);
                    callbackDataVec.push_back(&answerCorrectText);
                    callbackDataVec.push_back(&correctCountText);
                    callbackDataVec.push_back(quiz);
                }
            } else if(quizState == QUIZ_IN_PROGRESS) {
                questionDisplay->eventHandler(event);
                if(!readyForNext) {
                    if(questionDisplay->isAnySelected()) {
                        confirmAnswer.eventHandler(event);
                    }
                } else {
                    nextQuestion.eventHandler(event);
                }
            } else if(quizState == QUIZ_COMPLETED) {
                correctCountText.setString("Your score: " + std::to_string(numCorrect) + "/" + std::to_string(currQuestion));
                correctCountText.setOrigin(correctCountText.getGlobalBounds().width / 2, correctCountText.getGlobalBounds().height / 2);
                correctCountText.setPosition(sf::Vector2f(500, 500));
                correctCountText.setCharacterSize(50);

                answerCorrectText.setString("Quiz completed!");
                answerCorrectText.setFillColor(sf::Color::Black);
                answerCorrectText.setOrigin(answerCorrectText.getGlobalBounds().width / 2, answerCorrectText.getGlobalBounds().height / 2);
                answerCorrectText.setPosition(sf::Vector2f(500, 300));
                answerCorrectText.setCharacterSize(44);

                score2.setString(std::to_string((int)(((numCorrect*1.f)/currQuestion) * 100)) + "%");
                score2.setOrigin(score2.getGlobalBounds().width / 2, score2.getGlobalBounds().height / 2);
                score2.setPosition(sf::Vector2f(500, 650));
            }
            //beginQuizButton.eventHandler(event);
        }

        // clear the window with black color
        window.clear(sf::Color::White);

        // draw everything here...
       if(quizState == QUIZ_SELECTION) {
            for(sf::Text t : selectText) {
                window.draw(t);
            }
            startButton.draw(window);
            typeDropDown.draw(window);
            difficultyDropDown.draw(window);
            categoryDropDown.draw(window);
            questionCount.draw(window);
            
            if(showMessage) {
                window.draw(messageText);
            }

            gotData = 0;
        }
        else if(quizState == QUIZ_CONFIRMATION) {
            beginQuizButton.draw(window);
            beginQuizBackButton.draw(window);
            for(int i = 0; i < 5; i++)
            {
                window.draw(confirmText[i]);
            }
        } else if(quizState == QUIZ_IN_PROGRESS) {
            questionDisplay->draw(window);
            window.draw(correctCountText);
            if(readyForNext) {
                nextQuestion.draw(window);
                window.draw(answerCorrectText);
            } else if(questionDisplay->isAnySelected()) {
                confirmAnswer.draw(window);
            }
        } else if(quizState == QUIZ_COMPLETED) {
            window.draw(correctCountText);
            window.draw(answerCorrectText);
            window.draw(score2);
        }

        // end the current frame
        window.display();
    }

    return 0;
}
