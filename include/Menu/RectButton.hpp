#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Menu/Object.hpp"

using namespace sf;

class RectButton : public Object {
    private:
        vector<RectangleShape> buttons;         // all rectangle shape buttons
        vector<RectangleShape> visibleButtons;    // visible rectangle shapes only 
        vector<RectangleShape>::iterator it;

    public:
        void addObject(Color color, FloatRect bounds);
        RectangleShape getObject(int index);
        void setVisible(int index, bool isVisible);
        bool isVisible(int index);
        void draw(RenderWindow* window);
};
