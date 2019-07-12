#include"Enemy.h"

 void Enemy::update(const GameList& GL)
{     
     if(GL.c_inside(data))
    {
        if(GL.collides(data) == X)  data.vx( -data.vx() );
        if(GL.collides(data) == Y)  data.vy( -data.vy() );

        data.x( data.x() + data.vx() );
        data.y( data.y() + data.vy() );
    }
     else setAlive(false);
}
