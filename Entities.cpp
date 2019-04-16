#include"Entities.h"

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

 void Player::setKey(int key, ALLEGRO_EVENT_TYPE type)
{ 
     if(keymap.find(key)!=keymap.end())
    {
        int temp = key==ALLEGRO_KEY_SPACE ? keys[1] : keys[0];
        
        if(type==ALLEGRO_EVENT_KEY_DOWN and keymap.find(key)->second!=temp)     temp = keymap.find(key)->second;
        else if(type==ALLEGRO_EVENT_KEY_UP and keymap.find(key)->second==temp)  temp = still;

        if(key==ALLEGRO_KEY_SPACE or (!safe or (!temp or (directions[temp-1]==2 or (keys[1] and directions[temp-1]==1)))))
            keys[index.find(key)->second] = static_cast<KEYS>(temp);
    }
}

 void Player::update(const GameList& GL)
{
     switch(keys[0])
    {
        case LEFT:  if(data.vx() > 0) data.vx( -data.vx() ); break;
        case RIGHT: if(data.vx() < 0) data.vx( -data.vx() ); break;
        case UP:    if(data.vy() > 0) data.vy( -data.vy() ); break;
        case DOWN:  if(data.vy() < 0) data.vy( -data.vy() ); break;
        default:    break;
    }

    if(keys[0]!=still)
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

    safe = GL.onEdge(data.center()).first ? true : false;
}