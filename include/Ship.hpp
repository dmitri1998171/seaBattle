#ifndef SHIP_HPP
#define SHIP_HPP

#include "Map.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

class Ship {
    private:
        Texture texture;
        Sprite sprite;

        void checkBorderCollision(Map* map);
        void checkAnotherShipsCollision(Ship *ship, int chooseIndex);

    public:
        void setTexture(Texture* _texture);
        void createShip(int denominator);
        Sprite* getShip();
        void update(Map* map, int i, int j, Ship* ship, int *chooseIndex, Vector2i mousePos);
};

#endif