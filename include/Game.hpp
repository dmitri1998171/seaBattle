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
        IDrawUI ui;
        Map map;
        Ship ship[10];

        RenderWindow* window;
        Sprite playButton;
        int chooseIndex;   // Need for ship placement
        bool isPlacemented;
        bool placementCheck;
        
    public:
        Game(RenderWindow* window);
        
        void createMap(Font* font);
        void drawMap();

        void createShips(Texture shipsTexture[]);
        void drawShips();
        
        void drawOther();
        void update(Event* event, Menu* menu, State* currentState);
};

#endif