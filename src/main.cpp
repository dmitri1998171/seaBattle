#include <SFML/Graphics.hpp>
#include <MyLogger.hpp>

#define WIDTH 1216
#define HEIGHT 512

#define RECT_SIZE 32
#define OFFSET 3
#define GRID_STEP WIDTH / RECT_SIZE

using namespace sf;

void createBox(RectangleShape box[]) {
    box[0].setPosition(RECT_SIZE * 3, (RECT_SIZE * 3) - OFFSET);
    box[1].setPosition((RECT_SIZE * 13) + OFFSET, RECT_SIZE * 3);
    box[1].setRotation(90);
    box[2].setPosition(RECT_SIZE * 3, (RECT_SIZE * 13) - OFFSET);
    box[3].setPosition((RECT_SIZE * 3) + OFFSET, RECT_SIZE * 3);
    box[3].setRotation(90);

    for (int i = 0; i < 4; i++) {
        box[i].setSize(Vector2f(RECT_SIZE * 10, 5));
        box[i].setFillColor(Color::Blue);
    }
}

void createGrid(RectangleShape cell[GRID_STEP][GRID_STEP], Color liveCellColor) {
    for(int i = 0; i < GRID_STEP; i++) {
        for(int j = 0; j < GRID_STEP; j++) {
            cell[i][j].setSize(Vector2f(RECT_SIZE, RECT_SIZE));
            cell[i][j].setFillColor(liveCellColor);
            cell[i][j].setOutlineThickness(1);
            cell[i][j].setOutlineColor(Color::Black);
            cell[i][j].setPosition((i * RECT_SIZE) + 1, (j * RECT_SIZE));
        }
    }
}

void drawBoxes(RenderWindow *window, RectangleShape leftBox[], RectangleShape rightBox[]) {
    for (int i = 0; i < 4; i++) {
        window->draw(leftBox[i]);
        window->draw(rightBox[i]);
    }
}

void drawGrid(RenderWindow *window, RectangleShape cell[GRID_STEP][GRID_STEP]) {
    for(int i = 0; i < GRID_STEP; i++) 
        for(int j = 0; j < GRID_STEP; j++) 
            window->draw(cell[i][j]);
}

int main() {
    LOG_CONFIG_TIMESTAMP(false)

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "SFML works!");
    
    Color liveCellColor = Color::White;
    Color deathCellColor(192, 192, 192);    // grey color
    RectangleShape leftBox[4];
    RectangleShape rightBox[4];
    RectangleShape cell[GRID_STEP][GRID_STEP];

// Create 
    createGrid(cell, liveCellColor);
    createBox(leftBox);
    createBox(rightBox);

    for (int i = 0; i < 4; i++)
        rightBox[i].move(Vector2f(RECT_SIZE * 14, 0));

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if(event.type == Event::MouseButtonReleased) {
                if(event.key.code == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);

                    LOG(INFO, "mousePos: " + to_string(mousePos.x) + " " + to_string(mousePos.y))

                    for(int i = 0; i < GRID_STEP; i++) 
                        for(int j = 0; j < GRID_STEP; j++) 
                            if(cell[i][j].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                cell[i][j].setFillColor(deathCellColor);
                                LOG(INFO, "cell: " + to_string(i) + " " + to_string(j))
                            }
                }
            }
        }

        window.clear();

// Draw
        drawGrid(&window, cell);
        drawBoxes(&window, leftBox, rightBox);

        window.display();
    }

    return 0;
}