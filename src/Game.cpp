#include "Game.hpp"

Game::Game(RenderWindow* window) {
    this->window = window;
    chooseIndex = -1;
    isPlacemented = false;
    placementCheck = false;

    map.getWindow(this->window);

    playButton = *ui.createSprite("./media/img/play_button.png", Vector2f(WIDTH - 64, HEIGHT - 96));
    autoPlacementButton = *ui.createSprite("./media/img/auto_placement.png", Vector2f(WIDTH - 160, HEIGHT - 96));
    playButton.setColor(Color(255, 255, 255, 100));
}


void Game::createMap(Font* font) {
    map.setFont(font);
    map.createGrid();
    map.addBorders();
    map.addBorderBoxText();
}

void Game::drawMap() {
    map.drawGrid();
    map.drawBorderBoxes();
    map.drawText();
}


void Game::createShips(Texture shipsTexture[]) {
    int size = 1;
    float denominator = 2;

    for (int i = 0; i < 10; i++) {
        if (i < 4) {
            size = 1;
            denominator = 2;
            ship[i].setTexture(&shipsTexture[0]);
            ship[i].getShip()->setPosition(map.getCell(30 + i * 2, 3)->getPosition());
        }
        else if (i < 7) {
            size = 2;
            denominator = 4;
            ship[i].setTexture(&shipsTexture[1]);
            ship[i].getShip()->setPosition(map.getCell(17 + i * 3, 5)->getPosition());
        }
        else if (i < 9) {
            size = 3;
            denominator = 2;
            ship[i].setTexture(&shipsTexture[2]);
            ship[i].getShip()->setPosition(map.getCell(3 + i * 4, 7)->getPosition());
        }
        else {
            size = 4;
            denominator = 8;
            ship[i].setTexture(&shipsTexture[3]);
            ship[i].getShip()->setPosition(map.getCell(33, 9)->getPosition());
        }
        
        ship[i].createShip(size, denominator);
    }
    
}

void Game::drawShips() {
    for (int i = 0; i < 10; i++)
        window->draw(*ship[i].getShip());
}


void Game::drawOther() {
    if(!isPlacemented) {
        window->draw(playButton);
        window->draw(autoPlacementButton);
    }
}

void Game::update(Event* event, Menu* menu, State* currentState) {
    while (window->pollEvent(*event)) {
        if (event->type == Event::Closed)
            window->close();

        if(isPlacemented == false) {    // the Ships placement stage

            if(placementCheck) 
                playButton.setColor(Color(255, 255, 255, 255));
            else
                playButton.setColor(Color(255, 255, 255, 100));

            if(event->type == Event::KeyReleased) {
                if(event->key.code == Keyboard::R) {
                    if(chooseIndex > -1) { // If ship was chosen
                        ship[chooseIndex].getShip()->setRotation(ship[chooseIndex].getShip()->getRotation() + 90);
                        chooseIndex = -1;
                    }
                }
            
                if(event->key.code == Keyboard::Escape) {
                    *currentState = MENU;
                    menu->setState(PAUSE);
                }
            }

            if(event->type == Event::MouseButtonReleased) {
                if(event->mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(*window);

                    if(playButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        cout << "!" << endl;
                        
                        if(placementCheck) 
                            isPlacemented = true;
                    }

                    else if(autoPlacementButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        cout << "!!" << endl;

                        for (int i = 10; i >= 0; i--) {     // It's goes from end to start, because big ships easier placement on clear field 
                            do {
                                ship[i].autoPlacement(&map, ship, i);
                            } while(!(placementCheck = ship[i].placementRulesCheck(&map, ship, i)));
                        }

                        // placementCheck = ship[0].allShipsPlaced(ship);
                        
                        cout << endl;
                    }

                    else {
                        cout << "!!!" << endl;

                        for(int i = 0; i < GRID_STEP; i++) {
                            for(int j = 0; j < GRID_STEP; j++) {
                                if(map.getCell(i, j)->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                    ship[chooseIndex].update(&map, i, j, ship, &chooseIndex, mousePos, &placementCheck);
                                }
                            }
                        }
                    }
                }
            }
        }
        else {      // Playing the game stage
            if(playerTurn) {        // Player move
                if(event->type == Event::MouseButtonReleased) {
                    if(event->mouseButton.button == Mouse::Left) {
                        Vector2i mousePos = Mouse::getPosition(*window);
                        
                        for(int i = 0; i < GRID_STEP; i++) {
                            for(int j = 0; j < GRID_STEP; j++) {
                                if(map.getCell(i, j)->getGlobalBounds().contains(mousePos.x, mousePos.y)) {  
                                    if(ship[i].getShip()->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                    /*
                                        kill the ship logic
                                    */ 
                                    }
                                    else {
                                        if(map.getCell(i, j)->getFillColor() != Color(192, 192, 192)) {
                                            map.getCell(i, j)->setFillColor(Color(192, 192, 192));
                                            playerTurn = false;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else {                  // Computer move

            }
        }
    }
}
