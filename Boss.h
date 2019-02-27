#include "Enemy.h"

 class Boss : public Enemy
{
    protected:
        vector<Minion>& minions;
        
    public:
        Boss(vector<Minion>& m; int x = 0, int y = 0, int vx = 0, int vy = 0, int wh = 0, int hh = 0, ALLEGRO_BITMAP* p = nullptr):
            Enemy(x, y, vx, vy, wh, hh, p), minions(m) {}

    //void spawn();
};