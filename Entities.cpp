#include"Entities.h"

 void Enemy::update(const GameList& GL)
{     
     if(GL.c_inside(data)) //verifica se si trova dentro l'area o sul bordo
    {
        if(GL.collides(data) == X)  data.vx( -data.vx() );
        if(GL.collides(data) == Y)  data.vy( -data.vy() );

        data.x( data.x() + data.vx() );
        data.y( data.y() + data.vy() );
    }
     else setAlive(false);
}

 void Player::update(const GameList& GL)
{
    /*
    if(keys[1] != 0){
     if(keys[0] == LEFT || keys[0] == RIGHT)
    {
        if(keys[0] == LEFT && data.vx() > 0 ||
           keys[0] == RIGHT && data.vx() < 0 )
            data.vx( -data.vx() );
            
        data.x( data.x() + data.vx() );
    }
     else if(keys[0] == UP || keys[0] == DOWN)
    {
        if(keys[0] == UP && data.vy() > 0 || 
           keys[0] == DOWN && data.vy() < 0 )
            data.vy( -data.vy() );
            
        data.y( data.y() + data.vy() );
    }}
    */
    
     switch(keys[0])
    {
        case LEFT:  if(data.vx() > 0) data.vx( -data.vx() ); break;
        case RIGHT: if(data.vx() < 0) data.vx( -data.vx() ); break;
        case UP:    if(data.vy() > 0) data.vy( -data.vy() ); break;
        case DOWN:  if(data.vy() < 0) data.vy( -data.vy() ); break;
        default:    break;
    }

     for(unsigned i = UP; i < SPACE; ++i)
    {
        if(!GL.c_inside(data.projection(i).center()))     directions[i-1] = 0; 
        else if(GL.o_inside(data.projection(i).center())) directions[i-1] = 1;
        else                                              directions[i-1] = 2;
    }
    
     if(keys[0]!=still and directions[keys[0]-1]==1 and keys[1]!=still)
    {
        if(keys[0]==UP or keys[0]==DOWN)    data.y( data.y() + data.vy() );
        else                                data.x( data.x() + data.vx() );
    
    } else if(keys[0]!=still and directions[keys[0]-1]==2)
    {
        if(keys[0]==UP or keys[0]==DOWN)    data.y( data.y() + data.vy() );
        else                                data.x( data.x() + data.vx() );
    }
    
    return;
}
/*
     if(argc != still)
    {
         if(argf) //se è avvenuta una collisione nella precedente direzione del player
        {
            if(argc == LEFT && data.v[0] > 0 ||
               argc == RIGHT && data.v[0] < 0 )
                data.v[0] = -data.v[0];
            
            data.c[0] += data.v[0];
        }
         else if(argc == UP || argc == DOWN)
        {
            if(argc == UP && data.v[1] > 0 || 
               argc == DOWN && data.v[1] < 0 )
                data.v[1] = -data.v[1];
            
            data.c[1] += data.v[1];
        }

        //if(is_safe && k%2 == position%2 && k != position)
        //    is_safe = false;
    }
*/
