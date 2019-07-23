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

    bool enablePrint = false;

    if(pos() - val >= -1 or pos() - val <= 1) {
        xspeed = -xspeed;
    
        if(enablePrint)
            cout << "Entity::bounce() " << this << " x changed\n";
    }
    if(pos() - val >= -Root::getDim() or pos() - val <= Root::getDim()) {
        yspeed = -yspeed;

        if(enablePrint)
            cout << "Entity::bounce() " << this << " y changed\n";
    }
}

bool Entity::locksOnObj(const Hitbox *const param) const {
    
    return abs(getX() - param->getX()) <= GameObject::getSize() 
       and abs(getY() - param->getY()) <= GameObject::getSize();

}

bool Entity::checkForCollision(const Hitbox *const E) const {

    bool enablePrint = false;

    bool ret = 
    ((xspeed >= 0 and E->getX() >= getX()) or (xspeed <= 0 and E->getX() <= getX())) 
    and ((yspeed >= 0 and E->getY() >= getY()) or (yspeed <= 0 and E->getY() <= getY()))
    and Hitbox::checkForCollision(E);

    if(enablePrint)
        cout << "\nEntity::checkForCollision(): " << this << " <-> " << E << " :: " << ret << endl;

    return ret;
}
