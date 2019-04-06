#ifndef ENTITIES_H_
#define ENTITIES_H_

#include "Entity.h"

 class Enemy: public Entity
{
    public:
        Enemy(float x=0, float y=0, float vx=0, float vy=0, ALLEGRO_BITMAP* p=nullptr)
            : Entity(x, y, vx, vy, p, 1) {}

        virtual void update(const GameList&) override;
};

 class Player: public Entity
{
    private:
        inline static const map<int, int> index  {{ALLEGRO_KEY_UP, 0}, {ALLEGRO_KEY_DOWN, 0}, {ALLEGRO_KEY_LEFT, 0}, {ALLEGRO_KEY_RIGHT, 0}, {ALLEGRO_KEY_SPACE, 1}};
        inline static const map<int, int> keymap {{ALLEGRO_KEY_UP, 1}, {ALLEGRO_KEY_DOWN, 3}, {ALLEGRO_KEY_LEFT, 2}, {ALLEGRO_KEY_RIGHT, 4}, {ALLEGRO_KEY_SPACE, 5}};

        int directions[4] = {2, 2, 2, 2};
        KEYS keys[2] = {still, still};
        bool safe = true;
        bool directions[4] = {true, true, true, true};

    public:
        Player(float x=0, float y=0, float v=0, ALLEGRO_BITMAP* p=nullptr)
            : Entity(x, y, v, v, p, 4) {}
        
        Player(float x=0, float y=0, ALLEGRO_BITMAP* p=nullptr)
            : Entity(x, y, 4, 4, p, 4) {}
        
        virtual void update(const GameList&) override;

        bool isSafe() const { return safe; }
        void setSafe(bool k){ safe = k; }
};

#endif