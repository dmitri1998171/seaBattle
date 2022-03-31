#include <SFML/Graphics.hpp>

#define WIDTH 640
#define HEIGHT 480

using namespace sf;

int main() {
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "SFML works!");
    
    int rectSize = 16;
    int gridStep = WIDTH / rectSize;

    RectangleShape cell[gridStep][gridStep];
    Color liveCellColor = Color::White;
    Color deathCellColor(192, 192, 192);    // grey color

// create a grid
    for(int i = 0; i < gridStep; i++) {
        for(int j = 0; j < gridStep; j++) {
            cell[i][j].setSize(Vector2f(rectSize, rectSize));
            cell[i][j].setFillColor(liveCellColor);
            cell[i][j].setOutlineThickness(1);
            cell[i][j].setOutlineColor(Color::Black);
            cell[i][j].setPosition((i * rectSize) + 1, (j * rectSize));
        }
    }

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();
        
// draw a grid
        for(int i = 0; i < gridStep; i++) 
            for(int j = 0; j < gridStep; j++) 
                window.draw(cell[i][j]);


        if(Mouse::isButtonPressed(Mouse::Left)) {
            Vector2i mousePos = Mouse::getPosition(window);

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