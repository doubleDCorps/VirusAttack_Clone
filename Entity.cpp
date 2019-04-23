#include"Entity.h"

//Metodi della classe PointData

 AXIS PointData::collinear(const PointData& P) const  
{ 
    bool by = abs(p[1] - P.p[1])<=EPS;
    bool bx = abs(p[0] - P.p[0])<=EPS;
            
    return
        bx and by ? XY :
        bx        ? X  : 
               by ? Y  : none;
}

 bool PointData::collinear(const PointData& P1, const PointData& P2) const
{
    //static AXIS past1 = none, past2 = none;

    AXIS r1 = collinear(P1);
    AXIS r2 = collinear(P2);
    /*
    if(past1 != r1 or past2 != r2)
    {
        switch(r1){
            case none:  cout << "none "; break;
            case X:     cout << "X "; break;
            case Y:     cout << "Y "; break;
            case XY:    cout << "XY "; break;
        }
        switch(r2){
            case none:  cout << " none "; break;
            case X:     cout << " X "; break;
            case Y:     cout << " Y "; break;
            case XY:    cout << " XY "; break;
        }
        cout << endl;
        past1 = r1;
        past2 = r2;
    }
    */
    return
        r1 != none and 
        r2 != none and 
        (r1 == XY or r2 == XY or r1 == r2);
}

 PointData PointData::projection(const PointData& P1, const PointData& P2) const
{
    AXIS cond = P1.collinear(P2);
    
    if(cond == X)       return {P1.p[0], p[1]};
    else if(cond == Y)  return {p[0], P1.p[1]};
    else if(cond == XY) return P1;
    
    float x0 = min(P1.p[0], P2.p[0]),       y0 = min(P1.p[1], P2.p[1]);
    float tx = max(P1.p[0], P2.p[0]) - x0,  ty = max(P1.p[1], P2.p[1]) - y0;    
    float x = abs(p[0]-x0),                 y = abs(p[1]-y0);

    float k = (x*tx + y*ty)/(tx*tx + ty*ty);

    return { k*x, k*y };
}

//Metodi della classe HitboxData

 PointData HitboxData::point(unsigned i) const
{ 
    return 
        {p[0] + hitDir_x[i] * c[0],
         p[1] + hitDir_y[i] * c[1] };
}

 PointData HitboxData::center() const
{ 
    return
        {p[0] + c[0]/2.0F, 
         p[1] + c[1]/2.0F };
}

 bool HitboxData::collision(const HitboxData& D) const 
{
    return
        (in_range(p[0],          D.p[0], D.p[0]+D.c[0]) or
         in_range(p[0]+c[0],     D.p[0], D.p[0]+D.c[0]) or
         in_range(D.p[0],        p[0],   p[0]+c[0]    ) or 
         in_range(D.p[0]+D.c[0], p[0],   p[0]+c[0]    ))
        and
        (in_range(p[1],          D.p[1], D.p[1]+D.c[1]) or 
         in_range(p[1]+c[1],     D.p[1], D.p[1]+D.c[1]) or 
         in_range(D.p[1],        p[1],   p[1]+c[1]    ) or 
         in_range(D.p[1]+D.c[1], p[1],   p[1]+c[1]    ));
}

//Metodi della classe EntityData

 HitboxData EntityData::projection(unsigned i) const
{ 
    float offX {v[0] < 0 ? -1.0F : 1.0F};
    float offY {v[1] < 0 ? -1.0F : 1.0F};
     
     return
    {   p[0] + projDir_x[i-1]*v[0]*offX*3.0F,
        p[1] + projDir_y[i-1]*v[1]*offY*3.0F,
        c[0], 
        c[1]
    };
} 

