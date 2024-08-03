#include "Window.h"
#include "initialize_maps.h"

void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void Window::updateSettings(sf::RenderWindow& settingsWindow){
    settingsWindow.clear({200, 200, 200});

    sf::RectangleShape rectangleShape;
    rectangleShape.setFillColor({225,225,225});
    rectangleShape.setSize(sf::Vector2f(290,40));
    rectangleShape.setPosition(5,5);
    settingsWindow.draw(rectangleShape);

    sf::Text settingsText;
    settingsText.setFont(font);
    settingsText.setScale(0.87,0.87);
    settingsText.setFillColor(sf::Color::Black);
    settingsText.setString("Select a Data Structure");
    settingsText.setPosition(10,7);
    settingsWindow.draw(settingsText);

    for(int i = 12; i <= 13; i++){
        selectionButtons[i]->setSpritePos(30, 30 * (i - 11) + 20);
        settingsWindow.draw(selectionButtons[i]->getSprite());

        sf::CircleShape selected(4);
        selected.setPosition(40, 30 * (i - 11) + 27);
        if(selectionButtons[i]->selected){
            selected.setFillColor(sf::Color::Black);
        }
        else{
            selected.setFillColor(sf::Color::White);
        }

        selected.setOutlineColor(sf::Color::Black);
        selected.setOutlineThickness(1);
        settingsWindow.draw(selected);

        selectionButtons[i]->setTextPos(60, 30 * (i - 11) + 18);
        settingsWindow.draw(selectionButtons[i]->getText());
    }

    settingsWindow.display();
}

void Window::runSettings(){
    sf::RenderWindow settingsWindow (sf::VideoMode(300, 125), "Settings");

    sf::Event event;
    while(settingsWindow.isOpen()) {
        settingsWindow.setVisible(true);

        updateSettings(settingsWindow);

        while (settingsWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                settingsWindow.close();
                return;
            }

            float mouseX, mouseY;
            mouseX = sf::Mouse::getPosition(settingsWindow).x;
            mouseY = sf::Mouse::getPosition(settingsWindow).y;

            for (int i = 12; i <= 13; i++){
                if (selectionButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)) {
                    selectionButtons[i]->setSpriteColor({240, 240, 240});
                }
                else{
                    selectionButtons[i]->setSpriteColor({225, 225, 225});
                }
                if (event.type == sf::Event::MouseButtonPressed){
                    if (selectionButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)) {
                        selectionButtons[i]->setSpriteColor({225, 225, 225});
                    }
                }
                else if (event.type == sf::Event::MouseButtonReleased){
                    if (selectionButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)) {
                        for (int i = 12; i <= 13; i++) {
                            selectionButtons[i]->selected = false;
                        }
                        std::cout << selectionButtons[i]->text << " selected" << std::endl;
                        selectionButtons[i]->selected = true;
                        if (!structSelect) {
                            structSelect = i - 11;
                            settingsWindow.close();
                            return;
                        }
                        else{
                            structSelect = i - 11;
                        }
                    }
                }
            }
            if(settingsWindow.isOpen()){
                updateSettings(settingsWindow);
            }
        }
    }
}

void Window::updateCity(std::map<std::vector<std::string>, std::vector<std::string>>& cityStruct, int scale, int x, int y){
    float newScale = 1 + (scale / 10.0);

    maps[city]->setScale(newScale, newScale);
    maps[city]->setOrigin(400 + (x * 10), 300 + (y * 10));

    window->draw(*maps[city]);

    for(auto i: accidents){
        i->updateDot(city, scale, x, y);
        window->draw(*i->dot);
    }

    sf::RectangleShape rect(sf::Vector2f(250,600));
    rect.setFillColor({200,200,200});
    window->draw(rect);

    sf::Texture texture;
    sf::RectangleShape rectangleShape;
    rectangleShape.setFillColor({225,225,225});
    rectangleShape.setSize(sf::Vector2f(250,40));
    rectangleShape.setPosition(0,30);
    window->draw(rectangleShape);

    sf::Text selectedCity;
    selectedCity.setFont(font);
    selectedCity.setFillColor(sf::Color::Black);
    selectedCity.setString(city);
    selectedCity.setPosition(15,30);

    window->draw(selectedCity);
    window->draw(menuButtons[10]->getSprite());
    window->draw(menuButtons[10]->getText());


    for(int i = 0; i < 12; i++){
        selectionButtons[i]->setSpritePos(5, (30 * i + 5) + 75/* - j*/);
        window->draw(selectionButtons[i]->getSprite());

        sf::CircleShape selected(4);
        selected.setPosition(10, (30 * i + 13) + 75);
        if(selectionButtons[i]->selected){
            selected.setFillColor(sf::Color::Black);
        }
        else{
            selected.setFillColor(sf::Color::White);
        }

        selected.setOutlineColor(sf::Color::Black);
        selected.setOutlineThickness(1);
        window->draw(selected);

        selectionButtons[i]->setTextPos(30, (30 * i + 3) + 75/* - j*/);
        window->draw(selectionButtons[i]->getText());
    }

    window->display();
}

