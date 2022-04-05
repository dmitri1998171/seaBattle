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

void drawText(RenderWindow *window, Text letterLine[], Text numberColumn[]) {
    for (int i = 0; i < 20; i++) {
        window->draw(letterLine[i]);
        window->draw(numberColumn[i]);
    }
}

Texture loadTexture(string path) {
    Image image;
    Texture texture;

	if(!image.loadFromFile(path)) {
        LOG(ERROR, "Can't load a texture!");
        exit(1);
    }

    texture.loadFromImage(image);
    return texture;
}

void setText(char symbol, Text *text, Font *font, RectangleShape rect) {
    text->setFont(*font); 
    text->setString(symbol);
    text->setCharacterSize(24); 
    text->setFillColor(sf::Color::Red);
    text->setStyle(sf::Text::Bold);
    text->setPosition(rect.getPosition().x + rect.getSize().x / 4, rect.getPosition().y);
}

void addText(Text numberColumn[], Text letterLine[], Font *font, RectangleShape cell[GRID_STEP][GRID_STEP]) {
    string letters = "ABCDEFGHIJ";
    
    for (int i = 0; i < 10; i++) {
        // lines of letters
        setText(letters[i], &letterLine[i], font, cell[3 + i][2]);     // Left 
        setText(letters[i], &letterLine[10 + i], font, cell[17 + i][2]);   // Right 
        
        // Columns of numbers
        setText(*to_string(i + 1).c_str(), &numberColumn[i], font, cell[2][3 + i]);    // Left
        setText(*to_string(i + 1).c_str(), &numberColumn[10 + i], font, cell[16][3 + i]); // Right
    }

    numberColumn[9].setString("10");
    numberColumn[19].setString("10");
}

void loadFont(Font *font, string path) {
    if (!font->loadFromFile(path)) {
        LOG(ERROR, "Can't load a font!")
        exit(1);
    }
}

int main() {
    LOG_CONFIG_TIMESTAMP(false)

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Sea Battle");
    
    Font font;
    Color liveCellColor = Color::White;
    Color deathCellColor(192, 192, 192);    // grey color
    RectangleShape leftBox[4];
    RectangleShape rightBox[4];
    Text letterLine[20];
    Text numberColumn[20];
    RectangleShape cell[GRID_STEP][GRID_STEP];

// Create 
    createGrid(cell, liveCellColor);
    createBox(leftBox);
    createBox(rightBox);

    for (int i = 0; i < 4; i++)
        rightBox[i].move(Vector2f(RECT_SIZE * 14, 0)); // Move right box


    Texture shipsTexture[4];
    for (int i = 0; i < 4; i++)
        shipsTexture[i] = loadTexture("./media/img/ship_" + to_string(i + 1) + ".jpg");

    Sprite shipsSrite[10];
    shipsSrite[0].setTexture(shipsTexture[0]);
    shipsSrite[0].setPosition(cell[30][3].getPosition());

    shipsSrite[1].setTexture(shipsTexture[0]);
    shipsSrite[1].setPosition(cell[32][3].getPosition());

    shipsSrite[2].setTexture(shipsTexture[0]);
    shipsSrite[2].setPosition(cell[34][3].getPosition());

    shipsSrite[3].setTexture(shipsTexture[0]);
    shipsSrite[3].setPosition(cell[36][3].getPosition());

    shipsSrite[4].setTexture(shipsTexture[1]);
    shipsSrite[4].setPosition(cell[30][5].getPosition());

    for (int i = 0; i < 10; i++) {
        shipsSrite[i].setScale(0.6, 0.6);
    }

    loadFont(&font, "./media/fonts/Leto Text Sans Defect.otf");
    addText(numberColumn, letterLine, &font, cell);

    int chooseIndex = -1;

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if(event.type == Event::MouseButtonReleased) {
                if(event.key.code == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);

                    // LOG(INFO, "mousePos: " + to_string(mousePos.x) + " " + to_string(mousePos.y))

                    for(int i = 0; i < GRID_STEP; i++) 
                        for(int j = 0; j < GRID_STEP; j++) 
                            if(cell[i][j].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                cell[i][j].setFillColor(deathCellColor);
                                
                                if(chooseIndex > -1) {
                                    shipsSrite[chooseIndex].setPosition(cell[i][j].getPosition());
                                    chooseIndex = -1;
                                }
                                else {
                                    for (int i = 0; i < 10; i++)
                                        if(shipsSrite[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) 
                                            chooseIndex = i;
                                }
                                // LOG(INFO, "cell: " + to_string(i) + " " + to_string(j))
                            }
                }
            }
        }

        window.clear();

// Draw
        drawGrid(&window, cell);
        drawBoxes(&window, leftBox, rightBox);
        drawText(&window, letterLine, numberColumn);

        for (int i = 0; i < 10; i++)
            window.draw(shipsSrite[i]);

        window.display();
    }

    return 0;
}