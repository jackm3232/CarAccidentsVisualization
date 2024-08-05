#include <sstream>
#include <thread>
#include "Window.h"
#include "initialize_maps.h"

void Window::updateSettings(sf::RenderWindow& settingsWindow){
    // Set color of settings window
    settingsWindow.clear({200, 200, 200});

    // Set rectangle for text background
    sf::RectangleShape rectangleShape;
    rectangleShape.setFillColor({225,225,225});
    rectangleShape.setSize(sf::Vector2f(290,40));
    rectangleShape.setPosition(5,5);
    settingsWindow.draw(rectangleShape);

    // Settings text prompt
    sf::Text settingsText;
    settingsText.setFont(font);
    settingsText.setScale(0.87,0.87);
    settingsText.setFillColor(sf::Color::Black);
    settingsText.setString("Select a Data Structure");
    settingsText.setPosition(10,7);
    settingsWindow.draw(settingsText);

    // For each structure selection button
    for(int i = 12; i <= 13; i++){
        // Draw each button sprite
        selectionButtons[i]->setPos(30, 30 * (i - 11) + 20);
        settingsWindow.draw(selectionButtons[i]->getRect());

        // Draw circle and dot for selected button
        sf::CircleShape selected(4);
        selected.setPosition(40, 30 * (i - 11) + 27);
        if(selectionButtons[i]->getSelection()){
            selected.setFillColor(sf::Color::Black);
        }
        else{
            selected.setFillColor(sf::Color::White);
        }

        selected.setOutlineColor(sf::Color::Black);
        selected.setOutlineThickness(1);
        settingsWindow.draw(selected);

        // Draw button text
        selectionButtons[i]->setTextPos(60, 30 * (i - 11) + 18);
        settingsWindow.draw(selectionButtons[i]->getText());
    }

    // Render the settings window
    settingsWindow.display();
}

void Window::runSettings(){
    // Create the settings window
    sf::RenderWindow settingsWindow(sf::VideoMode(300, 125), "Settings");

    sf::Event event;
    while(settingsWindow.isOpen()){
        // Set the settings window on top
        settingsWindow.setVisible(true);

        // Update the settings window
        updateSettings(settingsWindow);

        while(settingsWindow.pollEvent(event)){
            // Poll the main window to catch extraneous input while settings open
            sf::Event windowEvent;
            while(window->pollEvent(windowEvent)){}

            // User closes settings window
            if (event.type == sf::Event::Closed){
                settingsWindow.close();
                return;
            }

            // Get mouse location
            float mouseX, mouseY;
            mouseX = sf::Mouse::getPosition(settingsWindow).x;
            mouseY = sf::Mouse::getPosition(settingsWindow).y;

            // Render both structure selection buttons
            for (int i = 12; i <= 13; i++){
                // Highlight button if user hovers mouse
                if(selectionButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)) {
                    selectionButtons[i]->setColor({240, 240, 240});
                }
                else{
                    selectionButtons[i]->setColor({225, 225, 225});
                }

                // Return colors to normal if user clicks
                if(event.type == sf::Event::MouseButtonPressed){
                    if (selectionButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)) {
                        selectionButtons[i]->setColor({225, 225, 225});
                    }
                }
                // If user releases mouse over button, select it
                else if(event.type == sf::Event::MouseButtonReleased){
                    // If user selects button
                    if (selectionButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)) {
                        // Unselect both buttons
                        for (int i = 12; i <= 13; i++) {
                            selectionButtons[i]->unselect();
                        }
                        // Select current button
                        selectionButtons[i]->select();

                        // If no structure selected (program just started)
                        if (!structSelect) {
                            // Update structure selection and close settings window
                            structSelect = i - 11;
                            settingsWindow.close();
                            return;
                        }
                        else{
                            // Update structure selection
                            structSelect = i - 11;
                        }
                    }
                }
            }
            // Update the settings window
            updateSettings(settingsWindow);
        }
    }
}

