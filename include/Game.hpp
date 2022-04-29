#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Menu/Menu.hpp"

#define WIDTH 1216
#define HEIGHT 512

#define RECT_SIZE 32
#define OFFSET 3
#define GRID_STEP WIDTH / RECT_SIZE

using namespace std;
using namespace sf;

enum State {MENU, PLAY, WIN, LOSE};

class Map {
    private:
        Font font;
        Text letterLine[20];
        Text numberColumn[20];
        RectangleShape leftBorderBox[4];
        RectangleShape rightBorderBox[4];
        RectangleShape cell[GRID_STEP][GRID_STEP];

    public:

        void setFont(Font* font) {
            this->font = *font;
        }
    
        void createBorderBox(RectangleShape BorderBox[]) {
            BorderBox[0].setPosition(RECT_SIZE * 3, (RECT_SIZE * 3) - OFFSET);
            BorderBox[1].setPosition((RECT_SIZE * 13) + OFFSET, RECT_SIZE * 3);
            BorderBox[1].setRotation(90);
            BorderBox[2].setPosition(RECT_SIZE * 3, (RECT_SIZE * 13) - OFFSET);
            BorderBox[3].setPosition((RECT_SIZE * 3) + OFFSET, RECT_SIZE * 3);
            BorderBox[3].setRotation(90);

            for (int i = 0; i < 4; i++) {
                BorderBox[i].setSize(Vector2f(RECT_SIZE * 10, 5));
                BorderBox[i].setFillColor(Color::Blue);
            }
}

        void createGrid() {
            for(int i = 0; i < GRID_STEP; i++) {
                for(int j = 0; j < GRID_STEP; j++) {
                    cell[i][j].setSize(Vector2f(RECT_SIZE, RECT_SIZE));
                    cell[i][j].setFillColor(Color::White);
                    cell[i][j].setOutlineThickness(1);
                    cell[i][j].setOutlineColor(Color::Black);
                    cell[i][j].setPosition((i * RECT_SIZE) + 1, (j * RECT_SIZE));
                }
            }
}

        void addBorders() {
            createBorderBox(leftBorderBox);
            createBorderBox(rightBorderBox);

            for (int i = 0; i < 4; i++)
                rightBorderBox[i].move(Vector2f(RECT_SIZE * 14, 0)); // Move right BorderBox
        }


        void setBorderBoxText(char symbol, Text *text, RectangleShape rect) {
            text->setFont(font); 
            text->setString(symbol);
            text->setCharacterSize(24); 
            text->setFillColor(sf::Color::Red);
            text->setStyle(sf::Text::Bold);
            text->setPosition(rect.getPosition().x + rect.getSize().x / 4, rect.getPosition().y);
        }

        void addBorderBoxText() {
            string letters = "ABCDEFGHIJ";
            
            for (int i = 0; i < 10; i++) {
                // lines of the letters
                setBorderBoxText(letters[i], &letterLine[i], cell[3 + i][2]);     // Left 
                setBorderBoxText(letters[i], &letterLine[10 + i], cell[17 + i][2]);   // Right 
                
                // Columns of the numbers
                setBorderBoxText(*to_string(i + 1).c_str(), &numberColumn[i], cell[2][3 + i]);    // Left
                setBorderBoxText(*to_string(i + 1).c_str(), &numberColumn[10 + i], cell[16][3 + i]); // Right
            }

            numberColumn[9].setString("10");
            numberColumn[19].setString("10");
        }


        void drawBorderBoxes(RenderWindow *window) {
            for (int i = 0; i < 4; i++) {
                window->draw(leftBorderBox[i]);
                window->draw(rightBorderBox[i]);
            }
}

        void drawGrid(RenderWindow *window) {
            for(int i = 0; i < GRID_STEP; i++) 
                for(int j = 0; j < GRID_STEP; j++) 
                    window->draw(cell[i][j]);
        }

