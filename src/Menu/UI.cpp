#include "Menu/UI.hpp"
#include <cstring>

#define FONT_SIZE 14

Texture* loadTexture(string path) {
    Image image;
    Texture* texture = new Texture;

	if(!image.loadFromFile(path)) {
        // LOG(ERROR, "Can't load a texture!");
        exit(1);
    }

    texture->loadFromImage(image);
    return texture;
}

void sprite_loader(Sprite* sprite, Texture* texture, string path, float x, float y) {
    loadTexture(path);
    sprite->setTexture(*texture);
    sprite->setPosition(x, y);
}

RectangleShape* IDrawUI::createRect(Color color, FloatRect bounds) {
    RectangleShape *rect = new RectangleShape;

    rect->setSize(Vector2f(bounds.width, bounds.height));
    rect->setPosition(Vector2f(bounds.left, bounds.top));
    rect->setOrigin(rect->getGlobalBounds().width / 2, rect->getGlobalBounds().height / 2);
    rect->setFillColor(Color(color));

    return rect;
}

void IDrawUI::addText(Font* font, Text *text, String message, Color color, float x, float y) {
    text->setFont(*font);
    text->setCharacterSize(FONT_SIZE);
    text->setString(message);
    text->setFillColor(Color(color));
    text->setPosition(Vector2f(x, y));
}

RectangleShape IDrawUI::createCircleAngleRect(Color color, int width, int height, int x, int y) {
    int radius = height / 2;
    // RectangleShape *rect = createRect(color, FloatRect(width, height, x, y));
    
    CircleShape left_circle(radius), right_circle(radius);
    left_circle.setPosition(x, y + radius);
    right_circle.setPosition(x + width, y + radius);

    // return *rect;
}

Sprite* IDrawUI::createSprite(string texturePath, Vector2f position) {
    Sprite* sprite = new Sprite;

    sprite->setTexture(*loadTexture(texturePath));
    sprite->setPosition(position);
    sprite->setOrigin(sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().height / 2);

    return sprite;
}