void Window::updateCity(int scale, int x, int y){
    // Set window color
    window->clear({200,200,200});

    // Set map scale
    float newScale = 1 + (scale / 10.0);

    // Apply the scale to city map
    maps[city]->setScale(newScale, newScale);
    maps[city]->setOrigin(400 + (x * 10), 300 + (y * 10));

    // Draw the selected city map
    window->draw(*maps[city]);

    // Update the dots with the current scale and x & y offset
    for(auto i: accidents){
        i->updateDot(city, scale, x, y);
        window->draw(*i->dot);
    }

    // Create and draw various window objects
    sf::RectangleShape rect(sf::Vector2f(250,600));
    rect.setFillColor({200,200,200});
    window->draw(rect);

    sf::RectangleShape rectangleShape;
    rectangleShape.setFillColor({225,225,225});
    rectangleShape.setSize(sf::Vector2f(250,40));
    rectangleShape.setPosition(0,30);
    window->draw(rectangleShape);

    // Set the name of the selected city
    sf::Text selectedCity;
    selectedCity.setFont(font);
    selectedCity.setFillColor(sf::Color::Black);
    selectedCity.setString(city);
    selectedCity.setPosition(15,30);

    // Draw the selected city text and back button
    window->draw(selectedCity);
    window->draw(menuButtons[10]->getRect());
    window->draw(menuButtons[10]->getText());

    // Draw accident info background rectangle
    sf::RectangleShape infoRect;
    infoRect.setFillColor({225,225,225});
    infoRect.setSize(sf::Vector2f(250,97));
    infoRect.setPosition(252,507);
    infoRect.setOutlineColor(sf::Color::Black);
    infoRect.setOutlineThickness(2);
    window->draw(infoRect);

    // Accident info header text
    sf::Text accidentText;
    accidentText.setFont(font);
    accidentText.setFillColor(sf::Color::Black);
    accidentText.setScale(0.5,0.5);
    accidentText.setString("Accident Info:");
    accidentText.setPosition(257,505);
    window->draw(accidentText);

    // Draw control hints
    sf::Text controlHint;
    controlHint.setFont(font);
    controlHint.setFillColor(sf::Color::Black);
    controlHint.setScale(0.5,0.5);
    controlHint.setString("Use Z and X to zoom");
    controlHint.setPosition(5,550);
    window->draw(controlHint);

    controlHint.setString("Use Arrow Keys to pan");
    controlHint.setPosition(5, 565);
    window->draw(controlHint);

    controlHint.setString("Select a dot to view accident data");
    controlHint.setPosition(5, 580);
    window->draw(controlHint);


    for(auto i : accidents){
        // When user clicks on a dot
        if(i->selected){
            // Use the coordinate stored in the dot to search selected data structure
            std::vector<std::string> key = {i->longitude, i->latitude};
            std::string date;
            std::string temp;
            std::string condition;
            std::string severity;

            // Load information from map
            if(structSelect == 1){
                date = mapStruct[city][std::to_string(month)][key][0];
                temp = mapStruct[city][std::to_string(month)][key][1];
                condition = mapStruct[city][std::to_string(month)][key][2];
                severity = mapStruct[city][std::to_string(month)][key][3];
            }
            // Load information from hash map
            else if(structSelect == 2){
                auto searchTimeStart = std::chrono::high_resolution_clock::now();
                date = hashMapStruct[city][std::to_string(month)][key[0] + "x" + key[1]][0];
                temp = hashMapStruct[city][std::to_string(month)][key[0] + "x" + key[1]][1];
                condition = hashMapStruct[city][std::to_string(month)][key[0] + "x" + key[1]][2];
                severity = hashMapStruct[city][std::to_string(month)][key[0] + "x" + key[1]][3];
            }

            // Display accident information

            // Date
            accidentText.setString("Date: " + date);
            accidentText.setPosition(257,520);
            window->draw(accidentText);

            // Coordinates
            accidentText.setString("Location: " + std::to_string(std::stof(i->longitude)) + ", " + std::to_string(std::stof(i->latitude)));
            accidentText.setPosition(257,535);
            window->draw(accidentText);

            // Temperature
            accidentText.setString("Temperature: " + temp + "F");
            accidentText.setPosition(257,550);
            window->draw(accidentText);

            // Weather condition
            accidentText.setString("Weather: " + condition);
            accidentText.setPosition(257,565);
            window->draw(accidentText);

            // Severity on a scale of 1 to 4
            accidentText.setString("Accident Severity (From 1-4): " + severity);
            accidentText.setPosition(257,580);
            window->draw(accidentText);
        }
    }

    // For each selection button
    for(int i = 0; i < 12; i++){
        // Draw selection buttons
        selectionButtons[i]->setPos(5, (30 * i + 5) + 75/* - j*/);
        window->draw(selectionButtons[i]->getRect());

        // Draw circle and dot for selected button
        sf::CircleShape selected(4);
        selected.setPosition(10, (30 * i + 13) + 75);
        if(selectionButtons[i]->getSelection()){
            selected.setFillColor(sf::Color::Black);
        }
        else{
            selected.setFillColor(sf::Color::White);
        }

        selected.setOutlineColor(sf::Color::Black);
        selected.setOutlineThickness(1);
        window->draw(selected);

        // Draw button text
        selectionButtons[i]->setTextPos(30, (30 * i + 3) + 75/* - j*/);
        window->draw(selectionButtons[i]->getText());
    }

    // Render window
    window->display();
}

