#include "../include/Game.hpp"
#include "../include/MyLogger.hpp"

#define WIDTH 1216
#define HEIGHT 512

#define RECT_SIZE 32
#define OFFSET 3
#define GRID_STEP WIDTH / RECT_SIZE

inline void splashscreen(RenderWindow* window, Event* event, Game* game, Menu* menu, Color color, Text* text);

void loadFont(Font *font, string path) {
    if (!font->loadFromFile(path)) {
        LOG(ERROR, "Can't load a font!")
        exit(1);
    }
}

int main() {
    LOG_CONFIG_TIMESTAMP(false)

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Sea Battle");
    srand(time(0));
    
    bool isCreated = false;
    Font font;
    Texture hited_cell;
    Texture shipsTexture[4];

    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(72);
    gameOverText.setPosition(WIDTH / 2, HEIGHT / 2);
    gameOverText.setOrigin(gameOverText.getLocalBounds().width / 2, gameOverText.getGlobalBounds().height / 2);

/* Load resources */
    loadFont(&font, "./media/fonts/Leto Text Sans Defect.otf");
    
    for (int i = 0; i < 4; i++)
        shipsTexture[i] = *loadTexture("./media/img/ship_" + to_string(i + 1) + ".jpg");

    hited_cell = *loadTexture("./media/img/hited_cell.jpg");

    Game game(&window, &hited_cell);

/* Create menu */
    Menu menu(&window, &font);
    
    // add buttons and text
    menu.addMenu();

    while (window.isOpen()) {
        Event event;
        window.clear();

        switch (game.getState()) {
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
                            if(event.mouseButton.button == Mouse::Left) {
                                if(menu.checkToClick(menu.getRectButton(PLAY_BUTTON))) {
                                    LOG(INFO, "Green button was clicked!")
                                    game.setState(PLAY);
                                }

                                if(menu.checkToClick(menu.getRectButton(SETTINGS_BUTTON))) {
                                    LOG(INFO, "Yellow button was clicked!")
                                    menu.setState(SETTINGS);
                                }

                                if(menu.checkToClick(menu.getRectButton(EXIT_BUTTON))) {
                                    LOG(INFO, "Red button was clicked!")
                                    exit(0);
                                }
                            }
                        }
                    }
                }

                if(menu.getState() == PAUSE) {
                    // menu.setMenuVisible(true);

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
                                game.setState(PLAY);
                            }
                        }

                        if(event.type == Event::MouseButtonReleased) {
                            if(event.mouseButton.button == Mouse::Left) {
                                Vector2i mousePos = Mouse::getPosition(window);

                                if(menu.checkToClick(menu.getRectButton(PLAY_BUTTON))) {
                                    LOG(INFO, "Green button was clicked!")
                                    game.setState(PLAY);
                                }

                                if(menu.checkToClick(menu.getRectButton(EXIT_BUTTON))) {
                                    LOG(INFO, "Red button was clicked!")
                                    game.setState(MENU);
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

                menu.draw();
                break;
            
            case PLAY:
                if(!isCreated) {
                    game.createMap(&font);
                    game.createShips(game.getAllShips(), shipsTexture);
                    game.createShips(game.getAllComputerShips(), shipsTexture);
                    game.computersPlacement(game.getAllComputerShips(), true);
                    isCreated = true;
                }

                game.update(&event, &menu);

                // Draw all
                game.drawMap();
                game.drawShips();
                game.drawComputerShips();
                game.drawOther();
                break;
            
            case WIN:
                gameOverText.setString("You Win!");
                gameOverText.setFillColor(Color::White);

                splashscreen(&window, &event, &game, &menu, Color::Green, &gameOverText);
                break;

            case LOSE:
                gameOverText.setString("You Lose.");
                gameOverText.setFillColor(Color::Red);

                splashscreen(&window, &event, &game, &menu, Color::Black, &gameOverText);
                break;
        }

        window.display();
    }

    return 0;
}

inline void splashscreen(RenderWindow* window, Event* event, Game* game, Menu* menu, Color color, Text* text) {
    text->setOrigin(text->getGlobalBounds().width / 2, text->getGlobalBounds().height / 2);
    window->clear(color);

    while (window->pollEvent(*event)) {
        if (event->type == Event::Closed)
            window->close();

        if(event->type == Event::KeyReleased) {
            if(event->key.code == Keyboard::Escape ||
                event->key.code == Keyboard::Space ||
                event->key.code == Keyboard::Enter) {
                game->setState(MENU);
                menu->setState(MAIN_MENU);
            }
        }
    }

    window->draw(*text);
}