void Window::cityMenu(std::string selection){
    int scale = 0;
    int x = 0;
    int y = 0;

    std::map<std::vector<std::string>, std::vector<std::string>> cityStruct;
    updateCity(cityStruct, scale, x, y);

    while(window->isOpen()){
        sf::Event event;
        while(window->pollEvent(event)){
            if (event.type == sf::Event::Closed) {
                window->close();
            }
            float mouseX, mouseY;
            mouseX = sf::Mouse::getPosition(*window).x;
            mouseY = sf::Mouse::getPosition(*window).y;

            if(menuButtons[10]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                menuButtons[10]->setSpriteColor({240,240,240});
            }
            else{
                menuButtons[10]->setSpriteColor({225,225,225});
            }

            if(event.type == sf::Event::MouseButtonPressed){
                if(menuButtons[10]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                    menuButtons[10]->setSpriteColor({225,225,225});
                }
            }
            else if(event.type == sf::Event::MouseButtonReleased){
                if(menuButtons[10]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                    for(int i = 0; i < 12; i++){
                        selectionButtons[i]->selected = false;
                    }
                    month = "";
                    return;
                }
                for(auto i : accidents){
                    if(i->dot->getGlobalBounds().contains(mouseX, mouseY)){
                        std::cout << i->latitude << " " << i->longitude << std::endl;
                    }
                }
            }

            for(int i = 0; i < 12; i++){
                if(selectionButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                    selectionButtons[i]->setSpriteColor({240,240,240});
                }
                else{
                    selectionButtons[i]->setSpriteColor({225,225,225});
                }
                if(event.type == sf::Event::MouseButtonPressed){
                    if(selectionButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                        selectionButtons[i]->setSpriteColor({225,225,225});
                    }
                }
                else if(event.type == sf::Event::MouseButtonReleased){
                    if(selectionButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                        for(int j = 0; j < 12; j++){
                            selectionButtons[j]->selected = false;
                        }
                        std::cout << selectionButtons[i]->text << " selected" << std::endl;
                        selectionButtons[i]->selected = true;
                        month = selectionButtons[i]->text;
                        cityStruct = mapStruct[city][std::to_string(i + 1)];
                        accidents = {};

                        for(auto i : cityStruct){
                            Accident* accident = new Accident;
                            accident->latitude = stof(i.first[1]);
                            accident->longitude = std::stof(i.first[0]);
                            //updateDot(*accident->dot, std::stof(i.first[0]), stof(i.first[1]), scale, x, y);
                            accident->updateDot(city, scale, x, y);
                            accidents.push_back(accident);
                        }
                    }
                }
            }

            if(event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Z) {
                    if (scale < 30) {
                        scale += 10;
                    }
                }
                else if (event.key.code == sf::Keyboard::X) {
                    if (scale > 0) {
                        scale -= 10;

                        while(300 + (-y * (10 * (1 + scale / 10))) > 300 * (1 + scale / 10)){
                            y += 1;
                        }
                        while(300 + (y * (10 * (1 + scale / 10))) > 300 * (1 + scale / 10)){
                            y -= 1;
                        }
                        while(400 + (-x * (10 * (1 + scale / 10))) > 400 * (1 + scale / 10)){
                            x += 1;
                        }
                        while(400 + (x * (10 * (1 + scale / 10))) > 400 * (1 + scale / 10)){
                            x -= 1;
                        }
                    }
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    if(300 + (-y * (10 * (1 + scale / 10))) < 300 * (1 + scale / 10)){
                        y -= 1;
                    }
                }
                else if (event.key.code == sf::Keyboard::Down){
                    if(300 + (y * (10 * (1 + scale / 10))) < 300 * (1 + scale / 10)){
                        y += 1;
                    }
                }
                else if (event.key.code == sf::Keyboard::Left){
                    if(400 + (-x * (10 * (1 + scale / 10))) < 400 * (1 + scale / 10)){
                        x -= 1;
                    }
                }
                else if (event.key.code == sf::Keyboard::Right){
                    if(400 + (x * (10 * (1 + scale / 10))) < 400 * (1 + scale / 10)){
                        x += 1;
                    }
                }
            }
            updateCity(cityStruct, scale, x, y);
        }
    }
}

