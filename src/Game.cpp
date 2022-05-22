#include "Game.hpp"

Game::Game(RenderWindow* window, Texture* texture) {
    this->window = window;
    chooseIndex = -1;
    isPlacemented = false;
    placementCheck = false;
    hited_cell = *texture;

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


Ship* Game::getAllShips() {
    return ship;
}

Ship* Game::getAllComputerShips() {
    return compShip;
}


void Game::createShips(Ship* ship, Texture shipsTexture[]) {
    int size = 1;

    for (int i = 0; i < 10; i++) {
        if (i < 4) {
            size = 1;
            ship[i].getShip()->setPosition(map.getCell(30 + i * 2, 3)->getPosition());
        }
        else if (i < 7) {
            size = 2;
            ship[i].getShip()->setPosition(map.getCell(17 + i * 3, 5)->getPosition());
        }
        else if (i < 9) {
            size = 3;
            ship[i].getShip()->setPosition(map.getCell(2 + i * 4, 7)->getPosition());
        }
        else {
            size = 4;
            ship[i].getShip()->setPosition(map.getCell(33, 9)->getPosition());
        }
        
        ship[i].createShip(shipsTexture, size);
    }
}

void Game::drawShips() {
    for (int i = 0; i < 10; i++)
        window->draw(*ship[i].getShip());
}

void Game::drawComputerShips() {
    for (int i = 0; i < 10; i++) {
        if(compShip[i].hitCount() == compShip[i].getShipSize())
            window->draw(*compShip[i].getShip());
    }
}


void Game::drawOther() {
    if(!isPlacemented) {
        window->draw(autoPlacementButton);
        window->draw(playButton);
    }
}


void Game::shipPlacementStage(Event* event, Menu* menu, State* currentState) {
    if(placementCheck) 
        playButton.setColor(Color(255, 255, 255, 255));

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
                if(placementCheck) 
                    isPlacemented = true;
            }

            if(autoPlacementButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                computersPlacement(ship, false);
                computersPlacement(compShip, true);

                placementCheck = true;
            }

            else {
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

void Game::playingGameStage(Event* event) {
    if(playerTurn) {        // Player move
        if(event->type == Event::MouseButtonReleased) {
            if(event->mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(*window);
                
                for(int i = 0; i < GRID_STEP; i++) {
                    for(int j = 0; j < GRID_STEP; j++) {
                        if(map.getCell(i, j)->getGlobalBounds().contains(mousePos.x, mousePos.y)) {  
                            playingUpdate(i, j, mousePos);
                        }
                    }
                }
            }
        }
    }
    
    else {                  // Computer move
        playerTurn = true;
    }
}

void Game::update(Event* event, Menu* menu, State* currentState) {
    while (window->pollEvent(*event)) {
        if (event->type == Event::Closed)
            window->close();

        if(isPlacemented == false) 
            shipPlacementStage(event, menu, currentState);
        else {
            playingGameStage(event);
            gameOverCheck();
        }
    }
}


void Game::playingUpdate(int i, int j, Vector2i mousePos) {
    if((i > 16 && i < 27) && (j > 2 && j < 13)) {    // If mouse click was inside the right BorderBox
        for (int k = 0; k < 10; k++) {
            if(compShip[k].checkToClick(i, j, mousePos)) {  // If click on the computer's ship
                if(!compShip[k].isKilled()) {
                    cout << "Ship was clicked!" << endl;
                    
                    compShip[k].addHit();

                    if((compShip[k].hitCount() > 0) && (compShip[k].getShipSize() != 1)) 
                        map.setTexture(i, j, &hited_cell);          // Change the texture

                    if(compShip[k].hitCount() == compShip[k].getShipSize())
                        killTheShip(&map, &compShip[k]);
                }
            }
            else {
                if(map.getCell(i, j)->getFillColor() != GREY_COLOR) {
                    cout << "CLICK!" << endl;

                    map.getCell(i, j)->setFillColor(GREY_COLOR);
                    playerTurn = false;
                }
            }
        }
    }
}

void Game::computersPlacement(Ship* _ship, bool isCompShip) {
    for (int i = 9; i >= 0; i--) {     // It's goes from end to start, because big ships easier placement on clear field 
        do {
            _ship[i].autoPlacement(&map, isCompShip);
        } while(!_ship[i].placementRulesCheck(&map, _ship, i));

        cout << i << ") " << "x: " << _ship[i].getCoord().x << "\ty: " << _ship[i].getCoord().y << endl;
    }
    cout << endl;
}

void Game::killTheShip(Map* map, Ship* _ship) {
    _ship->getShip()->setColor(Color::Red);

    int x = _ship->getCoord().x;
    int y = _ship->getCoord().y;
    
    if(x > 17) {
        map->getCell(x - 1, y)->setFillColor(GREY_COLOR);

        if(y < 12)
            map->getCell(x - 1, y + 1)->setFillColor(GREY_COLOR);

        if(y > 3) 
            map->getCell(x - 1, y - 1)->setFillColor(GREY_COLOR);
    }

    for (int i = 1; i <= _ship->getShipSize(); i++) {
        if(x < 26) {
            map->getCell(x + i, y)->setFillColor(GREY_COLOR);

            if(y > 3)
                map->getCell(x + i, y - 1)->setFillColor(GREY_COLOR);
            
            if(y < 12)        
                map->getCell(x + i, y + 1)->setFillColor(GREY_COLOR);
        }
    }

    if(y > 3) 
        map->getCell(x, y - 1)->setFillColor(GREY_COLOR);
    
    if(y < 12)
        map->getCell(x, y + 1)->setFillColor(GREY_COLOR);
}

void Game::gameOverCheck() {

}
