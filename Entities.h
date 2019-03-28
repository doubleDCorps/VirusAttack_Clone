#ifndef ENTITIES_H_
#define ENTITIES_H_

#include "Entity.h"

 static const map<int, int> index = {
            {ALLEGRO_KEY_UP, 0},   {ALLEGRO_KEY_DOWN, 0},
            {ALLEGRO_KEY_LEFT, 0}, {ALLEGRO_KEY_RIGHT, 0}, {ALLEGRO_KEY_SPACE, 1}
        };

 static const map<int, int> keymap = {
            {ALLEGRO_KEY_UP, 1},   {ALLEGRO_KEY_DOWN, 3},
            {ALLEGRO_KEY_LEFT, 2}, {ALLEGRO_KEY_RIGHT, 4}, {ALLEGRO_KEY_SPACE, 5}
 };

 class Enemy: public Entity
{
    public:
        Enemy(float x=0, float y=0, float vx=0, float vy=0, ALLEGRO_BITMAP* p=nullptr)
            : Entity(x, y, vx, vy, p, 1) {}

        virtual void update(int, bool) override;
};

class Player: public Entity
{
    private:
        int directions[4] = {1, 1, 1, 1};
        bool safe = true;

    public:
        KEYS keys[2] = {still, still};
        Player(float x=0, float y=0, ALLEGRO_BITMAP* p=nullptr)
            : Entity(x, y, 4, 4, p, 4) {}
        
        virtual void update(int, bool) override;

        bool isSafe() const  { return safe; }
        void setSafe(bool k) { safe = k; }

         void setKey(int key, ALLEGRO_EVENT_TYPE type)
        { 
            int temp;
            if(type == ALLEGRO_EVENT_KEY_DOWN)
                temp = keymap.find(key) != keymap.end() ? keymap.find(key)->second : keys[0];
            else if(type == ALLEGRO_EVENT_KEY_UP)
                temp = keymap.find(key) != keymap.end() ? still : keys[0];
            else 
                return; //ERRORE ERRORE ERRORE

            if(index.find(key) != index.end())
                keys[index.find(key)->second] = static_cast<KEYS>(temp);
        }

         int getKey(int k) const
        {
            if(index.find(k) != index.end()) 
                return keys[index.find(k)->second];

            return -1; //ERRORE ERRORE ERRORE
        }
};

#endif