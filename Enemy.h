#define ENEMY_H_
#ifndef ENEMY_H_

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
#endif