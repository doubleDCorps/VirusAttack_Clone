#ifndef ENTITIES_H_
#define ENTITIES_H_

#include "Entity.h"

 class Enemy: public Entity
{
    public:
        Enemy(float x=0, float y=0, float vx=0, float vy=0, ALLEGRO_BITMAP* p=nullptr)
            : Entity(x, y, vx, vy, p, 1) {}

        virtual void update(int argc, bool argf);
};

 class Player: public Entity
{
    private:
        bool safe = true;
        bool directions[4] = {false, true, true, true};

    public:
        Player(float x=0, float y=0, float v=0, ALLEGRO_BITMAP* p=nullptr)
            : Entity(x, y, v, v, p, 4) {}
        
        virtual void update(int argc, bool argf);

        bool isSafe() const { return safe; }
        void setSafe(bool k){ safe = k; }
};

#endif