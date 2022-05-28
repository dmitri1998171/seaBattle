#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <unistd.h>
#include "SFML/Graphics.hpp"
#include "Menu/Menu.hpp"
#include "Map.hpp"
#include "Ship.hpp"

using namespace std;
using namespace sf;

#define GREY_COLOR Color(192, 192, 192)

enum State {MENU, PLAY, WIN, LOSE};

class Game {
    private:
        IDrawUI ui;
        Map map;
        Ship ship[10];          // Player's ships
        Ship compShip[10];      // computer's ships

        State currentState;
        RenderWindow* window;
        Texture hited_cell;
        Sprite playButton, autoPlacementButton;
        bool isPlacemented;  // It turn 'setting' and 'playing' substates into the PLAY game state 
        bool placementCheck; // It check placement for all ships. Include border collision check and collision chosen ship with another ships check
        bool playerTurn;     // It turn Player move and Computer move 
        int chooseIndex;     // It select ship for placement
        int killedShipsCount;
        int killedCompShipsCount;
        float stepDelay;

    public:
        Game(RenderWindow* window, Texture* texture);
        
        State getState();
        void setState(State state);

        void createMap(Font* font);
        void drawMap();

        Ship* getAllShips();
        Ship* getAllComputerShips();

        void createShips(Ship* ship, Texture shipsTexture[]);
        void drawShips();
        void drawComputerShips();
        
        void drawOther();
        
        void shipPlacementStage(Event* event, Menu* menu);
        void playingGameStage(Event* event);
        void update(Event* event, Menu* menu);
        void playingUpdate(int i, int j, Vector2i mousePos);

        void computersPlacement(Ship* ship, bool isCompShip);
        void killTheShip(Map* map, Ship* _ship);
        void gameOverCheck();
};

#endif