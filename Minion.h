#include "Boss.h"

 class Minion : public Enemy
{
    protected:
        bool alive;

    public:
        Boss(int x = 0, int y = 0, int vx = 0, int vy = 0, int wh = 0, int hh = 0, ALLEGRO_BITMAP* p = nullptr, bool al = true):
            Enemy(x, y, vx, vy, wh, hh, p), alive(al) {}

        bool getAlive() const  { return alive; }
        void setAlive(bool al) { alive=av; }
};