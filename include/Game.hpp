#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Menu/Menu.hpp"
#include "Map.hpp"
#include "Ship.hpp"

using namespace std;
using namespace sf;

enum State {MENU, PLAY, WIN, LOSE};

class Game {
    private:
        Map map;
        Ship ship[10];

        IDrawUI ui;
        Sprite playButton;
        int chooseIndex;   // Need for ship placement
        bool isPlacemented = false;
        bool placementCheck = false;
        
    public:
        Game() {
            chooseIndex = -1;
            playButton = *ui.createSprite("./media/img/play_button.png", Vector2f(WIDTH - 96, HEIGHT - 96));
            playButton.setColor(Color(255, 255, 255, 100));
        }

        void createMap(Font* font) {
            map.setFont(font);
            map.createGrid();
            map.addBorders();
            map.addBorderBoxText();
        }

        void drawMap(RenderWindow *window) {
            map.drawGrid(window);
            map.drawBorderBoxes(window);
            map.drawText(window);
        }


        void createShips(Texture shipsTexture[]) {
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

        void drawShips(RenderWindow *window) {
            for (int i = 0; i < 10; i++)
                window->draw(*ship[i].getShip());
        }

        void drawOther(RenderWindow *window) {
            if(!isPlacemented)
                window->draw(playButton);
            
        }
        

        void update(RenderWindow* window, Event* event, Menu* menu, State* currentState) {
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
                                        // map.getCell(i, j)->setFillColor(Color(192, 192, 192));
                                        ship[chooseIndex].update(&map, i, j, ship, &chooseIndex, mousePos, &placementCheck);
                                    }
                                }
                            }
                        }
                    }
                }
                else {      // Playing the game stage

                }
            }
        }
};

#endif