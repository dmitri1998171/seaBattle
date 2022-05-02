#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <Menu/UI.hpp> 

class Object : public IDrawUI {
    public:
        void addObject(Color color, FloatRect bounds) {};
        void addObject(string texturePath, Vector2f position) {};
        virtual void setVisible(int index, bool isVisible) = 0;
        virtual bool isVisible(int index) = 0;
        virtual void draw(RenderWindow* window) = 0;
        virtual int capacity() = 0;
        virtual int size() = 0;
};

#endif