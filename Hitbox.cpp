#include"Hitbox.h"

Hitbox::Hitbox(float x, float y, unsigned w, unsigned h): GameObject(x, y), width(w), height(h) {}

void Hitbox::draw() {

    image.draw(getX(), getY());
}

float Hitbox::getW() const {

    return width;
}

float Hitbox::getH() const {

    return height;
}

float Hitbox::getXW() const {
    
    return getX() + width;
}

float Hitbox::getYH() const {
    
    return getY() + height;
}

bool Hitbox::checkForCollision(const Hitbox *const H) const {
    //verifica sull'asse delle x
    return (checkRange(getX(), H->getX(), H->getXW()) or checkRange(getXW(), H->getX(), H->getXW()) or
            checkRange(H->getX(), getX(), getXW()) or checkRange(H->getXW(), getX(), getXW()))
    //verifica sull'asse delle y
       and (checkRange(getY(), H->getY(), H->getYH()) or checkRange(getYH(), H->getY(), H->getYH()) or 
            checkRange(H->getY(), getY(), getYH()) or checkRange(H->getYH(), getY(), getYH()));
}