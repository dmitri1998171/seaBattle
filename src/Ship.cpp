
#include "Ship.hpp"

Ship::Ship() {
    _isPlaced = false;
    sprite.setColor(Color(255, 255, 255, 100));
}

bool Ship::checkBorderCollision(Map* map) {
    for (int k = 0; k < 4; k++) {    // Checking the collision of the ship with the borders
        if(sprite.getGlobalBounds().intersects(map->getLeftBorder(k).getGlobalBounds())) {
            sprite.setColor(Color::Red);
            // LOG(WARNING, "The ship have collision with a border!")
            return false;
        }
        else {
            sprite.setColor(Color::White);
            return true;
        }
    }
}

bool Ship::checkAnotherShipsCollision(Ship *ship, int chooseIndex) {
    for (int k = 0; k < 10; k++) {    // Checking the distance of the ship with the another ship
        if(chooseIndex != k) {
            if(sprite.getGlobalBounds().left >= ship[k].getShip()->getGlobalBounds().left - 32 && 
            sprite.getGlobalBounds().left <= ship[k].getShip()->getGlobalBounds().left + 32) {
                if(sprite.getGlobalBounds().top >= ship[k].getShip()->getGlobalBounds().top - 32 && 
                sprite.getGlobalBounds().top <= ship[k].getShip()->getGlobalBounds().top + 32) {
                    sprite.setColor(Color::Red);
                    // LOG(WARNING, "The ship have collision with another ship!")
                    break;
                    return false;
                }
            }
        }
    }
    return true;
}

void Ship::setTexture(Texture* _texture) {
    this->texture = *_texture;
    sprite.setTexture(texture);
}

void Ship::createShip(int denominator) {
    sprite.setTexture(texture);
    sprite.setOrigin(sprite.getLocalBounds().width / denominator, sprite.getLocalBounds().height / 2);
    sprite.setScale(0.6, 0.6);
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

            *placementCheck = checkBorderCollision(map);
            *placementCheck = checkAnotherShipsCollision(ship, *chooseIndex);
            *placementCheck = allShipsPlaced(ship);

            *chooseIndex = -1;
        }
    }
    else 
        for (int i = 0; i < 10; i++)    // The ship was chosen now
            if(ship[i].getShip()->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                ship[i].getShip()->setColor(Color(255, 255, 255, 255));
                *chooseIndex = i;
            }
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

