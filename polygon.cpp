#include "polygon.hpp"

 bool _range(int first, int second, int lower_bound, int upper_bound)
{
    return (first >= lower_bound && first <= upper_bound)
        || (second >= lower_bound && second <= upper_bound);
}

 bool hitbox(int x1a, int y1a, int x1b, int y1b, int x2a, int y2a, int x2b, int y2b)
{
    return
        ((_range(x1a, x1b, min(x2a, x2b), max(x2a, x2b)))
        || (_range(x2a, x2b, min(x1a, x1b), max(x1a, x1b))))
    &&  ((_range(y1a, y1b, min(y2a, y2b), max(y2a, y2b)))
        || (_range(y2a, y2b, min(y1a, y1b), max(y1a, y1b))));
}

 list<pair<int,int> >::const_iterator Polygon::successor(list<pair<int,int> >::const_iterator it, const list<pair<int,int> >& ref) const
{
    return 
        ++it == ref.end() ? ref.begin() : it;
}

 AXIS Polygon::hits(const array<float, 6>& p, const list<pair<int,int> >& ref) const
{
    //p : [0] = coord_x, [1] = coord_y, [2] = width, [3] = height, [4] = x_velocity, [5] = y_velocity
    //spessore hitbox bordo/traccia : 20  
 
     for(auto it {ref.begin()}; it != ref.end(); ++it)
    {
        if((p[5] >= 0 && it->second - p[1] > -5) || (p[5] <= 0 && it->second - p[1] < 5))
         if(it->first != successor(it, ref)->first)
        {
            if(hitbox(p[0], p[1], p[0] + p[2], p[1] + p[3], 
                                it->first, it->second, successor(it, ref)->first, it->second + 20))     
                return Y;
        }
        if((p[4] >= 0 && it->first - p[0] > -5) || (p[4] <= 0 && it->first - p[0] < 5))
         if(it->second != successor(it, ref)->second)
        {
            if(hitbox(p[0], p[1], p[0] + p[2], p[1] + p[3], 
                                it->first, it->second, it->first + 20, successor(it, ref)->second))
                return X;
        }
    }
    
    return none;
}

 void Polygon::updatePolygon() //DIPENDE DAL BOSS
{
    //front di trace va nella corretta posizione
    //back di trace va nella corretta posizione
    //elimina gli elementi in mezzo
    //splice del resto
}

 AXIS Polygon::hitsB(array<float, 6> p) const { return hits(p, border); }

 AXIS Polygon::hitsT(array<float, 6> p) const { return hits(p, trace);  }

 bool is_inside(pair<int, int>) const
{
    //DA IMPLEMENTARE
    //how?
    return false;
}

 bool Polygon::add_point(pair<int, int> p)
{
    if((trace.back().first  == (--trace.rbegin())->first  && trace.back().first == p.first) ||
       (trace.back().second == (--trace.rbegin())->second && trace.back().second == p.second))
    {
        trace.back() = p;
         if(hitsB({trace.back().first, trace.back().second, 0, 0, 0, 0} != none)
        {
            updatePolygon();
            return true;
        }
    }
     else
    {
        trace.push_back(p);
         if(hitsB({trace.back().first, trace.back().second, 0, 0, 0, 0} != none)
        {   
            updatePolygon();
            return true;
        }
    }

    return false;
}

 int Polygon::getArea() const
{
    int area{};

    for(auto it{ border.begin() }; it != border.end(); ++it)
        area += (it->first + successor(it, border)->first) *
                (successor(it, border)->second - it->second);

    return (area*50)/(base_h*base_w);
}