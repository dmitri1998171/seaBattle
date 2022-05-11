
#include "Ship.hpp"

Ship::Ship() {
    _isPlaced = false;
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

void Ship::setTexture(Texture* _texture) {
    this->texture = *_texture;
    sprite.setTexture(texture);
}

void Ship::createShip(int size, float denominator) {
    shipSize = size;

    sprite.setTexture(texture);
    sprite.setOrigin(sprite.getLocalBounds().width / denominator, sprite.getLocalBounds().height / 2);
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

void Ship::autoPlacement(Map* map) {
    int x = 3 + rand() % 10;
    int y = 3 + rand() % 10;

    cout << "X: " << x - 2 << " Y: " << y - 2 << endl;

    sprite.setColor(Color(255, 255, 255, 255));
    sprite.setPosition(map->getCell(x, y)->getPosition());
    sprite.move(16, 16);

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