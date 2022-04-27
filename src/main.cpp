// #include <SFML/Graphics.hpp>
// using namespace sf;

// #include "Menu/Menu.hpp"
#include "Game.hpp"
#include "MyLogger.hpp"

#define WIDTH 1216
#define HEIGHT 512

#define RECT_SIZE 32
#define OFFSET 3
#define GRID_STEP WIDTH / RECT_SIZE

enum ButtonsText {PLAY_BUTTON, SETTINGS_BUTTON, EXIT_BUTTON};

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

void setBorderBoxText(char symbol, Text *text, Font *font, RectangleShape rect) {
    text->setFont(*font); 
    text->setString(symbol);
    text->setCharacterSize(24); 
    text->setFillColor(sf::Color::Red);
    text->setStyle(sf::Text::Bold);
    text->setPosition(rect.getPosition().x + rect.getSize().x / 4, rect.getPosition().y);
}

void addBorderBoxText(Text numberColumn[], Text letterLine[], Font *font, RectangleShape cell[GRID_STEP][GRID_STEP]) {
    string letters = "ABCDEFGHIJ";
    
    for (int i = 0; i < 10; i++) {
        // lines of the letters
        setBorderBoxText(letters[i], &letterLine[i], font, cell[3 + i][2]);     // Left 
        setBorderBoxText(letters[i], &letterLine[10 + i], font, cell[17 + i][2]);   // Right 
        
        // Columns of the numbers
        setBorderBoxText(*to_string(i + 1).c_str(), &numberColumn[i], font, cell[2][3 + i]);    // Left
        setBorderBoxText(*to_string(i + 1).c_str(), &numberColumn[10 + i], font, cell[16][3 + i]); // Right
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
    Font font;
    Texture shipsTexture[4];
    State currentState = MENU;

    Game game;

/* Load resources */
    loadFont(&font, "./media/fonts/Leto Text Sans Defect.otf");
    
    for (int i = 0; i < 4; i++)
        shipsTexture[i] = loadTexture("./media/img/ship_" + to_string(i + 1) + ".jpg");

/* Create menu */
    Menu menu(&font);

    // add buttons
    menu.addRectButton(Color::Green, FloatRect(WIDTH / 2, HEIGHT / 2.35, WIDTH / 6, HEIGHT / 8));
    menu.addRectButton(Color::Yellow, FloatRect(WIDTH / 2, HEIGHT / 1.7, WIDTH / 6, HEIGHT / 8));
    menu.addRectButton(Color::Red, FloatRect(WIDTH / 2, HEIGHT / 1.35, WIDTH / 6, HEIGHT / 8));

    menu.addText(menu.getRectButton(PLAY_BUTTON).getPosition(), "PLAY", 40, Color::Black, Text::Bold);
    menu.addText(menu.getRectButton(SETTINGS_BUTTON).getPosition(), "SETTINGS", 40, Color::Black, Text::Bold);
    menu.addText(menu.getRectButton(EXIT_BUTTON).getPosition(), "EXIT", 40, Color::Black, Text::Bold);

    // menu.addSpriteButton(&shipsTexture[0], Vector2f(100, 100));

    while (window.isOpen()) {
        Event event;
        window.clear();

        switch (currentState) {
            case MENU:
                if(menu.getState() == MAIN_MENU) {
                    // if(menu.RectButton().isVisible(SETTINGS_BUTTON) == false) {
                        // LOG(INFO, "Visible is false")
                        // menu.RectButton().setVisible(SETTINGS_BUTTON, true);
                    //     menu.getText(SETTINGS_BUTTON)->setScale(1, 1);        // hide the text
                    //     menu.getText(EXIT)->setString("EXIT");
                    // }

                    while (window.pollEvent(event)) {
                        if (event.type == Event::Closed)
                            window.close();

                        if(event.type == Event::MouseButtonReleased) {
                            if(event.key.code == Mouse::Left) {
                                if(menu.checkToClick(&window, menu.getRectButton(PLAY_BUTTON))) {
                                    LOG(INFO, "Green button was clicked!")
                                    currentState = PLAY;
                                }

                                if(menu.checkToClick(&window, menu.getRectButton(SETTINGS_BUTTON))) {
                                    LOG(INFO, "Yellow button was clicked!")
                                    menu.setState(SETTINGS);
                                }

                                if(menu.checkToClick(&window, menu.getRectButton(EXIT_BUTTON))) {
                                    LOG(INFO, "Red button was clicked!")
                                    exit(0);
                                }
                            }
                        }
                    }
                }

                if(menu.getState() == PAUSE) {
                    // if(menu.RectButton().isVisible(SETTINGS_BUTTON) == true) {
                        // LOG(INFO, "Visible is true")
                        // menu.RectButton().setVisible(SETTINGS_BUTTON, false);
                    //     menu.getText(SETTINGS_BUTTON)->setScale(0, 0);         // hide the text
                    //     menu.getText(EXIT)->setString("MENU");
                    // }

                    while (window.pollEvent(event)) {
                        if (event.type == Event::Closed)
                            window.close();

                        if(event.type == Event::KeyReleased) {
                            if(event.key.code == Keyboard::Escape) {
                                currentState = PLAY;
                            }
                        }

                        if(event.type == Event::MouseButtonReleased) {
                            if(event.key.code == Mouse::Left) {
                                Vector2i mousePos = Mouse::getPosition(window);

                                if(menu.checkToClick(&window, menu.getRectButton(PLAY_BUTTON))) {
                                    LOG(INFO, "Green button was clicked!")
                                    currentState = PLAY;
                                }

                                if(menu.checkToClick(&window, menu.getRectButton(EXIT_BUTTON))) {
                                    LOG(INFO, "Red button was clicked!")
                                    currentState = MENU;
                                    menu.setState(MAIN_MENU);
                                }
                            }
                        }
                    }
                }

                if(menu.getState() == SETTINGS) {
                    while (window.pollEvent(event)) {
                        if (event.type == Event::Closed)
                            window.close();

                        if(event.type == Event::KeyReleased) 
                            if(event.key.code == Keyboard::Escape) 
                                menu.setState(MAIN_MENU);
                    }
                }

                menu.draw(&window);
                break;
            
            case PLAY:
                if(!isCreated) {
                    game.createMap(&font);
                    game.createShips(shipsTexture);
                    isCreated = true;
                }

                game.update(&window, &event, &menu, &currentState);
                

                // Draw all
                game.drawMap(&window);
                game.drawShips(&window);
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