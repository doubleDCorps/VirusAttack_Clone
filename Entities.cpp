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

void push_reverse_way(int &reverse_cont, vector<pair<int, int>> &reverse_way, int &pressed_key) {
    if(reverse_cont!=0) {
        if(pressed_key==UP) //UP
            reverse_way.push_back({reverse_cont, DOWN});
        else if(pressed_key==LEFT) //LEFT
            reverse_way.push_back({reverse_cont, RIGHT});
        else if(pressed_key==DOWN) //DOWN
            reverse_way.push_back({reverse_cont, UP});
        else if(pressed_key==RIGHT) //RIGHT
            reverse_way.push_back({reverse_cont, LEFT});

        reverse_cont=0;
        }
}

 void Player::update(const GameList& GL)
{
    if(keys[1] != still and !GL.onEdge(data.center()).first) {
            if(keys[0] != still && first_reverse_step) {
                first_reverse_step=false;
                pressed_key=keys[0];
                }

            if(keys[0] == UP && !first_reverse_step && pressed_key == UP)
                reverse_cont++;
            else if(keys[0] == UP && !first_reverse_step && pressed_key != UP) {
                push_reverse_way(reverse_cont, reverse_way, pressed_key);
                pressed_key = UP;
                reverse_cont++;
                }
            
            if(keys[0] == DOWN && !first_reverse_step && pressed_key == DOWN)
                reverse_cont++;
            else if(keys[0] == DOWN && !first_reverse_step && pressed_key != DOWN) {
                push_reverse_way(reverse_cont, reverse_way, pressed_key);
                pressed_key = DOWN;
                reverse_cont++;
                }

            if(keys[0] == LEFT && !first_reverse_step && pressed_key == LEFT)
                reverse_cont++;
            else if(keys[0] == LEFT && !first_reverse_step && pressed_key != LEFT) {
                push_reverse_way(reverse_cont, reverse_way, pressed_key);
                pressed_key = LEFT;
                reverse_cont++;
                }

            if(keys[0] == RIGHT && !first_reverse_step && pressed_key == RIGHT)
                reverse_cont++;
            else if(keys[0] == RIGHT && !first_reverse_step && pressed_key != RIGHT) {
                push_reverse_way(reverse_cont, reverse_way, pressed_key);
                pressed_key = RIGHT;
                reverse_cont++;
                }
                
            if(reverse_way.size()!=0)
                if(keys[0] == reverse_way[reverse_way.size()-1].second) {
                    reverse_way[reverse_way.size()-1].first--;
                    reverse_cont--; //se diventasse =0?
                    if(reverse_way[reverse_way.size()-1].first==0)
                        reverse_way.pop_back();
                    }

            }
    else if(keys[1]==still) {    
        push_reverse_way(reverse_cont, reverse_way, pressed_key);
        first_reverse_step=true;

        /*//____________ STAMPA TEST ____________
        cout << endl;
        for(unsigned i=0;i<reverse_way.size();i++)
            cout << reverse_way[i].first << " " << reverse_way[i].second << endl;
        cout << endl;
        //_____________________________________*/

            if(!reverse_way.empty()) { //ALGORITMO CAMMINO INVERSO
                if(reverse_way[reverse_way.size()-1].first>=0) {
                    keys[0] = (KEYS)reverse_way[reverse_way.size()-1].second;
                    force_reverse_way = true;
                }

                reverse_way[reverse_way.size()-1].first--;
                if(reverse_way[reverse_way.size()-1].first==0)
                    reverse_way.pop_back();
            }
    }

    if(GL.onEdge(data.center()).first) {
    
        reverse_way.clear();
        first_reverse_step=true;
        reverse_cont=0;
        pressed_key = still;
        force_reverse_way = false;
    }

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

     if(keys[0]!=still and directions[keys[0]-1]==1 and (keys[1]!=still or (keys[1]==still and force_reverse_way)))
    {
        if(keys[0]==UP or keys[0]==DOWN)    data.y( data.y() + data.vy() );
        else                                data.x( data.x() + data.vx() );
    
    } else if(keys[0]!=still and directions[keys[0]-1]==2)
    {
        if(keys[0]==UP or keys[0]==DOWN)    data.y( data.y() + data.vy() );
        else                                data.x( data.x() + data.vx() );
    }

    safe = GL.onEdge(data.center()).first ? true : false;
}