void Window::updateMain(int j){
    sf::RectangleShape rect(sf::Vector2f(250,600));
    rect.setFillColor({200,200,200});

    window->draw(rect);
    window->draw(*maps["United States"]);

    for(int i = 0; i < 10; i++){
        menuButtons[i]->setSpritePos(5, (30 * i + 5) - j);
        window->draw(menuButtons[i]->getSprite());

        menuButtons[i]->setTextPos(10, (30 * i + 3) - j);
        window->draw(menuButtons[i]->getText());
    }

    menuButtons[11]->setSpritePos(0, 550);
    menuButtons[11]->setTextPos(10, 560);

    window->draw(menuButtons[11]->getSprite());
    window->draw(menuButtons[11]->getText());

    window->display();
}

void Window::mainMenu() {
    updateMain(0);
    runSettings();
    if(!structSelect){
        return;
    }
    else if(structSelect == 1){
        mapStruct = initialize_maps("../US_Accidents_10_Major_Cities.csv");
    }
    else{
        // Select Hash Map
    }

    while(window->isOpen()){
        sf::Event event;
        while(window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }

            float mouseX, mouseY;
            mouseX = sf::Mouse::getPosition(*window).x;
            mouseY = sf::Mouse::getPosition(*window).y;

            for(int i = 0; i < 12; i++){
                if(menuButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                    menuButtons[i]->setSpriteColor({240,240,240});
                }
                else{
                    menuButtons[i]->setSpriteColor({225,225,225});
                }
                if(event.type == sf::Event::MouseButtonPressed){
                    if(menuButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                        menuButtons[i]->setSpriteColor({225,225,225});
                    }
                }
                if(event.type == sf::Event::MouseButtonReleased){
                    if(menuButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)) {
                        menuButtons[i]->setSpriteColor({240, 240, 240});
                        if(i < 10){
                            sf::Time elapsed;
                            sf::Clock clock;
                            sf::Time ms = sf::milliseconds(1);
                            int counter = 0;
                            while(counter < 300){
                                window->pollEvent(event);
                                elapsed += clock.restart();
                                while(elapsed >= ms){
                                    updateMain(counter);
                                    counter++;
                                    elapsed -= ms;
                                }
                            }

                            std::cout << menuButtons[i]->text << " pressed" << std::endl;

                            city = menuButtons[i]->text;

                            cityMenu(menuButtons[i]->text);
                            city = "None";
                            month = "None";

                            sf::Time elapsed2;
                            sf::Clock clock2;
                            sf::Time ms2 = sf::milliseconds(1);
                            int counter2 = 300;
                            while(counter2 >= 0){
                                window->pollEvent(event);
                                elapsed2 += clock2.restart();
                                while(elapsed2 >= ms2){
                                    updateMain(counter2);
                                    counter2--;
                                    elapsed2 -= ms2;
                                }
                            }
                        }
                        else{
                            std::cout << "Settings Pressed" << std::endl;
                            int previousSelection = structSelect;

                            runSettings();

                            if(structSelect == 1 && previousSelection != 1){
                                mapStruct = initialize_maps("../US_Accidents_10_Major_Cities.csv");
                            }
                            else if(structSelect == 2 && previousSelection != 2){
                                // Select Hashmap;
                            }
                        }
                    }
                }
            }
            updateMain(0);
        }
    }
}