void Window::cityMenu(){
    // Panning and scaling variables
    int scale = 0;
    int x = 0;
    int y = 0;

    // Update the city menu
    updateCity(scale, x, y);

    while(window->isOpen()){
        sf::Event event;
        while(window->pollEvent(event)){
            // User closes the window
            if(event.type == sf::Event::Closed){
                for(auto j : accidents){
                    j->selected = false;
                }
                window->close();
                return;
            }

            // Get mouse location
            float mouseX, mouseY;
            mouseX = sf::Mouse::getPosition(*window).x;
            mouseY = sf::Mouse::getPosition(*window).y;

            // Highlight back button if mouse is hovering
            if(menuButtons[10]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                menuButtons[10]->setColor({240,240,240});
            }
            else{
                menuButtons[10]->setColor({225,225,225});
            }

            // Reset colors if user clicks back button
            if(event.type == sf::Event::MouseButtonPressed){
                if(menuButtons[10]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                    menuButtons[10]->setColor({225,225,225});
                }
            }
            // Select back button if user releases mouse
            else if(event.type == sf::Event::MouseButtonReleased){
                // If user selects back button
                if(menuButtons[10]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                    // Unselect all buttons
                    for(int i = 0; i < 12; i++){
                        selectionButtons[i]->unselect();
                    }

                    // Clear accident vector and reset month
                    accidents = {};
                    month = 0;
                    return;
                }
                // User clicked an accident dot
                for(auto i : accidents){
                    if(i->dot->getGlobalBounds().contains(mouseX, mouseY)){
                        // Unselect all dots
                        for(auto j : accidents){
                            j->selected = false;
                        }
                        // Select current dot
                        i->selected = true;
                    }
                }
            }

            // Selection buttons
            for(int i = 0; i < 12; i++){
                // If user hovers over button, highlight it
                if(selectionButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                    selectionButtons[i]->setColor({240,240,240});
                }
                else{
                    selectionButtons[i]->setColor({225,225,225});
                }
                // Reset color if user clicks
                if(event.type == sf::Event::MouseButtonPressed){
                    if(selectionButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                        selectionButtons[i]->setColor({225,225,225});
                    }
                }
                // Select button when user releases mouse
                else if(event.type == sf::Event::MouseButtonReleased){
                    if(selectionButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                        // Unselect all selection buttons
                        for(int j = 0; j < 12; j++){
                            selectionButtons[j]->unselect();
                        }
                        // Select current selection button
                        selectionButtons[i]->select();
                        // Set the month index (+1 because off-by-one) and reset accidents vector
                        month = i + 1;
                        accidents = {};

                        // Current structure is map
                        if(structSelect== 1){
                            // Vectors to store traversal
                            std::vector<std::vector<std::string>> key_vect;
                            std::vector<std::vector<std::string>> value_vect;

                            // Traverse the map structure
                            mapStruct[city][std::to_string(month)].traverse(key_vect, value_vect);

                            // Read results of traversal and store keys in accident object vector
                            for(auto i : key_vect){
                                Accident* accident = new Accident;
                                accident->longitude = i[0];
                                accident->latitude = i[1];
                                accident->updateDot(city, scale, x, y);
                                accidents.push_back(accident);
                            }
                        }
                        // Current structure is hash map
                        else{
                            // Vectors to store result of traversal
                            std::vector<std::string> key_vect;
                            std::vector<std::vector<std::string>> value_vect;

                            // Traverse the hash map
                            hashMapStruct[city][std::to_string(month)].traverse(key_vect, value_vect);

                            // Read results
                            for(auto i : key_vect) {
                                std::istringstream coords(i);
                                std::vector<std::string> coordsParsed;

                                // Split string key into longitude and latitude coordinates
                                std::string buffer;
                                for (int i = 0; i < 2; i++) {
                                    getline(coords, buffer, 'x');
                                    coordsParsed.push_back(buffer);
                                }

                                // Store longitude and latitude in accident object vector
                                Accident* accident = new Accident;
                                accident->longitude = coordsParsed[0];
                                accident->latitude = coordsParsed[1];
                                accident->updateDot(city, scale, x, y);
                                accidents.push_back(accident);
                            }
                        }
                    }
                }
            }

            // User presses key
            if(event.type == sf::Event::KeyPressed) {
                // User presses Z to zoom
                if (event.key.code == sf::Keyboard::Z) {
                    // If scale is within limit, increase it
                    if (scale < 30) {
                        scale += 10;
                    }
                }
                // User presses X to zoom out
                else if (event.key.code == sf::Keyboard::X) {
                    // If zoomed, decrease scale
                    if (scale > 0) {
                        scale -= 10;

                        // Check all 4 sides of screen, pull map back into boundaries of screen when zooming out
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
                // If user pans up
                else if (event.key.code == sf::Keyboard::Up) {
                    // Decrement y if map within bounds of window
                    if(300 + (-y * (10 * (1 + scale / 10))) < 300 * (1 + scale / 10)){
                        y -= 1;
                    }
                }
                // If user pans down
                else if (event.key.code == sf::Keyboard::Down){
                    // Increment y if map within bounds of window
                    if(300 + (y * (10 * (1 + scale / 10))) < 300 * (1 + scale / 10)){
                        y += 1;
                    }
                }
                // If user pans left
                else if (event.key.code == sf::Keyboard::Left){
                    // Decrement x if map within bounds of window
                    if(400 + (-x * (10 * (1 + scale / 10))) < 400 * (1 + scale / 10)){
                        x -= 1;
                    }
                }
                // If user pans right
                else if (event.key.code == sf::Keyboard::Right){
                    // Increment x if map within bounds of window
                    if(400 + (x * (10 * (1 + scale / 10))) < 400 * (1 + scale / 10)){
                        x += 1;
                    }
                }
            }
            // Update the city menu
            updateCity(scale, x, y);
        }
    }
}

void Window::updateMain(int j, std::vector<float> times){
    // Create and render menu sidebar
    sf::RectangleShape rect(sf::Vector2f(250,600));
    rect.setFillColor({200,200,200});
    window->draw(rect);

    // Draw the U.S. map
    window->draw(*maps["United States"]);

    // Render the 10 city menu buttons
    for(int i = 0; i < 10; i++){
        menuButtons[i]->setPos(5, (30 * i + 5) - j);
        menuButtons[i]->setTextPos(10, (30 * i + 3) - j);
        window->draw(menuButtons[i]->getRect());
        window->draw(menuButtons[i]->getText());
    }

    // Render the settings button
    menuButtons[11]->setPos(0, 550);
    menuButtons[11]->setTextPos(10, 560);
    window->draw(menuButtons[11]->getRect());
    window->draw(menuButtons[11]->getText());

    // If the load times have been set
    if(times.size() > 0){
        // Set and display the performance times for each structure
        sf::Text performanceText;
        performanceText.setFont(font);
        performanceText.setFillColor(sf::Color::Black);
        performanceText.setScale(0.5,0.5);

        performanceText.setString("Structure Load Performance:");
        performanceText.setPosition(5,500);
        window->draw(performanceText);

        performanceText.setString("Map: " + std::to_string(times[0]) + "s");
        performanceText.setPosition(5,515);
        window->draw(performanceText);

        performanceText.setString("Hash Map: " + std::to_string(times[1]) + "s");
        performanceText.setPosition(5,530);
        window->draw(performanceText);
    }

    // Render the window
    window->display();
}

void Window::mainMenu(){
    // Vector to store the structure performance times
    std::vector<float> times;

    // Update the main menu, open the settings when window starts
    updateMain(0, times);
    runSettings();

    // If the user closes the settings window without selecting a structure
    if(!structSelect){
        // Close the window
        window->close();
        return;
    }
    else{
        // Load the structures using the data from the car accidents CSV file
        times = initialize_maps("../US_Accidents_10_Major_Cities.csv", mapStruct, hashMapStruct);
    }

    // While the main window is open
    while(window->isOpen()){
        sf::Event event;
        while(window->pollEvent(event)) {
            // User closes window
            if(event.type == sf::Event::Closed) {
                window->close();
                return;
            }

            // Get mouse coordinates
            float mouseX, mouseY;
            mouseX = sf::Mouse::getPosition(*window).x;
            mouseY = sf::Mouse::getPosition(*window).y;

            // Loop through menuButtons
            for(int i = 0; i < 12; i++){
                // Change color of menuButton if mouse is hovering over it
                if(menuButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                    menuButtons[i]->setColor({240,240,240});
                }
                else{
                    menuButtons[i]->setColor({225,225,225});
                }

                // If user clicks button, return color to normal
                if(event.type == sf::Event::MouseButtonPressed){
                    if(menuButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)){
                        menuButtons[i]->setColor({225,225,225});
                    }
                }
                // If user releases mouse over button, select it
                else if(event.type == sf::Event::MouseButtonReleased){
                    if(menuButtons[i]->getSprite().getGlobalBounds().contains(mouseX, mouseY)) {
                        menuButtons[i]->setColor({240, 240, 240});
                        // If any of the 10 city buttons selected
                        if(i < 10){
                            // Referenced from SFML Documentation: https://www.sfml-dev.org/documentation/2.6.1/classsf_1_1Clock.php,
                            //      https://www.sfml-dev.org/documentation/2.6.1/classsf_1_1Time.php
                            // Used to animate menu motion
                            sf::Time elapsed;
                            sf::Clock clock;
                            sf::Time ms = sf::milliseconds(1);
                            int elapsedMS = 0;
                            while(elapsedMS < 300){
                                sf::Event windowEvent;
                                window->pollEvent(windowEvent);
                                window->pollEvent(event);
                                elapsed += clock.restart();
                                while(elapsed >= ms){
                                    updateMain(elapsedMS, times);
                                    elapsedMS++;
                                    elapsed -= ms;
                                }
                            }

                            // Set the city selection to the button text
                            city = menuButtons[i]->getSelectionText();

                            // Call city menu
                            cityMenu();

                            // Reset values after cityMenu() returns
                            city = "None";
                            month = 0;

                            // Referenced from SFML Documentation: https://www.sfml-dev.org/documentation/2.6.1/classsf_1_1Clock.php
                            //      https://www.sfml-dev.org/documentation/2.6.1/classsf_1_1Time.php
                            // Used to animate menu motion
                            sf::Time elapsed2;
                            sf::Clock clock2;
                            sf::Time ms2 = sf::milliseconds(1);
                            int counter2 = 300;
                            while(counter2 >= 0){
                                sf::Event windowEvent;
                                window->pollEvent(windowEvent);
                                window->pollEvent(event);
                                elapsed2 += clock2.restart();
                                while(elapsed2 >= ms2){
                                    updateMain(counter2, times);
                                    counter2--;
                                    elapsed2 -= ms2;
                                }
                            }
                        }
                        // Settings button pressed
                        else if(i == 11){
                            runSettings();
                        }
                    }
                }
            }
            // Update the main menu
            updateMain(0, times);
        }
    }
}

void Window::loadTextures(){
    // Loading textures for buttons and maps

    // New York
    MenuButton* newYorkButton = new MenuButton("New York", 240, 24);
    menuButtons.push_back(newYorkButton);

    sf::Texture* newYorkTexture = new sf::Texture;
    newYorkTexture->loadFromFile("../files/NewYork.png");

    sf::RectangleShape* newYorkRect = new sf::RectangleShape;
    newYorkRect->setSize(sf::Vector2f(800,600));
    newYorkRect->setPosition(250 + 400,300);
    newYorkRect->setOrigin(400,300);
    newYorkRect->setTexture(newYorkTexture);

    maps["New York"] = newYorkRect;


    // Los Angeles
    MenuButton* losAngelesButton = new MenuButton("Los Angeles", 240, 24);
    menuButtons.push_back(losAngelesButton);

    sf::Texture* losAngelesTexture = new sf::Texture;
    losAngelesTexture->loadFromFile("../files/LosAngeles.png");

    sf::RectangleShape* losAngelesRect = new sf::RectangleShape;
    losAngelesRect->setSize(sf::Vector2f(800,600));
    losAngelesRect->setPosition(250 + 400,300);
    losAngelesRect->setOrigin(400,300);
    losAngelesRect->setTexture(losAngelesTexture);

    maps["Los Angeles"] = losAngelesRect;


    // Chicago
    MenuButton* chicagoButton = new MenuButton("Chicago", 240, 24);
    menuButtons.push_back(chicagoButton);

    sf::Texture* chicagoTexture = new sf::Texture;
    chicagoTexture->loadFromFile("../files/Chicago.png");

    sf::RectangleShape* chicagoRect = new sf::RectangleShape;
    chicagoRect->setSize(sf::Vector2f(800,600));
    chicagoRect->setPosition(250 + 400,300);
    chicagoRect->setOrigin(400,300);
    chicagoRect->setTexture(chicagoTexture);

    maps["Chicago"] = chicagoRect;


    // Houston
    MenuButton* houstonButton = new MenuButton("Houston", 240, 24);
    menuButtons.push_back(houstonButton);

    sf::Texture* houstonTexture = new sf::Texture;
    houstonTexture->loadFromFile("../files/Houston.png");

    sf::RectangleShape* houstonRect = new sf::RectangleShape;
    houstonRect->setSize(sf::Vector2f(800,600));
    houstonRect->setPosition(250 + 400,300);
    houstonRect->setOrigin(400,300);
    houstonRect->setTexture(houstonTexture);

    maps["Houston"] = houstonRect;


    // Phoenix
    MenuButton* phoenixButton = new MenuButton("Phoenix", 240, 24);
    menuButtons.push_back(phoenixButton);

    sf::Texture* phoenixTexture = new sf::Texture;
    phoenixTexture->loadFromFile("../files/Phoenix.png");

    sf::RectangleShape* phoenixRect = new sf::RectangleShape;
    phoenixRect->setSize(sf::Vector2f(800,600));
    phoenixRect->setPosition(250 + 400,300);
    phoenixRect->setOrigin(400,300);
    phoenixRect->setTexture(phoenixTexture);

    maps["Phoenix"] = phoenixRect;


    // Philadelphia
    MenuButton* philadelphiaButton = new MenuButton("Philadelphia", 240, 24);
    menuButtons.push_back(philadelphiaButton);

    sf::Texture* philadelphiaTexture = new sf::Texture;
    philadelphiaTexture->loadFromFile("../files/Philadelphia.png");

    sf::RectangleShape* philadelphiaRect = new sf::RectangleShape;
    philadelphiaRect->setSize(sf::Vector2f(800,600));
    philadelphiaRect->setPosition(250 + 400,300);
    philadelphiaRect->setOrigin(400,300);
    philadelphiaRect->setTexture(philadelphiaTexture);

    maps["Philadelphia"] = philadelphiaRect;


    // Seattle
    MenuButton* sanAntonioButton = new MenuButton("Seattle", 240, 24);
    menuButtons.push_back(sanAntonioButton);

    sf::Texture* seattleTexture = new sf::Texture;
    seattleTexture->loadFromFile("../files/Seattle.png");

    sf::RectangleShape* seattleRect = new sf::RectangleShape;
    seattleRect->setSize(sf::Vector2f(800,600));
    seattleRect->setPosition(250 + 400,300);
    seattleRect->setOrigin(400,300);
    seattleRect->setTexture(seattleTexture);

    maps["Seattle"] = seattleRect;


    // Detroit
    MenuButton* detroitButton = new MenuButton("Detroit", 240, 24);
    menuButtons.push_back(detroitButton);

    sf::Texture* detroitTexture = new sf::Texture;
    detroitTexture->loadFromFile("../files/Detroit.png");

    sf::RectangleShape* detroitRect = new sf::RectangleShape;
    detroitRect->setSize(sf::Vector2f(800,600));
    detroitRect->setPosition(250 + 400,300);
    detroitRect->setOrigin(400,300);
    detroitRect->setTexture(detroitTexture);

    maps["Detroit"] = detroitRect;


    // Boston
    MenuButton* bostonButton = new MenuButton("Boston", 240, 24);
    menuButtons.push_back(bostonButton);

    sf::Texture* bostonTexture = new sf::Texture;
    bostonTexture->loadFromFile("../files/Boston.png");

    sf::RectangleShape* bostonRect = new sf::RectangleShape;
    bostonRect->setSize(sf::Vector2f(800,600));
    bostonRect->setPosition(250 + 400,300);
    bostonRect->setOrigin(400,300);
    bostonRect->setTexture(bostonTexture);

    maps["Boston"] = bostonRect;


    // Miami
    MenuButton* miamiButton = new MenuButton("Miami", 240, 24);
    menuButtons.push_back(miamiButton);

    sf::Texture* miamiTexture = new sf::Texture;
    miamiTexture->loadFromFile("../files/Miami.png");

    sf::RectangleShape* miamiRect = new sf::RectangleShape;
    miamiRect->setSize(sf::Vector2f(800,600));
    miamiRect->setPosition(250 + 400,300);
    miamiRect->setOrigin(400,300);
    miamiRect->setTexture(miamiTexture);

    maps["Miami"] = miamiRect;


    // United States map texture
    sf::Texture* unitedStatesTexture = new sf::Texture;
    unitedStatesTexture->loadFromFile("../files/UnitedStates.png");

    sf::RectangleShape* unitedStatesRect = new sf::RectangleShape;
    unitedStatesRect->setSize(sf::Vector2f(800,600));
    unitedStatesRect->setPosition(250,0);
    unitedStatesRect->setTexture(unitedStatesTexture);

    maps["United States"] = unitedStatesRect;


    // Back button
    MenuButton* backButton = new MenuButton("<- Back", 70, 24);
    menuButtons.push_back(backButton);

    // Setting button
    MenuButton* settingsButton = new MenuButton("Settings", 250, 50);
    menuButtons.push_back(settingsButton);

    // Months buttons
    SelectionButton* janButton = new SelectionButton("January", 240, 24);
    selectionButtons.push_back(janButton);

    SelectionButton* febButton = new SelectionButton("February", 240, 24);
    selectionButtons.push_back(febButton);

    SelectionButton* marButton = new SelectionButton("March", 240, 24);
    selectionButtons.push_back(marButton);

    SelectionButton* aprButton = new SelectionButton("April", 240, 24);
    selectionButtons.push_back(aprButton);

    SelectionButton* mayButton = new SelectionButton("May", 240, 24);
    selectionButtons.push_back(mayButton);

    SelectionButton* junButton = new SelectionButton( "June", 240, 24);
    selectionButtons.push_back(junButton);

    SelectionButton* julButton = new SelectionButton("July", 240, 24);
    selectionButtons.push_back(julButton);

    SelectionButton* augButton = new SelectionButton("August", 240, 24);
    selectionButtons.push_back(augButton);

    SelectionButton* sepButton = new SelectionButton( "September", 240, 24);
    selectionButtons.push_back(sepButton);

    SelectionButton* octButton = new SelectionButton("October", 240, 24);
    selectionButtons.push_back(octButton);

    SelectionButton* novButton = new SelectionButton("November", 240, 24);
    selectionButtons.push_back(novButton);

    SelectionButton* decButton = new SelectionButton("December", 240, 24);
    selectionButtons.push_back(decButton);

    SelectionButton* mapButton = new SelectionButton("Map", 240, 24);
    selectionButtons.push_back(mapButton);

    SelectionButton* hashMapButton = new SelectionButton("Hash Map", 240, 24);
    selectionButtons.push_back(hashMapButton);
}

void Accident::updateDot(std::string& city, int scale, int x_offset, int y_offset){
    // Set the radius and outline depending on scale factor
    dot->setRadius(1.5 + scale / 10);
    dot->setOutlineColor(sf::Color::Black);
    dot->setOutlineThickness(0.1 * scale / 10);
    dot->setOrigin(1,1);

    // Scale and position dots on map based on latitude, longitude, scale factor, x and y offsets
    if(city == "New York"){
        dot->setPosition((((std::stof(latitude) + 74.373828) / 0.732651) * (800 * (1 + scale / 10.0))) + (250 - (scale * 39.9) - (x_offset*(10+scale))),
                         ((40.889949 - std::stof(longitude)) / 0.4137) * (600 * (1 + scale/10.0)) - (scale * 30) - (y_offset*(10+scale)));
    }
    else if(city == "Los Angeles"){
        dot->setPosition((((std::stof(latitude) + 118.951490) / 1.41861) * (800 * (1 + scale / 10.0))) + (250 - (scale * 39.9) - (x_offset*(10+scale))),
                         ((34.457127 - std::stof(longitude)) / 0.922217) * (600 * (1 + scale/10.0)) - (scale * 30) - (y_offset*(10+scale)));
    }
    else if(city == "Chicago"){
        dot->setPosition((((std::stof(latitude) + 88.001141) / 0.747931) * (800 * (1 + scale / 10.0))) + (250 - (scale * 39.9) - (x_offset*(10+scale))),
                         ((42.049659 - std::stof(longitude)) / 0.417422) * (600 * (1 + scale/10.0)) - (scale * 30) - (y_offset*(10+scale)));
    }
    else if(city == "Houston"){
        dot->setPosition((((std::stof(latitude) + 96.077007) / 1.487007) * (800 * (1 + scale / 10.0))) + (250 - (scale * 39.9) - (x_offset*(10+scale))),
                         ((30.205625 - std::stof(longitude)) / 0.971206) * (600 * (1 + scale/10.0)) - (scale * 30) - (y_offset*(10+scale)));
    }
    else if(city == "Phoenix"){
        dot->setPosition((((std::stof(latitude) + 112.832350) / 1.471048) * (800 * (1 + scale / 10.0))) + (250 - (scale * 39.9) - (x_offset*(10+scale))),
                         ((33.916651 - std::stof(longitude)) / 0.916546) * (600 * (1 + scale/10.0)) - (scale * 30) - (y_offset*(10+scale)));
    }
    else if(city == "Philadelphia"){
        dot->setPosition((((std::stof(latitude) + 75.533461) / 0.741084) * (800 * (1 + scale / 10.0))) + (250 - (scale * 39.9) - (x_offset*(10+scale))),
                         ((40.154221 - std::stof(longitude)) / 0.429186) * (600 * (1 + scale/10.0)) - (scale * 30) - (y_offset*(10+scale)));
    }
    else if(city == "Seattle"){
        dot->setPosition((((std::stof(latitude) + 122.704418) / 0.735338) * (800 * (1 + scale / 10.0))) + (250 - (scale * 39.9) - (x_offset*(10+scale))),
                         ((47.770371 - std::stof(longitude)) / 0.370043) * (600 * (1 + scale/10.0)) - (scale * 30) - (y_offset*(10+scale)));
    }
    else if(city == "Detroit"){
        dot->setPosition((((std::stof(latitude) + 83.329453) / 0.551048) * (800 * (1 + scale / 10.0))) + (250 - (scale * 39.9) - (x_offset*(10+scale))),
                         ((42.473015 - std::stof(longitude)) / 0.304626) * (600 * (1 + scale/10.0)) - (scale * 30) - (y_offset*(10+scale)));
    }
    else if(city == "Boston"){
        dot->setPosition((((std::stof(latitude) + 71.416616) / 0.738451) * (800 * (1 + scale / 10.0))) + (250 - (scale * 39.9) - (x_offset*(10+scale))),
                         ((42.540786 - std::stof(longitude)) / 0.406064) * (600 * (1 + scale/10.0)) - (scale * 30) - (y_offset*(10+scale)));
    }
    else if(city == "Miami"){
        dot->setPosition((((std::stof(latitude) + 80.374688) / 0.368266) * (800 * (1 + scale / 10.0))) + (250 - (scale * 39.9) - (x_offset*(10+scale))),
                         ((25.863971 - std::stof(longitude)) / 0.247405) * (600 * (1 + scale/10.0)) - (scale * 30) - (y_offset*(10+scale)));
    }
}

sf::Sprite MenuButton::getSprite(){
    // Get the MenuButton sprite
    return buttonSprite;
}

sf::Text MenuButton::getText(){
    // Get the MenuButton text
    return buttonText;
}

std::string MenuButton::getSelectionText(){
    // Return text as string
    return text;
}

sf::RectangleShape MenuButton::getRect(){
    // Get the button rectangle
    return buttonRectangle;
}

void MenuButton::setPos(int x, int y){
    // Set the sprite position
    buttonSprite.setPosition(x, y);

    // Set the rectangle position
    buttonRectangle.setPosition(x, y);
}

void MenuButton::setTextPos(int x, int y){
    // Set the text position
    buttonText.setPosition(x, y);
}

void MenuButton::setColor(sf::Color color){
    // Set the sprite color
    buttonRectangle.setFillColor(color);
}

bool SelectionButton::getSelection(){
    // Return button selection status
    return selected;
}

void SelectionButton::select(){
    // Set button to selected
    selected = true;
}

void SelectionButton::unselect(){
    // Set button to unselected
    selected = false;
}

Accident::Accident(){
    // Initialize dot object
    selected = false;
    dot = new sf::CircleShape;
    dot->setFillColor(sf::Color::Red);
}

MenuButton::MenuButton(const std::string& inputText, int width, int height){
    // Set selected city text
    text = inputText;

    // Set up button
    sf::IntRect menuButtonRect(0,0,width,height);
    buttonSprite.setTextureRect(menuButtonRect);
    buttonRectangle.setSize(sf::Vector2f(width, height));
    buttonRectangle.setFillColor({225,225,225});

    // Load Open Sans font: https://fonts.google.com/specimen/Open+Sans
    font.loadFromFile("../fonts/Open_Sans/static/OpenSans-Regular.ttf");
    buttonText.setFont(font);
    buttonText.setString(inputText);
    buttonText.setCharacterSize(20);
    buttonText.setFillColor({0,0,0});
}

SelectionButton::SelectionButton(const std::string& inputText, int width, int height)
: MenuButton(inputText, width, height) {
    // Initialize variables
    selected = false;

    // Set up button
    sf::IntRect selectionButtonRect(0,0,width,height);
    buttonSprite.setTextureRect(selectionButtonRect);
    buttonRectangle.setSize(sf::Vector2f(width, height));
    buttonRectangle.setFillColor({225,225,225});

    // Load Open Sans font: https://fonts.google.com/specimen/Open+Sans
    font.loadFromFile("../fonts/Open_Sans/static/OpenSans-Regular.ttf");
    buttonText.setFont(font);
    buttonText.setString(inputText);
    buttonText.setCharacterSize(20);
    buttonText.setFillColor({0,0,0});
}

Window::Window(){
    // Initialize variables, define window, load font
    month = 0;
    loadTextures();
    city = "None";
    structSelect = 0;

    // Load Open Sans font: https://fonts.google.com/specimen/Open+Sans
    font.loadFromFile("../fonts/Open_Sans/static/OpenSans-Regular.ttf");
    window = new sf::RenderWindow(sf::VideoMode(1050, 600), "Car Accident Visualization");
}
