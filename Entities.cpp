#include "Entities.h"

void push_reverse_way(int& reverse_cont, vector<pair<int, int>> &reverse_way, int pressed_key) {
    if(reverse_cont!=0) {
        if(pressed_key==1) //UP
            reverse_way.push_back({reverse_cont, 3});
        else if(pressed_key==2) //LEFT
            reverse_way.push_back({reverse_cont, 4});
        else if(pressed_key==3) //DOWN
            reverse_way.push_back({reverse_cont, 1});
        else if(pressed_key==4) //RIGHT
            reverse_way.push_back({reverse_cont, 2});

        reverse_cont=0;
        }
}

void Enemy::update(const GameList &GL)
{
    if (GL.c_inside(data)) //verifica se si trova dentro l'area o sul bordo
    {
        if (GL.collides(data) == X)
            data.vx(-data.vx());
        if (GL.collides(data) == Y)
            data.vy(-data.vy());

        data.x(data.x() + data.vx());
        data.y(data.y() + data.vy());
    }
    else
        setAlive(false);
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
    bool do_stuff{false};
    KEYS ifDo{still};

     if(keys[1]!=still and !GL.onEdge(data).first)
    {
        do_stuff = false;
         if(keys[0] != still and first_reverse_step)
        {
            first_reverse_step = false;
            pressed_key = keys[0];
        }

        if(keys[0] == pressed_key and !first_reverse_step)
            reverse_cont++;
         else if(keys[0] != pressed_key and !first_reverse_step)
        {
            push_reverse_way(reverse_cont, reverse_way, pressed_key);
            pressed_key = keys[0];
            reverse_cont++;
        }
    }
     else if(GL.onEdge(data.center()).first)
    {
        reverse_way.clear();
    }
     else if(keys[0] == still or directions[keys[0] - 1] != 2)
    {
        push_reverse_way(reverse_cont, reverse_way, pressed_key);
        first_reverse_step = true;

         if(reverse_way.size() != 0)
        { //ALGORITMO CAMMINO INVERSO
             if (reverse_way[reverse_way.size() - 1].first > 0)
            {
                ifDo = static_cast<KEYS>(reverse_way[reverse_way.size() - 1].second);
                reverse_way[reverse_way.size() - 1].first--;
                do_stuff = true;
            }
            if (reverse_way[reverse_way.size() - 1].first == 0)
                reverse_way.pop_back();
        }    
    }
    
     switch (keys[0])
    {
    case LEFT:
        if (data.vx() > 0)
            data.vx(-data.vx());
        break;
    case RIGHT:
        if (data.vx() < 0)
            data.vx(-data.vx());
        break;
    case UP:
        if (data.vy() > 0)
            data.vy(-data.vy());
        break;
    case DOWN:
        if (data.vy() < 0)
            data.vy(-data.vy());
        break;
    default:
        break;
    }

     for (unsigned i = UP; i < SPACE; ++i)
    {
        if (!GL.c_inside(data.projection(i).center()))      directions[i - 1] = 0;
        else if (GL.o_inside(data.projection(i).center()))  directions[i - 1] = 1;
        else                                                directions[i - 1] = 2;
    }

     if(do_stuff == true)
    {
        if (ifDo == UP or ifDo == DOWN)   data.y(data.y() + data.vy());
        else                              data.x(data.x() + data.vx());
    }
     else if(do_stuff == false and keys[0] != still and directions[keys[0] - 1] == 1 and keys[1] != still)
    {
        if (keys[0] == UP or keys[0] == DOWN)   data.y(data.y() + data.vy());
        else                                    data.x(data.x() + data.vx());
    }
    else if(keys[0] != still and directions[keys[0] - 1] == 2)
    {
        if (keys[0] == UP or keys[0] == DOWN)   data.y(data.y() + data.vy());
        else                                    data.x(data.x() + data.vx());
    }

    return;
}
/*
//GENA
     if(keys[1] != still && !(GL.collides(data)))
    {
        if (keys[0]!=still && first_reverse_step)
        {
            
        }

        if (keys[0] == UP && !first_reverse_step && pressed_key == UP)
            reverse_cont++;
        else if (keys[0] == UP && !first_reverse_step && pressed_key != UP)
        {
            push_reverse_way(reverse_cont, reverse_way, pressed_key);
            pressed_key = UP;
            reverse_cont++;
        }

        if (keys[0] == DOWN && !first_reverse_step && pressed_key == DOWN)
            reverse_cont++;
        else if (keys[0] == DOWN && !first_reverse_step && pressed_key != DOWN)
        {
            push_reverse_way(reverse_cont, reverse_way, pressed_key);
            pressed_key = DOWN;
            reverse_cont++;
        }

        if (keys[0] == LEFT && !first_reverse_step && pressed_key == LEFT)
            reverse_cont++;
        else if (keys[0] == LEFT && !first_reverse_step && pressed_key != LEFT)
        {
            push_reverse_way(reverse_cont, reverse_way, pressed_key);
            pressed_key = LEFT;
            reverse_cont++;
        }

        if (keys[0] == RIGHT && !first_reverse_step && pressed_key == RIGHT)
            reverse_cont++;
        else if (keys[0] == RIGHT && !first_reverse_step && pressed_key != RIGHT)
        {
            push_reverse_way(reverse_cont, reverse_way, pressed_key);
            pressed_key = RIGHT;
            reverse_cont++;
        }
    } 
    else
    {
        push_reverse_way(reverse_cont, reverse_way, pressed_key);
        first_reverse_step = true;

        if (reverse_way.size() != 0)
        { //ALGORITMO CAMMINO INVERSO
            if (reverse_way[reverse_way.size() - 1].first > 0)
                do_stuff = true;

                reverse_way[reverse_way.size() - 1].first--;
            if (reverse_way[reverse_way.size() - 1].first == 0)
                reverse_way.pop_back();
        }
    }
    //GENA
*/

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
