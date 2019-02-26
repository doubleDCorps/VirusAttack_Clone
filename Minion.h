#include "Boss.h"

class Minion : public Enemy
{
    protected:
        bool alive;
    public:
        Boss(int x = 0, int y = 0, int vx = 0, int vy = 0, int wh = 0, int hh = 0, ALLEGRO_BITMAP* p = nullptr, bool al = true):
            cord_x(x), cord_y(y), velocity_x(vx), velocity_y(vy), width_hitbox(wh), height_hitbox(hh), image(p), alive(al) {}

        bool getAlive() {return alive}
        void setAlive(bool al) {alive=av;}

        //distruttore(?)
};