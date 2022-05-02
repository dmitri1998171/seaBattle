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

void loadFont(Font *font, string path) {
    if (!font->loadFromFile(path)) {
        LOG(ERROR, "Can't load a font!")
        exit(1);
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
        shipsTexture[i] = *loadTexture("./media/img/ship_" + to_string(i + 1) + ".jpg");

/* Create menu */
    Menu menu(&font);
    
    // add buttons
    menu.addRectButton(Color::Green, FloatRect(WIDTH / 2, HEIGHT / 2.35, WIDTH / 6, HEIGHT / 8));
    menu.addRectButton(Color::Yellow, FloatRect(WIDTH / 2, HEIGHT / 1.7, WIDTH / 6, HEIGHT / 8));
    menu.addRectButton(Color::Red, FloatRect(WIDTH / 2, HEIGHT / 1.35, WIDTH / 6, HEIGHT / 8));

    menu.addText(menu.getRectButton(PLAY_BUTTON).getPosition(), "PLAY", 40, Color::Black, Text::Bold);
    menu.addText(menu.getRectButton(SETTINGS_BUTTON).getPosition(), "SETTINGS", 40, Color::Black, Text::Bold);
    menu.addText(menu.getRectButton(EXIT_BUTTON).getPosition(), "EXIT", 40, Color::Black, Text::Bold);

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
                game.drawOther(&window);
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