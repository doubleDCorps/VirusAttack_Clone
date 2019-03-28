#include"Entities.h"

 void Enemy::update(int argc, bool argf)
{     
     if(argf)
    {
        if(argc == X)
            data.v[0] = -data.v[0];
        
        data.c[0] += data.v[0];
        
        if(argc == Y)
            data.v[1] = -data.v[1];

        data.c[1] += data.v[1];
    }
     else
        setAlive(false);
}


 void Player::update(int argc, bool argf)
{
    //TO DO
    if(keys[1] != 0){
     if(keys[0] == LEFT || keys[0] == RIGHT)
    {
        if(keys[0] == LEFT && data.v[0] > 0 ||
           keys[0] == RIGHT && data.v[0] < 0 )
            data.v[0] = -data.v[0];
            
        data.c[0] += data.v[0];
    }
     else if(keys[0] == UP || keys[0] == DOWN)
    {
        if(keys[0] == UP && data.v[1] > 0 || 
           keys[0] == DOWN && data.v[1] < 0 )
            data.v[1] = -data.v[1];
            
        data.c[1] += data.v[1];
    
    }}

    return;

     if(argc != still)
    {
         if(argf) //se è avvenuta una collisione nella precedente direzione del player
        {
            //KEYS k = peek_in_PilaDiGena();
            //directions[ k ] = false;
            //directions[ k > 2 ? k - 2 : k + 2 ] = true;
        }
         else if(directions[argc])
        {
            data.v[ argc%2 ] *= pow(-1, (argc-1)/2 + 1);
            data.c[ argc%2 ] += data.v[ argc%2 ];
        }
    }
}