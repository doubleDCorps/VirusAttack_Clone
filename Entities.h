#ifndef ENTITIES_H_
#define ENTITIES_H_

#include "Player.h"

 class Enemy: public Entity
{
    public:
        using Entity::Entity;
        virtual ~Enemy() {};
        virtual void update(int argc, bool argf)
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
        unsigned lifes;
        bool is_safe;
        KEYS position;

    public:
        Player(float x=0, float y=0, float v=0, float wh=0, float hh=0, ALLEGRO_BITMAP* p=nullptr, unsigned l=1, bool s=true, KEYS k=still)
            : Entity(x, y, v, v, wh, hh, p), lifes(l), is_safe(s), position(k) {}
        
        void move(KEYS k)
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
        }

        unsigned getLifes() const { return lifes; }
        void loseLife() { lifes==0 ? lifes : --lifes; }
        
        bool getSafe() const { return is_safe; }
        void setSafe(bool s) { is_safe = s; }

        KEYS getPos() const { return position; }
        void setPos(KEYS k) { position = k; }
};

#endif