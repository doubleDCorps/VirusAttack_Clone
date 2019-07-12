#ifndef ENEMY_H
#define ENEMY_H

#include"Entity.h"

 class Enemy: public Entity
{
    public:
        Enemy(float x=0, float y=0, float vx=0, float vy=0, ALLEGRO_BITMAP* p=nullptr): Entity(x, y, vx, vy, p, 1) {}

        virtual void update(const GameList&) override;
};

#endif