sf::RenderWindow* Window::getWindow(){
    return window;
}

void Window::loadButtons(){
    MenuButton* newYork = new MenuButton("New York", 240, 24);
    menuButtons.push_back(newYork);

    MenuButton* losAngeles = new MenuButton("Los Angeles", 240, 24);
    menuButtons.push_back(losAngeles);

    MenuButton* chicago = new MenuButton("Chicago", 240, 24);
    menuButtons.push_back(chicago);

    MenuButton* houston = new MenuButton("Houston", 240, 24);
    menuButtons.push_back(houston);

    MenuButton* phoenix = new MenuButton("Phoenix", 240, 24);
    menuButtons.push_back(phoenix);

    MenuButton* philadelphia = new MenuButton("Philadelphia", 240, 24);
    menuButtons.push_back(philadelphia);

    MenuButton* sanAntonio = new MenuButton("Seattle", 240, 24);
    menuButtons.push_back(sanAntonio);

    MenuButton* detroit = new MenuButton("Detroit", 240, 24);
    menuButtons.push_back(detroit);

    MenuButton* dallas = new MenuButton("Boston", 240, 24);
    menuButtons.push_back(dallas);

    MenuButton* Miami = new MenuButton("Miami", 240, 24);
    menuButtons.push_back(Miami);

    MenuButton* back = new MenuButton("<- Back", 70, 24);
    menuButtons.push_back(back);

    MenuButton* settings = new MenuButton("Settings", 250, 50);
    menuButtons.push_back(settings);


    SelectionButton* jan = new SelectionButton("January", 240, 24);
    selectionButtons.push_back(jan);

    SelectionButton* feb = new SelectionButton("February", 240, 24);
    selectionButtons.push_back(feb);

    SelectionButton* mar = new SelectionButton("March", 240, 24);
    selectionButtons.push_back(mar);

    SelectionButton* apr = new SelectionButton("April", 240, 24);
    selectionButtons.push_back(apr);

    SelectionButton* may = new SelectionButton("May", 240, 24);
    selectionButtons.push_back(may);

    SelectionButton* jun = new SelectionButton( "June", 240, 24);
    selectionButtons.push_back(jun);

    SelectionButton* jul = new SelectionButton("July", 240, 24);
    selectionButtons.push_back(jul);

    SelectionButton* aug = new SelectionButton("August", 240, 24);
    selectionButtons.push_back(aug);

    SelectionButton* sep = new SelectionButton( "September", 240, 24);
    selectionButtons.push_back(sep);

    SelectionButton* oct = new SelectionButton("October", 240, 24);
    selectionButtons.push_back(oct);

    SelectionButton* nov = new SelectionButton("November", 240, 24);
    selectionButtons.push_back(nov);

    SelectionButton* dec = new SelectionButton("December", 240, 24);
    selectionButtons.push_back(dec);

    SelectionButton* map = new SelectionButton("Map", 240, 24);
    selectionButtons.push_back(map);

    SelectionButton* hashMap = new SelectionButton("Hash Map", 240, 24);
    selectionButtons.push_back(hashMap);


    sf::Texture* unitedStatesTexture = new sf::Texture;
    unitedStatesTexture->loadFromFile("../files/UnitedStates.png");

    sf::RectangleShape* unitedStatesRect = new sf::RectangleShape;
    unitedStatesRect->setSize(sf::Vector2f(800,600));
    unitedStatesRect->setPosition(250,0);
    unitedStatesRect->setTexture(unitedStatesTexture);

    maps["United States"] = unitedStatesRect;

    sf::Texture* newYorkTexture = new sf::Texture;
    newYorkTexture->loadFromFile("../files/NewYork.png");

    sf::RectangleShape* newYorkRect = new sf::RectangleShape;
    newYorkRect->setSize(sf::Vector2f(800,600));
    newYorkRect->setPosition(250 + 400,300);
    newYorkRect->setOrigin(400,300);
    newYorkRect->setTexture(newYorkTexture);

    maps["New York"] = newYorkRect;

    sf::Texture* losAngelesTexture = new sf::Texture;
    losAngelesTexture->loadFromFile("../files/LosAngeles.png");

    sf::RectangleShape* losAngelesRect = new sf::RectangleShape;
    losAngelesRect->setSize(sf::Vector2f(800,600));
    losAngelesRect->setPosition(250,0);
    losAngelesRect->setTexture(losAngelesTexture);

    maps["Los Angeles"] = losAngelesRect;

    sf::Texture* chicagoTexture = new sf::Texture;
    chicagoTexture->loadFromFile("../files/Chicago.png");

    sf::RectangleShape* chicagoRect = new sf::RectangleShape;
    chicagoRect->setSize(sf::Vector2f(800,600));
    chicagoRect->setPosition(250,0);
    chicagoRect->setTexture(chicagoTexture);

    maps["Chicago"] = chicagoRect;

    sf::Texture* houstonTexture = new sf::Texture;
    houstonTexture->loadFromFile("../files/Houston.png");

    sf::RectangleShape* houstonRect = new sf::RectangleShape;
    houstonRect->setSize(sf::Vector2f(800,600));
    houstonRect->setPosition(250,0);
    houstonRect->setTexture(houstonTexture);

    maps["Houston"] = houstonRect;

    sf::Texture* phoenixTexture = new sf::Texture;
    phoenixTexture->loadFromFile("../files/Phoenix.png");

    sf::RectangleShape* phoenixRect = new sf::RectangleShape;
    phoenixRect->setSize(sf::Vector2f(800,600));
    phoenixRect->setPosition(250,0);
    phoenixRect->setTexture(phoenixTexture);

    maps["Phoenix"] = phoenixRect;

    sf::Texture* philadelphiaTexture = new sf::Texture;
    philadelphiaTexture->loadFromFile("../files/Philadelphia.png");

    sf::RectangleShape* philadelphiaRect = new sf::RectangleShape;
    philadelphiaRect->setSize(sf::Vector2f(800,600));
    philadelphiaRect->setPosition(250,0);
    philadelphiaRect->setTexture(philadelphiaTexture);

    maps["Philadelphia"] = philadelphiaRect;

    sf::Texture* seattleTexture = new sf::Texture;
    seattleTexture->loadFromFile("../files/Seattle.png");

    sf::RectangleShape* seattleRect = new sf::RectangleShape;
    seattleRect->setSize(sf::Vector2f(800,600));
    seattleRect->setPosition(250,0);
    seattleRect->setTexture(seattleTexture);

    maps["Seattle"] = seattleRect;

    sf::Texture* detroitTexture = new sf::Texture;
    detroitTexture->loadFromFile("../files/Detroit.png");

    sf::RectangleShape* detroitRect = new sf::RectangleShape;
    detroitRect->setSize(sf::Vector2f(800,600));
    detroitRect->setPosition(250,0);
    detroitRect->setTexture(detroitTexture);

    maps["Detroit"] = detroitRect;

    sf::Texture* bostonTexture = new sf::Texture;
    bostonTexture->loadFromFile("../files/Boston.png");

    sf::RectangleShape* bostonRect = new sf::RectangleShape;
    bostonRect->setSize(sf::Vector2f(800,600));
    bostonRect->setPosition(250,0);
    bostonRect->setTexture(bostonTexture);

    maps["Boston"] = bostonRect;

    sf::Texture* miamiTexture = new sf::Texture;
    miamiTexture->loadFromFile("../files/Miami.png");

    sf::RectangleShape* miamiRect = new sf::RectangleShape;
    miamiRect->setSize(sf::Vector2f(800,600));
    miamiRect->setPosition(250,0);
    miamiRect->setTexture(miamiTexture);

    maps["Miami"] = miamiRect;
}

