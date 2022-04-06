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

void drawShipsSprite(RenderWindow *window, Sprite shipsSrite[]) {
    for (int i = 0; i < 10; i++)
        window->draw(shipsSrite[i]);
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

void setShipsSprite(Sprite shipsSrite[], Texture shipsTexture[], RectangleShape cell[GRID_STEP][GRID_STEP]) {
    for (int i = 0; i < 10; i++) {
        if (i < 4) {
            shipsSrite[i].setTexture(shipsTexture[0]);
            shipsSrite[i].setPosition(cell[30 + i * 2][3].getPosition());
        }

        else if (i < 7) {
            shipsSrite[i].setTexture(shipsTexture[1]);
            shipsSrite[i].setPosition(cell[17 + i * 3][5].getPosition());
        }

        else if (i < 9) {
            shipsSrite[i].setTexture(shipsTexture[2]);
            shipsSrite[i].setPosition(cell[2 + i * 4][7].getPosition());
        }

        else
        {
            shipsSrite[i].setTexture(shipsTexture[3]);
            shipsSrite[i].setPosition(cell[33][9].getPosition());
        }
        
        shipsSrite[i].setScale(0.6, 0.6);
    }
}

int main() {
    LOG_CONFIG_TIMESTAMP(false)

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Sea Battle");
    
    int chooseIndex = -1;   // Need for ship placement
    Font font;
    Color liveCellColor = Color::White;
    Color deathCellColor(192, 192, 192);    // grey color
    RectangleShape leftBox[4];
    RectangleShape rightBox[4];
    Texture shipsTexture[4];
    Sprite shipsSrite[10];
    Text letterLine[20];
    Text numberColumn[20];
    RectangleShape cell[GRID_STEP][GRID_STEP];

// Load resources
    loadFont(&font, "./media/fonts/Leto Text Sans Defect.otf");
    
    for (int i = 0; i < 4; i++)
        shipsTexture[i] = loadTexture("./media/img/ship_" + to_string(i + 1) + ".jpg");

// Create primitives
    createGrid(cell, liveCellColor);
    createBox(leftBox);
    createBox(rightBox);

    for (int i = 0; i < 4; i++)
        rightBox[i].move(Vector2f(RECT_SIZE * 14, 0)); // Move right box

    setShipsSprite(shipsSrite, shipsTexture, cell);
    addText(numberColumn, letterLine, &font, cell);

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

// Draw all
        drawGrid(&window, cell);
        drawBoxes(&window, leftBox, rightBox);
        drawText(&window, letterLine, numberColumn);
        drawShipsSprite(&window, shipsSrite);

        window.display();
    }

    return 0;
}