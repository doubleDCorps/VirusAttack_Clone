#ifndef PLAY_HPP
#define PLAY_HPP
#include"Entity.h"

 class Player : public Entity
{
    private:
        unsigned lifes;
        bool is_safe;
        KEYS position;

    protected:
    public:
        Player(float x=0, float y=0, float vx=0, float vy=0, float wh=0, float hh=0, ALLEGRO_BITMAP* p=nullptr, unsigned l=1, AXIS s=X, KEYS p=none)
            : Entity(x, y, vx, vy, wh, hh, p), lifes(l), is_safe(s), position(p) {}
        
        inline bool move(KEYS k);

        unsigned getLifes() const { return lifes; }
        void loseLife() { lifes==0 ? lifes : --lifes; }
        
        bool getSafe() const { return is_safe; }
        void setSafe(bool s) { is_safe = s; }

        KEYS getPos() const { return position; }
        void setPos(KEYS k) { position = k; }
};

#endif