void Accident::updateDot(std::string& city, int scale, int x_offset, int y_offset){
    dot->setRadius(1.5 + scale / 10);
    dot->setOutlineColor(sf::Color::Black);
    dot->setOutlineThickness(0.1 * scale / 10);
    dot->setOrigin(1,1);

    if(city == "New York"){
        dot->setPosition((((latitude + 74.373828) / 0.732651) * (800 * (1 + scale / 10.0))) + (250 - (scale * 39.9) - (x_offset*(10+scale))),
                         ((40.889949 - longitude) / 0.4137) * (600 * (1 + scale/10.0)) - (scale * 30) - (y_offset*(10+scale)));
    }
    else if(city == "Los Angeles"){
        dot->setPosition((((latitude + 118.951490) / 1.41861) * 800) + 250, ((34.457127 - longitude) / 0.922217) * 600);
    }
    else if(city == "Chicago"){
        dot->setPosition((((longitude + 88.001141) / 0.747931) * 800) + 250, ((42.049659 - latitude) / 0.417422) * 600);
    }
    else if(city == "Houston"){
        dot->setPosition((((longitude + 96.077007) / 1.487007) * 800) + 250, ((30.205625 - latitude) / 0.971206) * 600);
    }
    else if(city == "Phoenix"){
        dot->setPosition((((longitude + 112.832350) / 1.471048) * 800) + 250, ((33.916651 - latitude) / 0.916546) * 600);
    }
    else if(city == "Philadelphia"){
        dot->setPosition((((longitude + 75.533461) / 0.741084) * 800) + 250, ((40.154221 - latitude) / 0.429186) * 600);
    }
    else if(city == "Seattle"){
        dot->setPosition((((longitude + 122.704418) / 0.735338) * 800) + 250, ((47.770371 - latitude) / 0.370043) * 600);
    }
    else if(city == "Detroit"){
        dot->setPosition((((longitude + 83.329453) / 0.551048) * 800) + 250, ((42.473015 - latitude) / 0.304626) * 600);
    }
    else if(city == "Boston"){
        dot->setPosition((((longitude + 71.416616) / 0.738651) * 800) + 250, ((42.540786 - latitude) / 0.417064) * 600);
    }
    else if(city == "Miami"){
        dot->setPosition((((longitude + 80.374688) / 0.368266) * 800) + 250, ((25.863971 - latitude) / 0.247405) * 600);
    }
}

