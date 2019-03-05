#ifndef ENEMY_H_
#define ENEMY_H_

#include "Player.h"

 class Enemy : public Entity
{
    private:
    protected:
        ~Enemy() {};

    public:
        using Entity::Entity;
        virtual void update(AXIS v);
};

 class Boss : public Enemy
{
    protected:
        //vector<Minion>& minions;
        
    public:
        Boss(/*vector<Minion>& m;*/ float x = 0, float y = 0, float vx = 0, float vy = 0, int wh = 0, int hh = 0, ALLEGRO_BITMAP* p = nullptr):
            Enemy(x, y, vx, vy, wh, hh, p)/*, minions(m)*/ {}

    //void spawn();
};

 class Minion : public Enemy
{
    protected:
        bool alive;

    public:
        Minion(float x=0, float y=0, float vx=0, float vy=0, int wh=0, int hh=0, ALLEGRO_BITMAP* p=nullptr, bool al=true):
            Enemy(x, y, vx, vy, wh, hh, p), alive(al) {}

        bool getAlive() const  { return alive; }
        void setAlive(bool al) { alive=al; }
};

#endif