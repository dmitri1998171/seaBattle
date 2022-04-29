#include "Map.hpp"


void Map::setFont(Font* font) {
    this->font = *font;
}

void Map::createBorderBox(RectangleShape BorderBox[]) {
    BorderBox[0].setPosition(RECT_SIZE * 3, (RECT_SIZE * 3) - OFFSET);
    BorderBox[1].setPosition((RECT_SIZE * 13) + OFFSET, RECT_SIZE * 3);
    BorderBox[1].setRotation(90);
    BorderBox[2].setPosition(RECT_SIZE * 3, (RECT_SIZE * 13) - OFFSET);
    BorderBox[3].setPosition((RECT_SIZE * 3) + OFFSET, RECT_SIZE * 3);
    BorderBox[3].setRotation(90);

    for (int i = 0; i < 4; i++) {
        BorderBox[i].setSize(Vector2f(RECT_SIZE * 10, 5));
        BorderBox[i].setFillColor(Color::Blue);
    }
}

void Map::createGrid() {
    for(int i = 0; i < GRID_STEP; i++) {
        for(int j = 0; j < GRID_STEP; j++) {
            cell[i][j].setSize(Vector2f(RECT_SIZE, RECT_SIZE));
            cell[i][j].setFillColor(Color::White);
            cell[i][j].setOutlineThickness(1);
            cell[i][j].setOutlineColor(Color::Black);
            cell[i][j].setPosition((i * RECT_SIZE) + 1, (j * RECT_SIZE));
        }
    }
}

void Map::addBorders() {
    createBorderBox(leftBorderBox);
    createBorderBox(rightBorderBox);

    for (int i = 0; i < 4; i++)
        rightBorderBox[i].move(Vector2f(RECT_SIZE * 14, 0)); // Move right BorderBox
}


void Map::setBorderBoxText(char symbol, Text *text, RectangleShape rect) {
    text->setFont(font); 
    text->setString(symbol);
    text->setCharacterSize(24); 
    text->setFillColor(sf::Color::Red);
    text->setStyle(sf::Text::Bold);
    text->setPosition(rect.getPosition().x + rect.getSize().x / 4, rect.getPosition().y);
}

void Map::addBorderBoxText() {
    string letters = "ABCDEFGHIJ";
    
    for (int i = 0; i < 10; i++) {
        // lines of the letters
        setBorderBoxText(letters[i], &letterLine[i], cell[3 + i][2]);     // Left 
        setBorderBoxText(letters[i], &letterLine[10 + i], cell[17 + i][2]);   // Right 
        
        // Columns of the numbers
        setBorderBoxText(*to_string(i + 1).c_str(), &numberColumn[i], cell[2][3 + i]);    // Left
        setBorderBoxText(*to_string(i + 1).c_str(), &numberColumn[10 + i], cell[16][3 + i]); // Right
    }

    numberColumn[9].setString("10");
    numberColumn[19].setString("10");
}


void Map::drawBorderBoxes(RenderWindow *window) {
    for (int i = 0; i < 4; i++) {
        window->draw(leftBorderBox[i]);
        window->draw(rightBorderBox[i]);
    }
}

void Map::drawGrid(RenderWindow *window) {
    for(int i = 0; i < GRID_STEP; i++) 
        for(int j = 0; j < GRID_STEP; j++) 
            window->draw(cell[i][j]);
}

void Map::drawText(RenderWindow *window) {
    for (int i = 0; i < 20; i++) {
        window->draw(letterLine[i]);
        window->draw(numberColumn[i]);
    }
}


RectangleShape* Map::getCell(int i, int j) {
    return &cell[i][j];
}

RectangleShape Map::getLeftBorder(int index) {
    return leftBorderBox[index];
}

RectangleShape Map::getRightBorder(int index) {
    return rightBorderBox[index];
}