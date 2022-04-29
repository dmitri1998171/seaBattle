
#include "Ship.hpp"

void Ship::checkBorderCollision(Map* map) {
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

void Ship::checkAnotherShipsCollision(Ship *ship, int chooseIndex) {
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

void Ship::update(Map* map, int i, int j, Ship* ship, int *chooseIndex, Vector2i mousePos) {     
    if(*chooseIndex > -1) {    // If ship was chosen

        if((i > 2 && i < 13) && (j > 2 && j < 14)) {    // If mouse click was inside the left BorderBox
            ship[*chooseIndex].getShip()->setPosition(map->getCell(i, j)->getPosition());
            ship[*chooseIndex].getShip()->move(16, 16);   // Eccentricity compensation (setOrigin)

            ship[*chooseIndex].checkBorderCollision(map);
            ship[*chooseIndex].checkAnotherShipsCollision(ship, *chooseIndex);
            
            *chooseIndex = -1;
        }
    }
    else 
        for (int i = 0; i < 10; i++)
            if(ship[i].getShip()->getGlobalBounds().contains(mousePos.x, mousePos.y)) // If the mouse click was on a ship
                *chooseIndex = i;
}
