#include "Boss.h"

 class Minion : public Enemy
{
    protected:
        bool alive;

    public:
        Minion(float x=0, float y=0, float vx=0, float vy=0, int wh=0, int hh=0, ALLEGRO_BITMAP* p=nullptr, bool al=true):
            Enemy(x, y, vx, vy, wh, hh, p), alive(al) {}

        bool getAlive() const  { return alive; }
        void setAlive(bool al) { alive=av; }
};