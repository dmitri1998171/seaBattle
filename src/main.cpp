// #include <SFML/Graphics.hpp>
// using namespace sf;

#include "Menu/Menu.hpp"
#include "MyLogger.hpp"

#define RECT_SIZE 32
#define OFFSET 3
#define GRID_STEP WIDTH / RECT_SIZE

enum State {MENU, PLAY, WIN, LOSE};

void createBorderBox(RectangleShape BorderBox[]) {
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

void drawBorderBoxes(RenderWindow *window, RectangleShape leftBorderBox[], RectangleShape rightBorderBox[]) {
    for (int i = 0; i < 4; i++) {
        window->draw(leftBorderBox[i]);
        window->draw(rightBorderBox[i]);
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

void setShipsSprite(Sprite shipsSrite[], Texture shipsTexture[], RectangleShape cell[GRID_STEP][GRID_STEP], int isVertical[]) {
    for (int i = 0; i < 10; i++) {
        if (i < 4) {
            shipsSrite[i].setTexture(shipsTexture[0]);
            shipsSrite[i].setOrigin(shipsSrite[i].getLocalBounds().width / 2, shipsSrite[i].getLocalBounds().height / 2);
            shipsSrite[i].setPosition(cell[30 + i * 2][3].getPosition());
        }

        else if (i < 7) {
            shipsSrite[i].setTexture(shipsTexture[1]);
            shipsSrite[i].setOrigin(shipsSrite[i].getLocalBounds().width / 4, shipsSrite[i].getLocalBounds().height / 2);
            shipsSrite[i].setPosition(cell[17 + i * 3][5].getPosition());
        }

        else if (i < 9) {
            shipsSrite[i].setTexture(shipsTexture[2]);
            shipsSrite[i].setOrigin(shipsSrite[i].getLocalBounds().width / 2, shipsSrite[i].getLocalBounds().height / 2);
            shipsSrite[i].setPosition(cell[3 + i * 4][7].getPosition());
        }

        else {
            shipsSrite[i].setTexture(shipsTexture[3]);
            shipsSrite[i].setOrigin(shipsSrite[i].getLocalBounds().width / 8, shipsSrite[i].getLocalBounds().height / 2);
            shipsSrite[i].setPosition(cell[33][9].getPosition());
        }
        
        shipsSrite[i].setScale(0.6, 0.6);
        shipsSrite[i].move(16, 16);     // Eccentricity compensation (setOrigin)
        isVertical[i] = 0;
    }
}

int main() {
    LOG_CONFIG_TIMESTAMP(false)

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Sea Battle");
    
    bool isCreated = false;
    int chooseIndex = -1;   // Need for ship placement
    int isVertical[10];
    Font font;
    Color liveCellColor = Color::White;
    Color deathCellColor(192, 192, 192);    // grey color
    RectangleShape leftBorderBox[4];
    RectangleShape rightBorderBox[4];
    Texture shipsTexture[4];
    Sprite shipsSrite[10];
    Text letterLine[20];
    Text numberColumn[20];
    RectangleShape cell[GRID_STEP][GRID_STEP];
    State currentState = MENU;

/* Load resources */
    loadFont(&font, "./media/fonts/Leto Text Sans Defect.otf");
    
    for (int i = 0; i < 4; i++)
        shipsTexture[i] = loadTexture("./media/img/ship_" + to_string(i + 1) + ".jpg");

/* Create menu */
    Menu menu;
    RectangleShape playButton, settingsButton, exitButton;

    // add buttons
    menu.createRect(&playButton, Color::Green, FloatRect(WIDTH / 2, HEIGHT / 2.35, WIDTH / 6, HEIGHT / 8));
    menu.createRect(&settingsButton, Color::Yellow, FloatRect(WIDTH / 2, HEIGHT / 1.7, WIDTH / 6, HEIGHT / 8));
    menu.createRect(&exitButton, Color::Red, FloatRect(WIDTH / 2, HEIGHT / 1.35, WIDTH / 6, HEIGHT / 8));

    while (window.isOpen()) {
        Event event;
        window.clear();

        switch (currentState) {
            case MENU:
                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed)
                        window.close();

                    if(menu.getState() == MAIN_MENU) {
                        if(event.type == Event::MouseButtonReleased) {
                            if(event.key.code == Mouse::Left) {
                                Vector2i mousePos = Mouse::getPosition(window);

                                if(menu.checkToClickRect(&window, playButton)) {
                                    LOG(INFO, "Green button was clicked!")
                                    currentState = PLAY;
                                }

                                if(menu.checkToClickRect(&window, settingsButton)) {
                                    LOG(INFO, "Green button was clicked!")

                                }

                                if(menu.checkToClickRect(&window, exitButton)) {
                                    LOG(INFO, "Red button was clicked!")
                                    exit(0);
                                }
                            }
                        }
                    }

                    if(menu.getState() == PAUSE) {
                        if(event.type == Event::KeyReleased) {
                            if(event.key.code == Keyboard::Escape) {
                                currentState = PLAY;
                            }
                        }

                        if(event.type == Event::MouseButtonReleased) {
                            if(event.key.code == Mouse::Left) {
                                Vector2i mousePos = Mouse::getPosition(window);

                                if(menu.checkToClickRect(&window, playButton)) {
                                    LOG(INFO, "Green button was clicked!")
                                    currentState = PLAY;
                                }

                                if(menu.checkToClickRect(&window, settingsButton)) {
                                    LOG(INFO, "Yellow button was clicked!")
                                    menu.setState(SETTINGS);
                                }

                                if(menu.checkToClickRect(&window, exitButton)) {
                                    LOG(INFO, "Red button was clicked!")
                                    currentState = MENU;
                                    menu.setState(MAIN_MENU);
                                }
                            }
                        }
                    }

                    if(menu.getState() == SETTINGS) {
                        
                    }
                }

                menu.draw(&window);
                window.draw(playButton);
                window.draw(settingsButton);
                window.draw(exitButton);
                break;
            
            case PLAY:
                if(!isCreated) {
                    /* Create primitives */
                    createGrid(cell, liveCellColor);
                    createBorderBox(leftBorderBox);
                    createBorderBox(rightBorderBox);

                    for (int i = 0; i < 4; i++)
                        rightBorderBox[i].move(Vector2f(RECT_SIZE * 14, 0)); // Move right BorderBorderBox

                    setShipsSprite(shipsSrite, shipsTexture, cell, isVertical);
                    addText(numberColumn, letterLine, &font, cell);
                    isCreated = true;
                }

                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed)
                        window.close();

                    if(event.type == Event::KeyReleased) {
                        if(event.key.code == Keyboard::R) {
                            if(chooseIndex > -1) { // If ship was chosen
                                shipsSrite[chooseIndex].setRotation(shipsSrite[chooseIndex].getRotation() + 90);

                                isVertical[chooseIndex]++;
                                if(isVertical[chooseIndex] > 1)
                                    isVertical[chooseIndex] = 0;

                                chooseIndex = -1;
                            }
                        }

                        if(event.key.code == Keyboard::Escape) {
                            currentState = MENU;
                            menu.setState(PAUSE);
                        }
                    }

                    if(event.type == Event::MouseButtonReleased) {
                        if(event.key.code == Mouse::Left) {
                            Vector2i mousePos = Mouse::getPosition(window);

                            // LOG(INFO, "mousePos: " + to_string(mousePos.x) + " " + to_string(mousePos.y))

                            for(int i = 0; i < GRID_STEP; i++) 
                                for(int j = 0; j < GRID_STEP; j++) 
                                    if(cell[i][j].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                        cell[i][j].setFillColor(deathCellColor);
                                        
                                        if(chooseIndex > -1) {      // If ship was chosen
                                            if((i > 2 && i < 13) && (j > 2 && j < 14)) {    // If mouse click was inside the left BorderBox
                                                shipsSrite[chooseIndex].setPosition(cell[i][j].getPosition());
                                                shipsSrite[chooseIndex].move(16, 16);   // Eccentricity compensation (setOrigin)

                                                for (int k = 0; k < 4; k++) {    // Checking the collision of the ship with the borders
                                                    if(shipsSrite[chooseIndex].getGlobalBounds().intersects(leftBorderBox[k].getGlobalBounds())) {
                                                        shipsSrite[chooseIndex].setColor(Color::Red);
                                                        LOG(WARNING, "The ship have collision with a border!")
                                                        break;
                                                    }
                                                    else
                                                        shipsSrite[chooseIndex].setColor(Color::White);
                                                }

                                                for (int k = 0; k < 10; k++) {    // Checking the distance of the ship with the another ship
                                                    if(chooseIndex != k)
                                                        if(shipsSrite[chooseIndex].getGlobalBounds().left >= shipsSrite[k].getGlobalBounds().left - 32 && 
                                                        shipsSrite[chooseIndex].getGlobalBounds().left <= shipsSrite[k].getGlobalBounds().left + 32) {
                                                            if(shipsSrite[chooseIndex].getGlobalBounds().top >= shipsSrite[k].getGlobalBounds().top - 32 && 
                                                            shipsSrite[chooseIndex].getGlobalBounds().top <= shipsSrite[k].getGlobalBounds().top + 32)
                                                                shipsSrite[chooseIndex].setColor(Color::Red);
                                                                LOG(WARNING, "The ship have collision with another ship!")
                                                                break;
                                                        }
                                                }
                                                
                                                chooseIndex = -1;
                                            }
                                        }
                                        else {  
                                            for (int i = 0; i < 10; i++)
                                                if(shipsSrite[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) // If the mouse click was on a ship
                                                    chooseIndex = i;
                                        }
                                        // LOG(INFO, "cell: " + to_string(i) + " " + to_string(j))
                                    }
                        }
                    }
                }
        
        // Draw all
                drawGrid(&window, cell);
                drawBorderBoxes(&window, leftBorderBox, rightBorderBox);
                drawText(&window, letterLine, numberColumn);
                drawShipsSprite(&window, shipsSrite);
                break;
            
            case WIN:
                break;

            case LOSE:
                break;
        }

        window.display();
    }

    return 0;
}