#ifndef SHIP_HPP
#define SHIP_HPP

#include "Map.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

class Ship {
    private:
        Texture texture;
        Sprite sprite;
        bool _isPlaced;
        int shipSize;

        bool checkBorderCollision(Map* map);
        bool checkAnotherShipsCollision(Ship *ship, int chooseIndex);

    public:
        Ship();
        void setTexture(Texture* _texture);
        void createShip(int size, float denominator);
        Sprite* getShip();
        void update(Map* map, int i, int j, Ship* ship, int *chooseIndex, Vector2i mousePos, bool* placementCheck);
        void setPlaceState(bool state);
        bool isPlaced();
        bool allShipsPlaced(Ship* ship);
        void autoPlacement(Map* map);
        bool placementRulesCheck(Map* map, Ship* ship, int chooseIndex);
};

#endif