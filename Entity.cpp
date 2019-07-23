#include"Entity.h"

Entity::Entity(float x, float y, unsigned w, unsigned h, float sx, float sy): 
    Hitbox(x, y, w, h), xspeed(sx), yspeed(sy), active(true) {}

void Entity::setActive(bool a) {
    
    active = a;
}

bool Entity::isActive() const {

    return active;
}

void Entity::respawn() {

    setActive(true);
}

float Entity::getXStep() const {

    return xspeed / GameObject::getSize();
}

float Entity::getYStep() const {

    return yspeed / GameObject::getSize();
}

void Entity::bounce(int val) {

    bool check = false;
    for(const auto& element : Root::makeNeighborhood(pos())) {
        if(val == element) {
            check = true;
            if(pos() - val >= -1 and pos() - val <= 1) {
                xspeed = -xspeed;
            } else if(pos() - val >= -Root::getDim() and pos() - val <= Root::getDim()) {
                yspeed = -yspeed;
            }
        }
        if(check)
            return;
    }
}

bool Entity::locksOnObj(const GameObject *const param) const {
    
    return *this == *param;
}

bool Entity::checkForCollision(const Hitbox *const E) const {

    return ((xspeed >= 0 and E->getX() >= getX()) or (xspeed <= 0 and E->getX() <= getX())) 
       and ((yspeed >= 0 and E->getY() >= getY()) or (yspeed <= 0 and E->getY() <= getY()))
       and Hitbox::checkForCollision(E);
}
