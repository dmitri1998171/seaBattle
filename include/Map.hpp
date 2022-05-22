#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

#define WIDTH 1216
#define HEIGHT 512

#define RECT_SIZE 32
#define OFFSET 3
#define GRID_STEP WIDTH / RECT_SIZE

class Map {
    private:
        RenderWindow* window;
        Font font;
        Text letterLine[20];
        Text numberColumn[20];
        RectangleShape leftBorderBox[4];
        RectangleShape rightBorderBox[4];
        RectangleShape leftCollisionBox[4];
        RectangleShape cell[GRID_STEP][GRID_STEP];

    public:
        void getWindow(RenderWindow* window);

        void setFont(Font* font);
        void setTexture(int i, int j, Texture* texture);
        
        void createBorderBox(RectangleShape BorderBox[], int offset);
        void createGrid();
        void addBorders();

        void setBorderBoxText(char symbol, Text *text, RectangleShape rect);
        void addBorderBoxText();
        void drawBorderBoxes();

        void drawGrid();
        void drawText();

        RectangleShape* getCell(int i, int j);
        RectangleShape getLeftBorder(int index);
        RectangleShape getRightBorder(int index);
};

#endif