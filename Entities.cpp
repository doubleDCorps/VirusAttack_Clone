#include"Entities.h"

 void Enemy::update(const GameList& GL)
{    
    int  argc{ GL.hits(getData())   };
    bool argf{ GL.inside(getData()) };
    
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

 void Player::move()
{
     if(keys[0] == LEFT || keys[0] == RIGHT)
    {
        if(keys[0] == LEFT && data.v[0] > 0 || keys[0] == RIGHT && data.v[0] < 0 )
            data.v[0] = -data.v[0];
            
        data.c[0] += data.v[0];
    }
     else if(keys[0] == UP || keys[0] == DOWN)
    {
        if(keys[0] == UP && data.v[1] > 0 || keys[0] == DOWN && data.v[1] < 0 )
            data.v[1] = -data.v[1];
            
        data.c[1] += data.v[1];
    }
}

 void Player::update(const GameList& GL)
{
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
/*
     if(keys[0] != still)
    {
         if( GL.hits(getData()) ) //se è avvenuta una collisione nella precedente direzione del player
        {
            //KEYS k = peek_in_PilaDiGena();
            //directions[ k ] = false;
            //directions[ k > 2 ? k - 2 : k + 2 ] = true;
        }
         else if(directions[ keys[0]-1 ])
        {
            data.v[ keys[0]%2 ] *= (int)pow(-1, (keys[0]-1)/2 + 1);
            data.c[ keys[0]%2 ] += data.v[ keys[0]%2 ];
        }
    }
*/
     if(keys[1] != still)
    {
         if( GL.hits(getData()) )
        {
            //proiezioni
        }
         else
        {
            move();
        }
    }
     else //se lo spazio non è premuto
    {
        //pop_back_da pila_di_gena();
        //muovi_con_quello();
    }
    //se spazio è premuto
        //verifica collisione
            //se avvenuta, proietta e verifica se la proiezione funge
            //aggiorna dir se necessario
        //muovi
        //pusha
}