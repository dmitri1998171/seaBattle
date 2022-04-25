
#include "Menu/Menu.hpp"

Menu::Menu(Font* font) {
    this->font = *font;
    state = MAIN_MENU;
    background.setFillColor(Color(128, 128, 128));
    background.setPosition(0, 0);
    background.setSize(Vector2f(WIDTH, HEIGHT));
    // background = *addBackground(Color(128, 128, 128));
}


void Menu::addRectButton(Color color, FloatRect bounds) {
    rectButton.addObject(color, bounds);
}

RectangleShape Menu::getRectButton(int index) {
    return rectButton.getObject(index);
}

RectButton Menu::RectButton() {
    return rectButton;
}


void Menu::addSpriteButton(string texturePath, Vector2f position) {
    spriteButton.addObject(texturePath, position);
}

SpriteButton* Menu::getSpriteButton() {
    return &spriteButton;
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

void Menu::drawTexts(RenderWindow *window) {
    for (int i = 0; i < texts.size(); i++)
        window->draw(texts[i]);
}


void Menu::setState(menuState newState) {
    this->state = newState;
}

menuState Menu::getState() {
    return state;
}

void Menu::draw(RenderWindow *window) {
    window->draw(background);
    // for (int i = 0; i < visibleRects.size(); i++)
    //     window->draw(visibleRects[i]);

    rectButton.draw(window);
    // spriteButton.draw(window);
    drawTexts(window);
    
}
