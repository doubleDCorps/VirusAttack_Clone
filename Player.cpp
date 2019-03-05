#include"Player.h"

 bool Player::move(KEYS k)
{
     if(position != k)
    {
        if(k == UP)         data.c[1] -= data.v[1];
        else if(k == DOWN)  data.c[1] += data.v[1];
        else if(k == LEFT)  data.c[0] -= data.v[0];
        else if(k == RIGHT) data.c[0] += data.v[0];

        if(k%2 == position%2)
            is_safe = false;
    }

    return is_safe;
}