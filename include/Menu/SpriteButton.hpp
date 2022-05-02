#include "Menu/Object.hpp"

class SpriteButton : public Object {
    private:
        vector<Sprite> buttons;         // all sprite buttons
        vector<Sprite> visibleButtons;    // visible sprite buttons only 
        vector<Sprite>::iterator it;

    public:
        
        void addObject(string texturePath, Vector2f position);
        Sprite getObject(int index);
        void setVisible(int index, bool isVisible);
        bool isVisible(int index);
        void draw(RenderWindow* window);
        int capacity();
        int size();
};
