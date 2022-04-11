// #include "assets.hpp"
#ifndef UI_HPP
#define UI_HPP

#include <SFML/Graphics.hpp>
#include <string>

#define WIDTH 1216
#define HEIGHT 512

using namespace sf;
using namespace std;

Texture loadTexture(string path);
void sprite_loader(Sprite* sprite, Texture* texture, string path, float x, float y);

class IDrawUI {
    public:
        void addBackground(RectangleShape* background, Color color);
        void addText(Font*, Text*, String, Color, float, float);
        Sprite addButton(string texturePath, Vector2f position);
        void createRect(RectangleShape*, Color, FloatRect);
        void createCircleAngleRect(RectangleShape*, Color, int, int, int, int);
};
#endif