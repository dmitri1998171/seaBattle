#ifndef MENU_HPP
#define MENU_HPP

#include "Menu/UI.hpp"
#include <iostream>

enum menuState {MAIN_MENU, PAUSE, SETTINGS, EXIT};

class Menu : public IDrawUI {
    private:
        Font font;
        menuState state;
        vector<RectangleShape> buttons;         // all rectangle shape buttons
        vector<Sprite> spriteButtons;           // all sprite buttons
        vector<RectangleShape> visibleRects;    // visible rectangle shapes only 
        vector<Sprite> visibleSprites;          // visible sprites only 
        vector<Text> texts; 
        vector<RectangleShape>::iterator it;

    public:
        Menu(Font* font);
        void addRectButton(Color Color, FloatRect bounds);
        RectangleShape getRectButton(int index);
        int checkToClickRect(RenderWindow* window, RectangleShape rect);
        void setVisible(int index, bool isVisible);
        bool isVisible(int index);
        void addSpriteButton(Texture* texture, Vector2f position);
        Sprite getSpriteButton(int index);
        int checkToClickSprite(RenderWindow* window, Sprite button);
        void addText(Vector2f position, string str, int size, Color color, Uint32 style);
        Text* getText(int index);
        void setState(menuState newState);
        menuState getState();
        void draw(RenderWindow *window);
};

Menu::Menu(Font* font) {
    this->font = *font;
    state = MAIN_MENU;
    buttons.push_back(*addBackground(Color(128, 128, 128)));
    visibleRects.push_back(buttons.back());
    it = visibleRects.begin();
}

void Menu::addRectButton(Color color, FloatRect bounds) {
    buttons.push_back(*createRect(color, bounds));
    visibleRects.push_back(buttons.back());
}

RectangleShape Menu::getRectButton(int index) {
    return buttons[index];
}

int Menu::checkToClickRect(RenderWindow* window, RectangleShape rect) {
    Vector2i mouse_pos = Mouse::getPosition(*window);

    if(rect.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) 
        return 1;
    else 
        return 0;
}

void Menu::setVisible(int index, bool isVisible) {
    it = visibleRects.begin();

    if (isVisible) {
        for (int i = 0; i < index; i++)
            it++;
     
        visibleRects.insert(it, buttons[index]);
    }
    else {
        it = visibleRects.begin();

        for (int i = 0; i < index; i++)
            it++;
            
        visibleRects.erase(it);
    }
}

bool Menu::isVisible(int index) {
    if (visibleRects[index].getPosition() == buttons[index].getPosition())
        return true;
    else
        return false;
}

void Menu::addSpriteButton(Texture* texture, Vector2f position) {
    Sprite sprite;

    sprite.setTexture(*texture);
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setPosition(position);

    spriteButtons.push_back(sprite);
    visibleSprites.push_back(spriteButtons.back());
}

Sprite Menu::getSpriteButton(int index) {
    return spriteButtons[index];
}

int Menu::checkToClickSprite(RenderWindow* window, Sprite button) {
    Vector2i mouse_pos = Mouse::getPosition(*window);

    if(button.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) 
        return 1;
    else 
        return 0;
}

void Menu::addText(Vector2f position, string str, int size, Color color, Uint32 style) {
    Text text;
    
    text.setFont(font); 
    text.setString(str);
    text.setCharacterSize(size); 
    text.setFillColor(color);
    text.setStyle(style);
    text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
    text.setPosition(position);

    texts.push_back(text);
}

Text* Menu::getText(int index) {
    return &texts[index];
}

void Menu::setState(menuState newState) {
    this->state = newState;
}

menuState Menu::getState() {
    return state;
}

void Menu::draw(RenderWindow *window) {
    for (int i = 0; i < visibleRects.size(); i++)
        window->draw(visibleRects[i]);
    
    for (int i = 0; i < visibleSprites.size(); i++)
        window->draw(visibleSprites[i]);

    for (int i = 0; i < texts.size(); i++)
        window->draw(texts[i]);
}

#endif