//Metodi della classe GameList

 ostream& operator<<(ostream& os, const GameList& GL) //stampe di prova, usate per il debugging
{
    for(auto& it : GL)
        os << "\n[" << it.x() << ' ' << it.y() << ']';

    return os;
}

 bool GameList::pushPoint(const PointData& P)
{
    if(size()>=2 and P.collinear(back(), *(++rbegin())))
    {
        back() = P;
    }
    else if(empty() or
        (size()==1 and P.collinear(back()) and P != back()) or 
        (size()>=2 and !P.collinear(back(), *(++rbegin())) and P != back()))
    {
        
        push_back(P);
    }
    else
        return false;

    if(size()>=2 and P==back() and P==*(++rbegin()))
    {
        pop_back();
    }

    return true;
}

 pair<AXIS, bool> GameList::onEdge(const HitboxData& P) const
{
    if(empty()) return {XY, true};

     for(auto it{ begin() }; it != end(); ++it)
    {
        // coordinate degli estremi del segmento
        float x1{ P.x() }  , w1{ P.dx() };
        float y1{ P.y() }  , h1{ P.dy() };
        float x2{ it->x() }, x3{ successor(it)->x() };
        float y2{ it->y() }, y3{ successor(it)->y() };

        // il segmento è orizzontale
        // verifica se avviene una collisione fra le hitbox
        //if(x2 != x3 and P.collision({min(x2, x3), y2, abs(x2-x3), 7})) return {Y, y1 <= y2 ? true : false}; 
        if(x2!=x3 and P.collision({min(x2, x3), y2-6, abs(x2-x3), 12}))
            return 
                {Y, y1 <= y2-6 ? true : false};
        // il segmento è verticale
        // verifica se avviene una collisione fra le hitbox
        //if(y2 != y3 and P.collision({x2, min(y2, y3), 7, abs(y2-y3)})) return {X, x1 <= x2 ? true : false};
        if(y2!=y3 and P.collision({x2-6, min(y2, y3), 12, abs(y2-y3)})) 
            return 
                {X, x1 <= x2-6 ? true : false};
    }

    return {none, false};
}

 bool GameList::inArea(const HitboxData& P) const
{
    if(empty()) return true;

    int cont{};

     for(auto it{ begin() }; it != end(); ++it)
    {
        float y1{ it->y() }, y2{ successor(it)->y() };

         if(P.collision({0, min(y1, y2), it->x(), abs(y1-y2)}))
        { 
            if(y2 < y1)      --cont;
            else if(y2 > y1) ++cont;
        }
    }

    return cont <= 0 ? false : true;
}

 AXIS GameList::collides(const EntityData& E) const
{
    pair<AXIS, bool> cond = onEdge(E);
    
    if(cond.first == X and ((E.vx()>=0 and cond.second) or (E.vx()<0 and !cond.second))) return X;
    if(cond.first == Y and ((E.vy()>=0 and cond.second) or (E.vy()<0 and !cond.second))) return Y;
    
    return none;
}

 bool GameList::o_inside(const HitboxData& P) const { return !(onEdge(P).first) and inArea(P); }
 bool GameList::c_inside(const HitboxData& P) const { return onEdge(P).first or inArea(P); }

 PointData GameList::nearestPoint(const PointData& P) const
{
    PointData temp = {0.0F, 0.0F};
    PointData dist = {10000.0F, 10000.0F};

    for(const PointData& i : *this)
     if(abs(i.x() - P.x()) < dist.x() or (abs(i.x() - P.x()) == dist.x() and abs(i.y() - P.y()) < dist.y()))
    {
        dist.x(abs(i.x() - P.x()));
        dist.y(abs(i.y() - P.y()));
        temp = i;
    }

    return temp;
}

 pair<PointData, PointData> GameList::nearestLine(const PointData& P) const
{
    perimeter::const_iterator t1{cbegin()};
    perimeter::const_iterator t2{successor(cbegin())};
    float dist = 10000.0F;

     for(auto it{cbegin()}; it != cend(); ++it)
    {
         if(it->collinear(*successor(it)) == X and abs(P.x() - it->x()) < dist)
        {
            dist = abs(P.x() - it->x());
            t1 = it;
            t2 = successor(it);
        }
         else if(it->collinear(*successor(it)) == Y and abs(P.y() - it->y()) < dist)
        {
            dist = abs(P.y() - it->y());
            t1 = it;
            t2 = successor(it);
        }
    }

    return {*t1, *t2};
}

 void GameList::u_print(ALLEGRO_BITMAP* buffer) const
{
    if(empty()) return;

     if(buffer!=nullptr)
    {
        al_set_target_bitmap(buffer);
         for(auto it{ begin() }; successor(it) != begin(); ++it)
        {
            if(abs(it->x() - successor(it)->x()) <= PointData::EPS)
                al_draw_line(it->x(), min(it->y(), successor(it)->y())-thickness/2.0F,
                             it->x(), max(it->y(), successor(it)->y())+thickness/2.0F,
                             color, thickness);

            else if(abs(it->y() - successor(it)->y()) <= PointData::EPS)
                al_draw_line(min(it->x(), successor(it)->x())-thickness/2.0, it->y(),
                             max(it->x(), successor(it)->x())+thickness/2.0, it->y(), 
                             color, thickness);
        }
    }
}

 void GameList::l_print(ALLEGRO_BITMAP* buffer) const
{
    if(empty()) return;

     if(buffer!=nullptr)
    {
        al_set_target_bitmap(buffer);
         for(auto it{ begin() }; it != end(); ++it)
        {
            if(abs(it->x() - successor(it)->x()) <= PointData::EPS)
                al_draw_line(it->x(), min(it->y(), successor(it)->y())-thickness/2,
                             it->x(), max(it->y(), successor(it)->y())+thickness/2,
                             color, thickness);

            else if(abs(it->y() - successor(it)->y()) <= PointData::EPS)
                al_draw_line(min(it->x(), successor(it)->x())-thickness/2, it->y(),
                             max(it->x(), successor(it)->x())+thickness/2, it->y(), 
                             color, thickness);
        }
    }
}
/*
 bool GameList::inside(const HitboxData& P) const //fixare i nomi delle variabili
{
    if(empty()) return true;

    int cont{};

     for(auto it{ begin() }; it != end(); ++it)
    {
        int y1{ it->second };
        int y2{ successor(it)->second };

         if(y2-y1!=0 && hitbox(x, y, x+w, y+h, 0, min(y1, y2), it->first, max(y1, y2)) )
        {
            if(y2-y1 < 0) --cont;
            if(y2-y1 > 0) ++cont;
        }
    }

    return !hits(x, y, w, h, 0, 0) && cont <= 0 ? false : true;
}
*/