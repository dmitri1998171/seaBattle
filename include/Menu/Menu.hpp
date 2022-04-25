#ifndef MENU_HPP
#define MENU_HPP

#include "Menu/UI.hpp"
#include "Menu/RectButton.hpp"
#include "Menu/SpriteButton.hpp"
#include <iostream>

#define WIDTH 1216
#define HEIGHT 512

enum menuState {MAIN_MENU, PAUSE, SETTINGS, EXIT};

class Menu {
    private:
        Font font;
        menuState state;
        RectangleShape background;

        RectButton rectButton;
        SpriteButton spriteButton;

        vector<Sprite> spriteButtons;           // all sprite buttons
        vector<Sprite> visibleSprites;          // visible sprites only 
        vector<Text> texts; 
        vector<RectangleShape>::iterator it;

    public:
        Menu(Font* font);
        
        void addRectButton(Color Color, FloatRect bounds);
        RectangleShape getRectButton(int index);
        RectButton RectButton();

        void addSpriteButton(string texturePath, Vector2f position);
        SpriteButton* getSpriteButton();
        
        void addText(Vector2f position, string str, int size, Color color, Uint32 style);
        Text* getText(int index);
        void drawTexts(RenderWindow *window);

        void setState(menuState newState);
        menuState getState();

        template<class T>
        bool checkToClick(RenderWindow* window, T rect) {
            Vector2i mouse_pos = Mouse::getPosition(*window);

            if(rect.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) 
                return true;
            else 
                return false;
        }

        void draw(RenderWindow *window);
};

#endif