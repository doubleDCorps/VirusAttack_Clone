#ifndef ENTITIES_H_
#define ENTITIES_H_

#include "Entity.h"

 class Enemy: public Entity
{
    public:
        Enemy(float x=0, float y=0, float vx=0, float vy=0, ALLEGRO_BITMAP* p=nullptr)
            : Entity(x, y, vx, vy, p, 1) {}

        virtual void update(int argc, bool argf);
        /* 
         void Enemy::update(AXIS v)
        {
            if(v == X) data.v[0] = -data.v[0];
            if(v == Y) data.v[1] = -data.v[1];

            data.c[0] += data.v[0];
            data.c[1] += data.v[1];
        }
        */
};

 class Player: public Entity
{
    private:
        bool safe = true;
        bool directions[4] = {true, true, true, true};

    public:
        Player(float x=0, float y=0, float v=0, ALLEGRO_BITMAP* p=nullptr)
            : Entity(x, y, v, v, p, 4) {}
        
        virtual void update(int argc, bool argf);

        bool isSafe() const { return safe; }
        void setSafe(bool k){ safe = k; }
        /*void move(KEYS k)
        {
             if((position != k && is_safe) || !is_safe)
            {
                 if(k == LEFT || k == RIGHT)
                {
                    if(k == LEFT && data.v[0] > 0 || k == RIGHT && data.v[0] < 0)   
                        data.v[0] = -data.v[0];

                    data.c[0] += data.v[0];
                }
                 else if(k == UP || k == DOWN)
                {
                    if(k == UP && data.v[1] > 0 || k == DOWN && data.v[1] < 0)
                        data.v[1] = -data.v[1];

                    data.c[1] += data.v[1];
                }

                if(is_safe && k%2 == position%2 && k != position)
                    is_safe = false;
            }
        }*/
};

#endif