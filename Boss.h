#include "Enemy.h"

 class Boss : public Enemy
{
    protected:
        //vector<Minion>& minions;
        
    public:
        Boss(/*vector<Minion>& m;*/ float x = 0, float y = 0, float vx = 0, float vy = 0, int wh = 0, int hh = 0, ALLEGRO_BITMAP* p = nullptr):
            Enemy(x, y, vx, vy, wh, hh, p)/*, minions(m)*/ {}

    //void spawn();
};