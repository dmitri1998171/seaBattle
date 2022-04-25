#include "Menu/RectButton.hpp"

void RectButton::addObject(Color color, FloatRect bounds) {
    buttons.push_back(*ui.createRect(color, bounds));
    visibleButtons.push_back(buttons.back());
} 

RectangleShape RectButton::getObject(int index) {
    return buttons[index];
}

void RectButton::setVisible(int index, bool isVisible) {
    it = visibleButtons.begin();

    for (int i = 0; i < index; i++)
        it++;

    if (isVisible) {
        visibleButtons.insert(it, buttons[index]);
    }
    else {
        visibleButtons.erase(it);
    }
}

bool RectButton::isVisible(int index) {
    if (visibleButtons[index].getPosition() == buttons[index].getPosition())
        return true;
    else
        return false;
}

void RectButton::draw(RenderWindow* window) {
    for (int i = 0; i < visibleButtons.size(); i++)
        window->draw(visibleButtons[i]);
}
