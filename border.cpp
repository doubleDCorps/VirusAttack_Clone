#include "border.hpp"

 bool _range(int contains, int lower, int upper)
{
    return contains >= lower && contains <= upper;
}

 bool hitbox(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    return
        ((_range(x1, min(x2, x2 + w2), max(x2, x2 + w2)) || _range(x1 + w1, min(x2, x2 + w2), max(x2, x2 + w2)))
      || (_range(x2, min(x1, x1 + w1), max(x1, x1 + w1)) || _range(x2 + w2, min(x1, x1 + w1), max(x1, x1 + w1)))
        )
    &&  ((_range(y1, min(y2, y2 + h2), max(y2, y2 + h2)) || _range(y1 + h1, min(y2, y2 + h2), max(y2, y2 + h2)))
      || (_range(y2, min(y1, y1 + h1), max(y1, y1 + h1)) || _range(y2 + h2, min(y1, y1 + h1), max(y1, y1 + h1)))
        );
}


 list<pair<int, int>>::iterator Border::next( list<pair<int, int>>::iterator it )
{
    return 
        (++it == border.end() ? border.begin() : it);
}

 Side Border::in_border(array<float, 4> p)
{
     for(auto it {border.begin()}; it != border.end(); ++it)
    {
        if(it->first != next(it)->first)
        if( hitbox( p[0], p[1], p[2], p[3], it->first, it->second, next(it)->first - it->first, 20))
            return HORIZONTAL;
        
        if(it->second != next(it)->second)
        if( hitbox( p[0], p[1], p[2], p[3], it->first, it->second, 20, next(it)->second - it->second))
            return VERTICAL;
    }

    return NONE;
}

 Side Border::in_trace(array<float, 4> p)
{
    int dx, dy;
    bool h, v;

     for(auto it{ trace.begin() }; it != trace.end(); ++it)
    {
        dx = it->first  == next(it)->first  ? prev(it)->first  : next(it)->first;
        h = hitbox(p[0], p[1], p[2], p[3], it->first, it->second, dx - it->first,  5);
            if(h) return HORIZONTAL;
        
        dy = it->second == next(it)->second ? prev(it)->second : next(it)->second;
        v = hitbox(p[0], p[1], p[2], p[3], it->first, it->second, 5, dy - it->second);
            if(v) return VERTICAL;
    }

    return NONE;
}

 int Border::add_point(pair<int, int> p)
{
    trace.push_back(p);
    return -1;
}

/*     
#include<iostream>
 int main()
{
    list< pair<int, int> > test_list{{0, 0}, {800, 0}, {800, 600}, {0, 600}};
    list< pair<int, int> > test_cist{{100, 0}, {100, 40}, {0, 40}, {0, 0}};
    
    Border test(test_list.begin(), test_list.end(), test_cist.begin(), test_cist.end(), nullptr, nullptr);

    cout << test.in_border({0, 0, 32, 32}) << test.in_trace({11, 7, 32, 32}) << endl;

    return 0;
}*/