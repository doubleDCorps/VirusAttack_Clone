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
    AXIS r1 = collinear(P1);
    AXIS r2 = collinear(P2);
            
    return
        r1 != none and 
        r2 != none and 
        (r1 == XY or r2 == XY or r1 == r2);
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
        {p[0] + c[0]/2, 
         p[1] + c[1]/2 };
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
    float offX {v[0] < 0 ? -1.0f : 1.0f};
    float offY {v[1] < 0 ? -1.0f : 1.0f};
     
     return
    {   p[0] + projDir_x[i-1]*v[0]*offX*3.0f,
        p[1] + projDir_y[i-1]*v[1]*offY*3.0f,
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
    if(empty() or
        (P != back() and
            (size()==1 and P.collinear(back())) or 
            (size()>=2 and !P.collinear(back(), *(++rbegin())))))
    {
        push_back(P);
    }
    //else if(size()==1 and P.collinear(front()) or (size()>=2 and !P.collinear(front(), *(++begin()))))          push_front(P);
    else if(size()>=2 and P.collinear(back(), *(++rbegin())) )
    {
        if(P == back() and P == *(++rbegin()))
            pop_back();
        back() = P;
    }
    //else if(size()>=2 and P.collinear(front(), *(++begin())))                                                   front() = P;
    else return false;
    
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
        if(x2 != x3 and P.collision({min(x2, x3), y2-6, abs(x2-x3), 12}))
            return 
                {Y, y1 <= y2-6 ? true : false};
        // il segmento è verticale
        // verifica se avviene una collisione fra le hitbox
        //if(y2 != y3 and P.collision({x2, min(y2, y3), 7, abs(y2-y3)})) return {X, x1 <= x2 ? true : false};
        if(y2 != y3 and P.collision({x2-6, min(y2, y3), 12, abs(y2-y3)})) 
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
 
 void GameList::u_print(ALLEGRO_BITMAP* buffer) const
{
    if(empty()) return;

     if(buffer!=nullptr)
    {
        al_set_target_bitmap(buffer);
         for(auto it{ begin() }; successor(it) != begin(); ++it)
        {
            if(abs(it->x() - successor(it)->x()) <= PointData::EPS)
                al_draw_line(it->x(), min(it->y(), successor(it)->y())-thickness/2.0,
                             it->x(), max(it->y(), successor(it)->y())+thickness/2.0,
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

/*
 AXIS GameList::hits(const EntityData& P) const //fixare i nomi delle variabili
{
    if(empty()) return none;

     for(auto it{ begin() }; it != end(); ++it)
    {
        // coordinate degli estremi del segmento
        int x2{ it->first }, x3{ successor(it)->first };
        int y2{ it->second }, y3{ successor(it)->second };

        // il segmento è orizzontale
        // la velocità e la distanza hanno segno concorde (con uno scarto di sicurezza)
        // verifica se avviene una collisione fra le hitbox

        if(x2 != x3 &&
          (vy>=0 && y2-y1-h1/2 > 0 && hitbox(x1, y1, x1+w1, y1+h1, x2, y2, x3, y2+14))
        ||(vy<=0 && y2-y1-h1/2 < 0 && hitbox(x1, y1, x1+w1, y1+h1, x2, y2, x3, y2-14)))
            return Y;
        
        // il segmento è verticale
        // la velocità e la distanza hanno segno concorde (con uno scarto di sicurezza)
        // verifica se avviene una collisione fra le hitbox

        if(y2 != y3 &&                                                   
          (vx>=0 && x2-x1-w1/2 > 0 && hitbox(x1, y1, x1+w1, y1+h1, x2, y2, x2+14, y3))
        ||(vx<=0 && x2-x1-w1/2 < 0 && hitbox(x1, y1, x1+w1, y1+h1, x2, y2, x2-14, y3)))          
            return X;
    }
    
    return none;
}
*/