#include "Game.hpp"

Game::Game(RenderWindow* window) {
    this->window = window;
    chooseIndex = -1;
    isPlacemented = false;
    placementCheck = false;

    map.getWindow(this->window);

    playButton = *ui.createSprite("./media/img/play_button.png", Vector2f(WIDTH - 96, HEIGHT - 96));
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
    int denominator = 2;

    for (int i = 0; i < 10; i++) {
        if (i < 4) {
            denominator = 2;
            ship[i].setTexture(&shipsTexture[0]);
            ship[i].getShip()->setPosition(map.getCell(30 + i * 2, 3)->getPosition());
        }
        else if (i < 7) {
            denominator = 4;
            ship[i].setTexture(&shipsTexture[1]);
            ship[i].getShip()->setPosition(map.getCell(17 + i * 3, 5)->getPosition());
        }
        else if (i < 9) {
            denominator = 2;
            ship[i].setTexture(&shipsTexture[2]);
            ship[i].getShip()->setPosition(map.getCell(3 + i * 4, 7)->getPosition());
        }
        else {
            denominator = 8;
            ship[i].setTexture(&shipsTexture[3]);
            ship[i].getShip()->setPosition(map.getCell(33, 9)->getPosition());
        }
        
        ship[i].createShip(denominator);
    }
    
}

void Game::drawShips() {
    for (int i = 0; i < 10; i++)
        window->draw(*ship[i].getShip());
}


void Game::drawOther() {
    if(!isPlacemented)
        window->draw(playButton);
    
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
                if(event->key.code == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(*window);

                    if(playButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        if(placementCheck) 
                            isPlacemented = true;
                    }

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
        else {      // Playing the game stage
            if(playerTurn) {        // Player move
                if(event->type == Event::MouseButtonReleased) {
                    if(event->key.code == Mouse::Left) {
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
