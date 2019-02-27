#include"Player.h"

 bool Player::move(KEYS k)
{
     if(position != k)
    {
        if(k == UP)         coord_y -= velocity_y;
        else if(k == DOWN)  coord_y += velocity_y;
        else if(k == LEFT)  coord_x -= velocity_x;
        else if(k == RIGHT) coord_x += velocity_x;

        if(k%2 == position%2)
            is_safe = false;
    }

    return is_safe;
}