sf::Sprite MenuButton::getSprite(){
    return buttonSprite;
}

sf::Text MenuButton::getText(){
    return buttonText;
}

void MenuButton::setSpritePos(int x, int y){
    buttonSprite.setPosition(x, y);
}

void MenuButton::setTextPos(int x, int y){
    buttonText.setPosition(x, y);
}

void MenuButton::setSpriteColor(sf::Color color){
    buttonSprite.setColor(color);
}

Accident::Accident(){
    dot = new sf::CircleShape;
    dot->setFillColor(sf::Color::Red);
}

MenuButton::MenuButton(std::string inputText, int width, int height) {
    text = inputText;

    sf::Texture *texture = new sf::Texture;
    sf::IntRect testButtonRect(0,0,width,height);
    buttonSprite.setColor({225,225,225});
    buttonSprite.setTextureRect(testButtonRect);
    buttonSprite.setTexture(*texture);

    font.loadFromFile("../fonts/OpenSans-Regular.ttf");
    buttonText.setFont(font);
    buttonText.setString(inputText);
    buttonText.setCharacterSize(20);
    buttonText.setFillColor({0,0,0});
}

SelectionButton::SelectionButton(std::string inputText, int width, int height) : MenuButton(inputText, width, height) {
    selected = false;
    text = inputText;

    sf::Texture texture;
    sf::IntRect testButtonRect(0,0,width,height);
    buttonSprite.setColor({225,225,225});
    buttonSprite.setTextureRect(testButtonRect);
    buttonSprite.setTexture(texture);

    font.loadFromFile("../fonts/OpenSans-Regular.ttf");
    buttonText.setFont(font);
    buttonText.setString(inputText);
    buttonText.setCharacterSize(20);
    buttonText.setFillColor({0,0,0});
}

Window::Window(){
    city = "None";
    month = "None";
    structSelect = 0;
    window = new sf::RenderWindow(sf::VideoMode(1050, 600), "Car Accident Visualization");
    loadButtons();
    font.loadFromFile("../fonts/OpenSans-Regular.ttf");
}
