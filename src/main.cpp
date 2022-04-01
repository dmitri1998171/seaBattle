#include <SFML/Graphics.hpp>
#include <MyLogger.hpp>

#define WIDTH 1216
#define HEIGHT 512

#define RECT_SIZE 32
#define OFFSET 3

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

int main() {
    LOG_CONFIG_TIMESTAMP(false)

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "SFML works!");
    
    int gridStep = WIDTH / RECT_SIZE;
    RectangleShape cell[gridStep][gridStep];
    Color liveCellColor = Color::White;
    Color deathCellColor(192, 192, 192);    // grey color

    RectangleShape leftBox[4];
    RectangleShape rightBox[4];
    
    createBox(leftBox);
    createBox(rightBox);

    for (int i = 0; i < 4; i++)
        rightBox[i].move(Vector2f(RECT_SIZE * 14, 0));

// create a grid
    for(int i = 0; i < gridStep; i++) {
        for(int j = 0; j < gridStep; j++) {
            cell[i][j].setSize(Vector2f(RECT_SIZE, RECT_SIZE));
            cell[i][j].setFillColor(liveCellColor);
            cell[i][j].setOutlineThickness(1);
            cell[i][j].setOutlineColor(Color::Black);
            cell[i][j].setPosition((i * RECT_SIZE) + 1, (j * RECT_SIZE));
        }
    }

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();

// Draw a grid
        for(int i = 0; i < gridStep; i++) 
            for(int j = 0; j < gridStep; j++) 
                window.draw(cell[i][j]);

// Draw boxes
        for (int i = 0; i < 4; i++) {
            window.draw(leftBox[i]);
            window.draw(rightBox[i]);
        }
        
// Mouse click
        if(Mouse::isButtonPressed(Mouse::Left)) {
            Vector2i mousePos = Mouse::getPosition(window);

            // LOG(INFO, to_string(mousePos.x) + " " + to_string(mousePos.y))

            for(int i = 0; i < gridStep; i++) 
                for(int j = 0; j < gridStep; j++) 
                    if(cell[i][j].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        cell[i][j].setFillColor(deathCellColor);
                    }
        }

        window.display();
    }

    return 0;
}