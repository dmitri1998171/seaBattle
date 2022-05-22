#ifndef SHIP_HPP
#define SHIP_HPP

#include "Map.hpp"
#include "SFML/Graphics.hpp"

using namespace sf;

class Ship {
    private:
        bool _isPlaced;
        bool _isKilled;
        int hitTheShip;
        int shipSize;
        int *cellsArr;
        Texture texture;
        Sprite sprite;
        Vector2i coord;

        bool checkBorderCollision(Map* map);
        bool checkAnotherShipsCollision(Ship *ship, int chooseIndex);

    public:
        Ship();

        void createShip(Texture shipsTexture[], int size);
        Sprite* getShip();
        
        void update(Map* map, int i, int j, Ship* ship, int *chooseIndex, Vector2i mousePos, bool* placementCheck);
        
        void setCoord(Vector2i coord);
        Vector2i getCoord();

        void setPlaceState(bool state);
        bool isPlaced();
        bool allShipsPlaced(Ship* ship);

        void setKillState(bool state);
        bool isKilled();
        int getShipSize();

        void addHit();
        int hitCount();

        void autoPlacement(Map* map, bool isCompShip);
        bool placementRulesCheck(Map* map, Ship* ship, int chooseIndex);

        bool checkToClick(int i, int j, Vector2i mousePos);
};

#endif