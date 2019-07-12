#ifndef PLAYER_H
#define PLAYER_H

#include "Enemy.h"

 void push_reverse_way(int &reverse_cont, vector<pair<int, int>> &reverse_way, int &pressed_key);

 class Player: public Entity
{
    private:
        inline static const map<int, int> index  {{ALLEGRO_KEY_UP, 0}, {ALLEGRO_KEY_DOWN, 0}, {ALLEGRO_KEY_LEFT, 0}, {ALLEGRO_KEY_RIGHT, 0}, {ALLEGRO_KEY_SPACE, 1}};
        inline static const map<int, int> keymap {{ALLEGRO_KEY_UP, 1}, {ALLEGRO_KEY_DOWN, 3}, {ALLEGRO_KEY_LEFT, 2}, {ALLEGRO_KEY_RIGHT, 4}, {ALLEGRO_KEY_SPACE, 5}};

        int directions[4] = {2, 2, 2, 2};
        KEYS keys[2] = {still, still};
        bool safe = true;

        vector<pair<int, int>> reverse_way;
        int reverse_cont{0};
        int pressed_key{0};
        bool force_reverse_way = {false};
        bool first_reverse_step{true};

        using Entity::setVelocity_x;
        using Entity::setVelocity_y;
    public:
        Player(float x=0, float y=0, float v=0, ALLEGRO_BITMAP* p=nullptr)
            : Entity(x, y, v, v, p, 4) {}
        
        Player(float x=0, float y=0, ALLEGRO_BITMAP* p=nullptr)
            : Entity(x, y, 4, 4, p, 4) {}
        
        virtual void update(const GameList&) override;
        /*
            Restituisce la safety flag (true: sul bordo, false: può morire).
        */
        bool isSafe() const { return safe; }
        /*
            Imposta la safety flag.
        */
        void setSafe(bool k){ safe = k; }
        /*
            Salva l'ultimo tasto premuto in memoria per eseguire l'aggiornamento del movimento;
            nel caso in cui più tasti vengono premuti, si dà priorità esclusivamente all'ultimo.
        */
        void setKey(int, ALLEGRO_EVENT_TYPE);
        /*
            Restituisce l'ultimo tasto premuto.
        */
        int  getKey() const     { return keys[0];        } 
        /*
            Restituisce lo stato attuale dello spazio (premuto/non premuto).
        */
        bool getSpace() const   { return keys[1]!=still; }
        /*
            imposta il centro del player alla posizione indicata.
        */
         void setCenter(const PointData& P)
        { 
            data.x(P.x()-data.dx()/2.0F);
            data.y(P.y()-data.dy()/2.0F);
        }
        /*
            Svuota la pila dell'algoritmo reverse_way.
        */
         void clearReverse()
        { 
            reverse_way.clear();
            pressed_key=still;
            reverse_cont=0;
            force_reverse_way=false;
            first_reverse_step=true;
            keys[1] = still;
            keys[0] = still;
        }
};

#endif