        void drawText(RenderWindow *window) {
            for (int i = 0; i < 20; i++) {
                window->draw(letterLine[i]);
                window->draw(numberColumn[i]);
            }
        }


        RectangleShape* getCell(int i, int j) {
            return &cell[i][j];
        }

        RectangleShape getLeftBorder(int index) {
            return leftBorderBox[index];
        }

        RectangleShape getRightBorder(int index) {
            return rightBorderBox[index];
        }
};

class Ship {
    private:
        Texture texture;
        Sprite sprite;

        void checkBorderCollision(Map* map) {
            for (int k = 0; k < 4; k++) {    // Checking the collision of the ship with the borders
                if(sprite.getGlobalBounds().intersects(map->getLeftBorder(k).getGlobalBounds())) {
                    sprite.setColor(Color::Red);
                    // LOG(WARNING, "The ship have collision with a border!")
                    break;
                }
                else {
                    sprite.setColor(Color::White);
                }
            }
        }

        void checkAnotherShipsCollision(Ship *ship, int chooseIndex) {
            for (int k = 0; k < 10; k++) {    // Checking the distance of the ship with the another ship
                if(chooseIndex != k)
                    if(sprite.getGlobalBounds().left >= ship[k].getShip()->getGlobalBounds().left - 32 && 
                    sprite.getGlobalBounds().left <= ship[k].getShip()->getGlobalBounds().left + 32) {
                        if(sprite.getGlobalBounds().top >= ship[k].getShip()->getGlobalBounds().top - 32 && 
                        sprite.getGlobalBounds().top <= ship[k].getShip()->getGlobalBounds().top + 32)
                            sprite.setColor(Color::Red);
                            // LOG(WARNING, "The ship have collision with another ship!")
                            break;
                    }
            }
        }

    public:
        void setTexture(Texture* _texture) {
            this->texture = *_texture;
            sprite.setTexture(texture);
        }

        void createShip(int denominator) {
            sprite.setTexture(texture);
            sprite.setOrigin(sprite.getLocalBounds().width / denominator, sprite.getLocalBounds().height / 2);
            sprite.setScale(0.6, 0.6);
            sprite.move(16, 16);     // Eccentricity compensation (setOrigin)
        }

        Sprite* getShip() {
            return &sprite;
        }

        void update(Map* map, int i, int j, Ship* ship, int *chooseIndex) {                          
            if((i > 2 && i < 13) && (j > 2 && j < 14)) {    // If mouse click was inside the left BorderBox
                ship[*chooseIndex].getShip()->setPosition(map->getCell(i, j)->getPosition());
                ship[*chooseIndex].getShip()->move(16, 16);   // Eccentricity compensation (setOrigin)

                ship[*chooseIndex].checkBorderCollision(map);
                ship[*chooseIndex].checkAnotherShipsCollision(ship, *chooseIndex);
                
                *chooseIndex = -1;
            }
        }
};

class Game {
    private:
        Map map;
        Ship ship[10];

        int chooseIndex;   // Need for ship placement
        
    public:
        Game() {
            chooseIndex = -1;
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


        void update(RenderWindow* window, Event* event, Menu* menu, State* currentState) {
            while (window->pollEvent(*event)) {
                if (event->type == Event::Closed)
                    window->close();

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

                        for(int i = 0; i < GRID_STEP; i++) {
                            for(int j = 0; j < GRID_STEP; j++) {
                                if(map.getCell(i, j)->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                    map.getCell(i, j)->setFillColor(Color(192, 192, 192));

                                    if(chooseIndex > -1)      // If ship was chosen
                                        ship[chooseIndex].update(&map, i, j, ship, &chooseIndex);
                                    else 
                                        for (int i = 0; i < 10; i++)
                                            if(ship[i].getShip()->getGlobalBounds().contains(mousePos.x, mousePos.y)) // If the mouse click was on a ship
                                                chooseIndex = i;
                                }
                            }
                        }
                    }
                }
            }
        }
};

#endif