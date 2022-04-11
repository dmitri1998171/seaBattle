#ifndef MENU_HPP
#define MENU_HPP

#include "Menu/UI.hpp"

enum menuState {MAIN_MENU, PAUSE, SETTINGS, EXIT};

class Menu : public IDrawUI {
    private:
        menuState state;
        RectangleShape backgroundRect;

    public:
        Menu();
        int checkToClickRect(RenderWindow* window, RectangleShape rect);
        int checkToClickSprite(RenderWindow* window, Sprite* button);
        void draw(RenderWindow *window);
        menuState getState();
        void setState(menuState newState);
};

Menu::Menu() {
    state = MAIN_MENU;
    addBackground(&backgroundRect, Color(128, 128, 128));
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
    window->draw(backgroundRect);

    // if (state == MAIN_MENU) {
    //     window->draw(newGameButton);
    //     window->draw();
    //     window->draw();
    // }
    // if (state == SETTINGS)

    // if (state == EXIT)
            
}

menuState Menu::getState() {
    return state;
}

void Menu::setState(menuState newState) {
    this->state = newState;
}

#endif