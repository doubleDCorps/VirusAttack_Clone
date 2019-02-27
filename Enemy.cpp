#include"Enemy.h"

 void Enemy::update(AXIS v)
{
    if(v == X) velocity_x = -velocity_x;
    if(v == Y) velocity_y = -velocity_y;

    cord_x += velocity_x;
    cord_y += velocity_y;
}