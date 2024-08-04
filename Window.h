#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "ordered_map.h"

class Accident{
public:
    std::string latitude;
    std::string longitude;
    sf::CircleShape* dot;
    //void updateDot(sf::CircleShape& dot, float y, float x, int scale, int x_offset, int y_offset);
    void updateDot(std::string& city, int scale, int x_offset, int y_offset);
    bool selected;

    Accident();
};

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
    int month;
    int structSelect;
    sf::RenderWindow* window;

    void updateCity(int scale, int x, int y);
    void runSettings();
    void loadButtons();
    void updateMain(int j, std::vector<float> times);
    void cityMenu(std::string selection);
    void updateSettings(sf::RenderWindow& settingsWindow);

public:
    std::vector<MenuButton*> menuButtons;
    std::vector<SelectionButton*> selectionButtons;
    std::map<std::string, sf::RectangleShape*> maps;
    std::vector<Accident*> accidents;

    OrderedMap<std::string, OrderedMap<std::string, OrderedMap<std::vector<std::string>,
    std::vector<std::string>>>> mapStruct;

    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string,
    std::vector<std::string>>>> hashMapStruct;

    void mainMenu();
    sf::RenderWindow* getWindow();

    Window();
};
