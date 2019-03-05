#include"Enemy.h"

 void Enemy::update(AXIS v)
{
    if(v == X) data.v[0] = -data.v[0];
    if(v == Y) data.v[1] = -data.v[1];

    data.c[0] += data.v[0];
    data.c[1] += data.v[1];
}