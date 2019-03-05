#include"Player.h"

 bool Player::move(KEYS k)
{
     if(position != k)
    {
        if(k == UP)         c[1] -= v[1];
        else if(k == DOWN)  c[1] += v[1];
        else if(k == LEFT)  c[0] -= v[0];
        else if(k == RIGHT) c[0] += v[0];

        if(k%2 == position%2)
            is_safe = false;
    }

    return is_safe;
}