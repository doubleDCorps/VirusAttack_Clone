#include"GameList.h"

 ostream& operator<<(ostream& os, const GameList& GL) //stampe di prova, usate per il debugging
{
    for(auto& it : GL)
        os << " [" << it.x() << ' ' << it.y() << ']';

    return os;
}

//cambiando la policy d'inserimento probabilmente si risolverebbero un sacco di problemi
 bool GameList::pushPoint(const PointData& P)
{
    //?
    if(find(begin(), end(), P) != end())
        return false;
    //?
    if(empty() or (P.collinear(back()) and (size()==1 or (size()>=2 and !P.collinear(back(), *(++rbegin())))))) {
        push_back(P); 
        return true;
    }
    //?
    if(size()>=2 and P.collinear(back(), *(++rbegin()))) {
        back() = P; 
        return true;
    }
    
    return false;
}

 pair<AXIS, bool> GameList::onEdge(const HitboxData& P) const
{
//  non ha un cazzo di senso, perchè?
    if(empty())
        return {XY, true};

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
    if(empty())
        return false;

    int cont{};

     for(auto it{ begin() }; it != end(); ++it)
        if(it->x() == successor(it)->x()) {
            float y1{ it->y() };
            float y2{ successor(it)->y() };

             if(P.collision({0, min(y1,y2), it->x(), abs(y1-y2)}))
            { 
                if(y2 < y1) --cont;
                if(y2 > y1) ++cont;
            }
        }

    return cont == 0 ? false : true;
}

 AXIS GameList::collides(const EntityData& E) const
{
    pair<AXIS, bool> cond = onEdge(E);
    
    if(cond.first == X and ((E.vx()>=0 and cond.second) or (E.vx()<0 and !cond.second))) return X;
    if(cond.first == Y and ((E.vy()>=0 and cond.second) or (E.vy()<0 and !cond.second))) return Y;
    
    return none;
}

 bool GameList::o_inside(const HitboxData& P) const { return !(onEdge(P).first) and inArea(P); }
 bool GameList::c_inside(const HitboxData& P) const { return   onEdge(P).first  or  inArea(P); }

 bool GameList::is_closed() const
{
    if(size() <= 2)
        return false;
    for(auto it{begin()}; it != end(); ++it)
     if(!it->collinear(*successor(it)))
        return false;
    return true;
}

 PointData GameList::nearestPoint(const PointData& P) const
{
    perimeter::const_iterator output{begin()};
    float dist = 100000.0F;
    float condition;

     for(auto it{ begin() }; it != end(); ++it)
    {
        condition = abs(P.distance(P.projection(*it, *successor(it))));
         if(condition < dist)
        {
            dist = condition;
            output = it;
        }
    }
    
    return P.projection(*output, *successor(output));
}

 pair<PointData, PointData> GameList::nearestLine(const PointData& P) const
{
    perimeter::const_iterator t1{cbegin()};
    float dist = 10000.0F;
    float condition;

     for(auto it{cbegin()}; it != cend(); ++it)
    {
        condition = abs(P.distance(P.projection(*it, *successor(it))));
         if(condition < dist)
        {
            dist = condition;
            t1 = it;
        }
    }

    return {*t1, *successor(t1)};
}

//DA RIFARE
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
                al_draw_line(min(it->x(), successor(it)->x())-thickness/2.0F, it->y(),
                             max(it->x(), successor(it)->x())+thickness/2.0F, it->y(), 
                             color, thickness);
        }
    }
}

//DA RIFARE
 void GameList::l_print(ALLEGRO_BITMAP* buffer) const
{
    if(empty()) return;

     if(buffer!=nullptr)
    {
        al_set_target_bitmap(buffer);
         for(auto it{ begin() }; it != end(); ++it)
        {
            if(abs(it->x() - successor(it)->x()) <= PointData::EPS)
                al_draw_line(it->x(), min(it->y(), successor(it)->y())-thickness/2.0F,
                             it->x(), max(it->y(), successor(it)->y())+thickness/2.0F,
                             color, thickness);

            else if(abs(it->y() - successor(it)->y()) <= PointData::EPS)
                al_draw_line(min(it->x(), successor(it)->x())-thickness/2.0F, it->y(),
                             max(it->x(), successor(it)->x())+thickness/2.0F, it->y(), 
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