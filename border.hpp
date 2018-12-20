/*
Border
  |
  v
Entity -> Player -> Enemy -> Mob -> Spawn -> main
                              |       |
                              v       v
                             ...     ...

*/

#ifndef BORDER_HPP
#define BORDER_HPP

#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<iostream>
#include<algorithm>
#include<iterator>
#include<utility>
#include<list>
#include<array>
#include<cstdlib>
#include<ctime>
using namespace std;

 enum Side : int {NONE, HORIZONTAL, VERTICAL};

 bool _range(int, int, int);
 bool hitbox(int, int, int, int, int, int, int, int);

 class Border
{
    private:
        list<pair<int, int>> border;
        list<pair<int, int>> trace;

        list<pair<int, int>>::iterator next( list<pair<int, int>>::iterator );
        //void update_border(); //fa lo splice di trace in border
    
    public:
        ALLEGRO_BITMAP* border_pic;
        ALLEGRO_BITMAP* trace_pic;

        Border() {}
        Border(list<pair<int, int>>::iterator bb, list<pair<int, int>>::iterator eb,
               list<pair<int, int>>::iterator bt, list<pair<int, int>>::iterator et,
               ALLEGRO_BITMAP* _b = nullptr, ALLEGRO_BITMAP* _t = nullptr):
                   border(bb, eb), border_pic(_b), trace(bt, et), trace_pic(_t) {}
        
        Side in_border(array<float, 4>); //verifica se l'oggetto è sul bordo
        Side in_trace (array<float, 4>); //verifica se l'oggetto è sulla traccia
        int add_point(pair<int, int>); //aggiunge un punto alla traccia

};

#endif