#include "Menu/SpriteButton.hpp"

void SpriteButton::addObject(string texturePath, Vector2f position) {
    buttons.push_back(*createSprite(texturePath, position));
    visibleButtons.push_back(buttons.back());
} 

Sprite SpriteButton::getObject(int index) {
    return buttons[index];
}

void SpriteButton::setVisible(int index, bool isVisible) {
    it = visibleButtons.begin();

    if (isVisible) {
        for (int i = 0; i < index; i++)
            it++;
     
        visibleButtons.insert(it, buttons[index]);
    }
    else {
        it = visibleButtons.begin();

        for (int i = 0; i < index; i++)
            it++;
            
        visibleButtons.erase(it);
    }
}

bool SpriteButton::isVisible(int index) {
    if (visibleButtons[index].getTexture() == buttons[index].getTexture())
        return true;
    else
        return false;
}

void SpriteButton::draw(RenderWindow* window) {
    for (int i = 0; i < visibleButtons.size(); i++)
        window->draw(visibleButtons[i]);
}

int SpriteButton::capacity() {
    return buttons.size();
}

int SpriteButton::size() {
    return visibleButtons.size();
}