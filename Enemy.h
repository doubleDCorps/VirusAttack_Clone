#define ENEMY_H_
#ifndef ENEMY_H_

#include "Player.h"

class Enemy : public Entity
{
    private:
    protected:
    public:
        Enemy(int x = 0, int y = 0, int vx = 0, int vy = 0, int wh = 0, int hh = 0, ALLEGRO_BITMAP* p = nullptr):
            cord_x(x), cord_y(y), velocity_x(vx), velocity_y(vy), width_hitbox(wh), height_hitbox(hh), image(p) {}

        virtual ~Enemy()=0;

        virtual void update (AXIS v)=0;


};
#endif