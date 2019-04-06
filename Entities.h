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

    public:
        
        Player(float x=0, float y=0, ALLEGRO_BITMAP* p=nullptr)
            : Entity(x, y, 4, 4, p, 4) {}
        
        virtual void update(const GameList&) override;

        bool isSafe() const  { return safe; }
        void setSafe(bool k) { safe = k; }
        /*
            Salva l'ultimo tasto premuto in memoria per eseguire l'aggiornamento del movimento;
            nel caso in cui più tasti vengono premuti, si dà priorità esclusivamente all'ultimo.
        */
         void setKey(int key, ALLEGRO_EVENT_TYPE type)
        { 
             if(keymap.find(key) != keymap.end()) //se il tasto è valido
            {
                int actual, temp;
                if(key == ALLEGRO_KEY_SPACE) actual = temp = keys[1];
                else                         actual = temp = keys[0];
                

                 if(type == ALLEGRO_EVENT_KEY_DOWN and keymap.find(key)->second != actual)
                {
                    temp = keymap.find(key)->second;
                }
                 else if(type == ALLEGRO_EVENT_KEY_UP and keymap.find(key)->second == actual)
                {
                    temp = still;
                }

                if(index.find(key) != index.end())
                    keys[index.find(key)->second] = static_cast<KEYS>(temp);
            }

        }

         int getKey(int k) const
        {
            if(index.find(k) != index.end()) 
                return keys[index.find(k)->second];

            return -1; //ERRORE ERRORE ERRORE
        }
};

#endif