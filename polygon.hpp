/*
Entity -> Player -> Enemy -> Boss -> Minion -> Polygon -> main
                              |       |
                              v       v
                             ...     ...

*/

#ifndef POLY_HPP
#define POLY_HPP

#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<iostream>
#include<algorithm>
#include<iterator>
#include<utility>
#include<vector>
#include<list>
#include<array>
#include<cstdlib>
#include<ctime>
using namespace std;

 const int base_w = 500;
 const int base_h = 500;

 enum AXIS : int {none = 0, X = 1, Y = 2};

 inline bool _range(int first, int second, int lower_bound, int upper_bound);
 bool hitbox(int x1a, int y1a, int x1b, int y1b, int x2a, int y2a, int x2b, int y2b);

 class Polygon
{
    private:
        list<pair<int,int> > border;
        list<pair<int,int> > trace;
        
        ALLEGRO_BITMAP* border_pic;
        ALLEGRO_BITMAP* trace_pic;

        AXIS hits(const array<float, 6>&, const list<pair<int,int> >&) const;
        list<pair<int,int> >::const_iterator successor(list<pair<int,int> >::const_iterator, const list<pair<int,int> >&) const;
        void update_Polygon();
    
    public:
        Polygon(): border( {{125, 25}, {625, 25}, {625, 525}, {125, 525}} ) {}
        Polygon(list<pair<int,int> >::const_iterator bb, list<pair<int,int> >::const_iterator eb,
                ALLEGRO_BITMAP* _b = nullptr, ALLEGRO_BITMAP* _t = nullptr): 
                border(bb, eb), border_pic(_b), trace_pic(_t) {}
        
        AXIS hitsB (array<float, 6>) const;
        AXIS hitsT (array<float, 6>) const;
        bool is_inside(pair<int, int>) const; //determina la posizione rispetto al bordo (dentro/fuori)
        bool add_point(pair<int, int>);
        int getArea() const;

};

#endif