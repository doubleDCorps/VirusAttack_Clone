#include "border.hpp"

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


 list<pair<int, int>>::iterator Border::next( list<pair<int, int>>::iterator it )
{
    return 
        (++it == border.end() ? border.begin() : it);
}

 Side Border::in_border(array<float, 4> p)
{
    Side temp{NONE};
     for(auto it {border.begin()}; it != border.end(); ++it)
    {
        if(it->first != next(it)->first)
         if( hitbox( p[0], p[1], p[0] + p[2], p[1] + p[3], it->first, it->second, next(it)->first, it->second + 20))
        {
            if(temp == VERTICAL) return BOTH;
            temp = HORIZONTAL;
        }
        
        if(it->second != next(it)->second)
         if( hitbox( p[0], p[1], p[0] + p[2], p[1] + p[3], it->first, it->second, it->first + 20, next(it)->second))
        {
            if(temp == HORIZONTAL) return BOTH;
            temp = VERTICAL;
        }
    }

    return temp;
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