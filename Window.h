#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "ordered_map.h"
#include "HashMap.h"

// Accident class
class Accident{
public:
    // Used to search map & hash map
    std::string latitude;
    std::string longitude;

    // Dot object to display on map
    sf::CircleShape* dot;
    void updateDot(std::string& city, int scale, int x_offset, int y_offset);
    bool selected;

    // Constructor
    Accident();
};

// Menu button class
class MenuButton{
private:
    // MenuButton attributes
    sf::Font font;
    std::string text;
    sf::Text buttonText;
    sf::Sprite buttonSprite;
    sf::RectangleShape buttonRectangle;

public:
    // Getters and setters
    sf::Text getText();
    std::string getSelectionText();
    sf::Sprite getSprite();
    sf::RectangleShape getRect();
    void setPos(int x, int y);
    void setTextPos(int x, int y);
    void setColor(sf::Color color);

    // Parameterized constructor
    MenuButton(const std::string& text, int width, int height);
};

// Selection button child class
class SelectionButton : public MenuButton{
private:
    // Attributes
    sf::Font font;
    bool selected;
    sf::Text buttonText;
    sf::Sprite buttonSprite;
    sf::RectangleShape buttonRectangle;

public:
    // Getter and setters
    void select();
    void unselect();
    bool getSelection();

    // Parameterized constructor
    SelectionButton(const std::string& inputText, int width, int height);
};

// Window class
class Window{
private:
    // Attributes
    int month;
    sf::Font font;
    std::string city;
    int structSelect;
    sf::RenderWindow* window;

    // Private Functions
    void updateCity(int scale, int x, int y);
    void runSettings();
    void loadTextures();
    void updateMain(int j, std::vector<float> times);
    void cityMenu();
    void updateSettings(sf::RenderWindow& settingsWindow);

    // Maps, buttons, textures and accident objects
    std::vector<Accident*> accidents;
    std::vector<MenuButton*> menuButtons;
    std::vector<SelectionButton*> selectionButtons;
    std::map<std::string, sf::RectangleShape*> maps;

    // Map structure
    OrderedMap<std::string, OrderedMap<std::string,
    OrderedMap<std::vector<std::string>,std::vector<std::string>>>> mapStruct;

    // Hash map structure
    Hashmap<std::string, Hashmap<std::string,
    Hashmap<std::string, std::vector<std::string>>>> hashMapStruct;

public:
    // Run program
    void mainMenu();

    // Default constructor
    Window();
};
