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
        Font font;
        Text letterLine[20];
        Text numberColumn[20];
        RectangleShape leftBorderBox[4];
        RectangleShape rightBorderBox[4];
        RectangleShape cell[GRID_STEP][GRID_STEP];

    public:
        void setFont(Font* font);
        void createBorderBox(RectangleShape BorderBox[]);
        void createGrid();
        void addBorders();

        void setBorderBoxText(char symbol, Text *text, RectangleShape rect);
        void addBorderBoxText();
        void drawBorderBoxes(RenderWindow *window);

        void drawGrid(RenderWindow *window);
        void drawText(RenderWindow *window);

        RectangleShape* getCell(int i, int j);
        RectangleShape getLeftBorder(int index);
        RectangleShape getRightBorder(int index);
};

#endif