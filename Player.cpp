#include"Player.h"

 void Player::move(KEYS k)
{
     if((position != k && is_safe) || !is_safe)
    {
         if(k == LEFT || k == RIGHT)
        {
            if(k == LEFT && data.v[0] > 0 || k == RIGHT && data.v[0] < 0)   
                data.v[0] = -data.v[0];

            data.c[0] += data.v[0];
        }
         else if(k == UP || k == DOWN)
        {
            if(k == UP && data.v[1] > 0 || k == DOWN && data.v[1] < 0)
                data.v[1] = -data.v[1];

            data.c[1] += data.v[1];
        }

        if(is_safe && k%2 == position%2)
            is_safe = false;
    }
}