#ifndef MENU_HPP
#define MENU_HPP

#include "Menu/UI.hpp"
#include <iostream>

enum menuState {MAIN_MENU, PAUSE, SETTINGS, EXIT};

class Menu : public IDrawUI {
    private:
        menuState state;
        vector<RectangleShape> buttons;         // all buttons
        vector<RectangleShape> visible;         // visible only 
        vector<RectangleShape>::iterator it;

    public:
        Menu();
        int checkToClickRect(RenderWindow* window, RectangleShape rect);
        int checkToClickSprite(RenderWindow* window, Sprite* button);
        void draw(RenderWindow *window);
        void addRectButton(Color Color, FloatRect bounds);
        // void addSpriteButton(Color color, FloatRect bounds);    
        menuState getState();
        void setState(menuState newState);
        RectangleShape getRectButton(int index);
        void setVisible(int index, bool isVisible);
        bool isVisible(int index);
};

Menu::Menu() {
    state = MAIN_MENU;
    buttons.push_back(*addBackground(Color(128, 128, 128)));
    visible.push_back(buttons.back());
    it = visible.begin();
}

int Menu::checkToClickRect(RenderWindow* window, RectangleShape rect) {
    Vector2i mouse_pos = Mouse::getPosition(*window);

    if(rect.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) 
        return 1;
    else 
        return 0;
}

int Menu::checkToClickSprite(RenderWindow* window, Sprite* button) {
    Vector2i mouse_pos = Mouse::getPosition(*window);
    // IntRect rect = button->getTextureRect();

    if(button->getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) 
    // if(rect.contains(mouse_pos)) 
        return 1;
    else 
        return 0;
}

void Menu::draw(RenderWindow *window) {
    for (int i = 0; i < visible.size(); i++)
        window->draw(visible[i]);
}

void Menu::addRectButton(Color color, FloatRect bounds) {
    buttons.push_back(*createRect(color, bounds));
    visible.push_back(buttons.back());
}

// void Menu::addSpriteButton(Color color, FloatRect bounds) {
//     buttons.push_back(*createRect(color, bounds));
// }

menuState Menu::getState() {
    return state;
}

void Menu::setState(menuState newState) {
    this->state = newState;
}

RectangleShape Menu::getRectButton(int index) {
    return buttons[index];
}

void Menu::setVisible(int index, bool isVisible) {
    it = visible.begin();

    if (isVisible) {
        for (int i = 0; i < index; i++)
            it++;
     
        visible.insert(it, buttons[index]);
    }
    else {
        it = visible.begin();

        for (int i = 0; i < index; i++)
            it++;
            
        visible.erase(it);
    }
}

bool Menu::isVisible(int index) {
    if (visible[index].getPosition() == buttons[index].getPosition())
        return true;
    else
        return false;
}


#endif