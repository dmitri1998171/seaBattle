#include "Ship.hpp"

void selectRandCell(Vector2i* randCoord, bool isCompShip) {
    randCoord->x = 3 + rand() % 9;
    randCoord->y = 3 + rand() % 9;

    if(isCompShip) 
        randCoord->x = 17 + rand() % 7;
}

Ship::Ship() {
    _isPlaced = false;
    _isKilled = false;
    hitTheShip = 0;
    sprite.setColor(Color(255, 255, 255, 100));
}

bool Ship::checkBorderCollision(Map* map) {
    for (int k = 0; k < 4; k++) {    // Checking the collision of the ship with the borders
        if(sprite.getGlobalBounds().intersects(map->getLeftBorder(k).getGlobalBounds())) {
            // LOG(WARNING, "The ship have collision with a border!")
            return false;
        }
    }

    return true;
}

bool Ship::checkAnotherShipsCollision(Ship *ship, int chooseIndex) {
    for (int k = 0; k < 10; k++) {    // Checking the distance of the ship with the another ship
        if(chooseIndex != k) {
            if(sprite.getGlobalBounds().intersects(ship[k].getShip()->getGlobalBounds())) {
                // LOG(WARNING, "The ship have collision with another ship!")
                return false;
            }
        }
    }

    // sprite.setColor(Color::White);
    return true;
}


void Ship::createShip(Texture shipsTexture[], int size) {
    shipSize = size;

    this->texture = shipsTexture[size - 1];
    sprite.setTexture(shipsTexture[size - 1]);

    sprite.setOrigin(sprite.getLocalBounds().width / (size * 2), sprite.getLocalBounds().height / 2);
    sprite.setScale(0.63, 0.63);
    sprite.move(16, 16);     // Eccentricity compensation (setOrigin)
}

Sprite* Ship::getShip() {
    return &sprite;
}


void Ship::update(Map* map, int i, int j, Ship* ship, int *chooseIndex, Vector2i mousePos, bool* placementCheck) {     
    if(*chooseIndex > -1) {    // If ship already chosen
        if((i > 2 && i < 13) && (j > 2 && j < 14)) {    // If mouse click was inside the left BorderBox
            _isPlaced = true;
            sprite.setPosition(map->getCell(i, j)->getPosition());
            sprite.move(16, 16);   // Eccentricity compensation (setOrigin)
        
            setCoord(Vector2i(i, j));

            *placementCheck = placementRulesCheck(map, ship, *chooseIndex);
            *placementCheck = allShipsPlaced(ship);
            *chooseIndex = -1;
        }
    }
    else 
        for (int i = 0; i < 10; i++)    // The ship was chosen now
            if(ship[i].getShip()->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                ship[i].getShip()->setColor(Color(0, 255, 0, 255));
                *chooseIndex = i;
                break;
            }
}


void Ship::setCoord(Vector2i coord) {
    this->coord = coord;
}

Vector2i Ship::getCoord() {
    return coord;
}


void Ship::setPlaceState(bool state) {
    _isPlaced = state;
}

bool Ship::isPlaced() {
    return _isPlaced;
}

bool Ship::allShipsPlaced(Ship* ship) {
    for (int i = 0; i < 10; i++)
        if(ship[i].isPlaced() == false)
            return false;
    
    return true;
}


void Ship::setKillState(bool state) {
    _isKilled = state;
}

bool Ship::isKilled() {
    return _isKilled;
}

int Ship::getShipSize() {
    return shipSize;
}


void Ship::addHit() {
    hitTheShip++;
}

int Ship::hitCount() {
    return hitTheShip;
}


void Ship::autoPlacement(Map* map, bool isCompShip) {
    Vector2i randCoord;
    selectRandCell(&randCoord, isCompShip);
        
    sprite.setColor(Color(255, 255, 255, 255));
    sprite.setPosition(map->getCell(randCoord.x, randCoord.y)->getPosition());
    sprite.move(16, 16);

    setCoord(Vector2i(randCoord.x, randCoord.y));

    // if(0 + rand() % 2) 
    //     sprite.setRotation(sprite.getRotation() - 90);
}


bool Ship::placementRulesCheck(Map* map, Ship* ship, int chooseIndex) {
    if(checkBorderCollision(map) && checkAnotherShipsCollision(ship, chooseIndex)) {
        sprite.setColor(Color::White);
        return true;
    }

    sprite.setColor(Color::Red);
    return false;
}

bool Ship::checkToClick(int i, int j) {
    if(getCoord().y == j) {
        for (int k = 0; k < getShipSize(); k++) {
            if(i == getCoord().x + k)
                return true;
        }
    } 

    return false;
}

