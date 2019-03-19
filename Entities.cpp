#include"Entities.h"

 void Enemy::update(int argc, bool argf)
{
    if(argc == X) data.v[0] = -data.v[0];
    if(argc == Y) data.v[1] = -data.v[1];

    data.c[0] += data.v[0];
    data.c[1] += data.v[1];
}

//forse si creano conflitti con l'implementazione delle collisioni?
 void Player::update(int argc, bool argf)
{
    //if((position != k && is_safe) || !is_safe)
    {
         if(argc == LEFT || argc == RIGHT)
        {
            if(argc == LEFT && data.v[0] > 0 || argc == RIGHT && data.v[0] < 0)
                data.v[0] = -data.v[0];

            data.c[0] += data.v[0];
        }
         else if(argc == UP || argc == DOWN)
        {
            if(argc == UP && data.v[1] > 0 || argc == DOWN && data.v[1] < 0)
                data.v[1] = -data.v[1];

            data.c[1] += data.v[1];
        }

        //if(is_safe && k%2 == position%2 && k != position)
        //    is_safe = false;
    }
 
}