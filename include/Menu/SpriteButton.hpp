#include "Menu/Object.hpp"

class SpriteButton : public Object {
    private:
        vector<Sprite> buttons;         // all rectangle shape buttons
        vector<Sprite> visibleButtons;    // visible rectangle shapes only 
        vector<Sprite>::iterator it;

    public:
        
        void addObject(string texturePath, Vector2f position);
        Sprite getObject(int index);
        void setVisible(int index, bool isVisible);
        bool isVisible(int index);
        void draw(RenderWindow* window);
};
