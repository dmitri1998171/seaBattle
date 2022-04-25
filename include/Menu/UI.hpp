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
        RectangleShape* addBackground(Color color);
        void addText(Font*, Text*, String, Color, float, float);
        Sprite* createSprite(string texturePath, Vector2f position);
        RectangleShape* createRect(Color, FloatRect);
        RectangleShape createCircleAngleRect(Color, int, int, int, int);
};
#endif