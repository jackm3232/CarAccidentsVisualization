#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

class MenuButton{
private:
    sf::Font font;
    sf::Text buttonText;
    sf::Sprite buttonSprite;

public:
    std::string text;
    MenuButton(std::string text, int width, int height);
    sf::Sprite getSprite();
    sf::Text getText();
    void setSpritePos(int x,int y);
    void setTextPos(int x, int y);
    void setSpriteColor(sf::Color color);
};

class SelectionButton : public MenuButton{
private:
    sf::Font font;
    sf::Text buttonText;
    sf::Sprite buttonSprite;

public:
    bool selected;
    std::string text;
    SelectionButton(std::string inputText, int width, int height);
};

class Window{
private:
    sf::Font font;
    std::string city;
    std::string month;
    int structSelect;
    sf::RenderWindow* window;

    void updateCity();
    void runSettings();
    void loadButtons();
    void updateMain(int j);
    void insertDot(float y, float x);
    void cityMenu(std::string selection);
    void updateSettings(sf::RenderWindow& settingsWindow);

public:
    std::vector<MenuButton*> menuButtons;
    std::vector<SelectionButton*> selectionButtons;
    std::map<std::string, sf::RectangleShape*> maps;
    std::vector<sf::CircleShape*> dots;

    void mainMenu();
    sf::RenderWindow* getWindow();

    Window();
};
