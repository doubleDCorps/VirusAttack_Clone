#include"Enemy.h"

Enemy::Enemy(float x, float y, unsigned w, unsigned h, float sx, float sy, bool b): 
    Entity(x, y, w, h, sx, sy), boss(b) {}

void Enemy::deathEvent() {

    setActive(false);
}

void Enemy::update() {

    if(!isActive()) return;
    addToX(getXStep());
    addToY(getYStep());

}
        
bool Enemy::isBoss() const {
    
    return boss;
}