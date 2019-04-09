#ifndef ENTITY_H_
#define ENTITY_H_

#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include<array>
#include<iostream>
#include<utility>
#include<vector>
#include<list>
#include<cstdlib>
#include<cmath>
#include<map>
#include<cassert>
using namespace std;

enum AXIS : unsigned {none=0, X=1, Y=2, XY=3};
enum KEYS : unsigned {still=0, UP=1, LEFT=2, DOWN=3, RIGHT=4, SPACE=5};
enum DIRS : unsigned {NW=0, NE=1, SE=2, SW=3};

 inline bool in_range(int first, int lower_bound, int upper_bound) { return first >= lower_bound and first <= upper_bound; } 
/*
 inline bool hitbox(int x1a, int y1a, int x1b, int y1b, int x2a, int y2a, int x2b, int y2b)
{
    return
        (in_range(x1a, min(x2a, x2b), max(x2a, x2b)) || in_range(x1b, min(x2a, x2b), max(x2a, x2b)) || in_range(x2a, min(x1a, x1b), max(x1a, x1b)) || in_range(x2b, min(x1a, x1b), max(x1a, x1b)) )
     && (in_range(y1a, min(y2a, y2b), max(y2a, y2b)) || in_range(y1b, min(y2a, y2b), max(y2a, y2b)) || in_range(y2a, min(y1a, y1b), max(y1a, y1b)) || in_range(y2b, min(y1a, y1b), max(y1a, y1b)) );
}
*/
/*
    Due strutture dati elementari che definiscono i seguenti dati:
        HitboxData: coordinata x, coordinata y, larghezza, altezza
        EntityData: coordinata x, coordinata y, larghezza, altezza, velocità x, velocità y
    Il polimorfismo viene sfruttato per rendere più lineari alcune chiamate a funzione nella classe Level.
*/
 struct PointData
{
    protected:
        constexpr static float EPS{0.1};
        float p[2];

    public:
        PointData(float a, float b): p{a, b} {}
        PointData(const PointData& P): p{P.p[0], P.p[1]} {}
         PointData& operator=(const PointData& P)
        { 
             if(this != &P)
            { 
                p[0]=P.p[0];
                p[1]=P.p[1];
            } 
            return *this;
        }
        virtual ~PointData() {};

        float x() const { return p[0]; }
        float y() const { return p[1]; }
        void x(float x) { p[0] = x > 0 ? x : 0; } 
        void y(float y) { p[1] = y > 0 ? y : 0; }

        bool operator==(const PointData& P) const { return fabs(p[0] - P.p[0])<EPS and fabs(p[1] - P.p[1])<EPS; }
        bool operator!=(const PointData& P) const { return !(*this == P); }
         
         AXIS collinear(const PointData& P) const  
        { 
            bool by = fabs(p[1] - P.p[1])<EPS;
            bool bx = fabs(p[0] - P.p[0])<EPS;
            
            return
                bx and by ? XY : bx ? X : by ? Y : none;
        }
         
         bool collinear(const PointData& P1, const PointData& P2) const
        {
            AXIS r1 = collinear(P1);
            AXIS r2 = collinear(P2);
            
            return
                r1 != none && r2 != none && (r1 == XY || r2 == XY || r1 == r2);
        }
};

 class HitboxData: public PointData
{
    protected:
        constexpr static int hitDir_x[]{0, 1, 1, 0};
        constexpr static int hitDir_y[]{0, 0, 1, 1};

        float c[2];
    
    public:
        HitboxData(float a, float b, float d=0, float e=0): PointData{a, b}, c{d, e} {}
        HitboxData(const HitboxData& H): PointData(H), c{H.c[0], H.c[1]} {}
        HitboxData(const PointData& P): PointData(P), c{4, 4} {}
         HitboxData& operator=(const HitboxData& H)
        { 
            PointData::operator=(H);
             if(this!=&H)
            {
                c[0]=H.c[0];
                c[1]=H.c[1]; 
            } 
            
            return *this; 
        }
        virtual ~HitboxData() {};

        float dx() const { return c[0]; }
        float dy() const { return c[1]; }
        void dx(float x) { c[0] = x; }
        void dy(float y) { c[1] = y; }

        PointData point(unsigned i) const       { return {p[0] + hitDir_x[i] * c[0] , p[1] + hitDir_y[i] * c[1]  }; }
        PointData center() const                { return {p[0] + c[0]/2             , p[1] + c[1]/2              }; }

         bool collision(const HitboxData& D) const 
        {
            return
                (in_range(p[0], D.p[0], D.p[0]+D.c[0]) or in_range(p[0]+c[0], D.p[0], D.p[0]+D.c[0]) or in_range(D.p[0], p[0], p[0]+c[0]) or in_range(D.p[0]+D.c[0], p[0], p[0]+c[0]))
            and (in_range(p[1], D.p[1], D.p[1]+D.c[1]) or in_range(p[1]+c[1], D.p[1], D.p[1]+D.c[1]) or in_range(D.p[1], p[1], p[1]+c[1]) or in_range(D.p[1]+D.c[1], p[1], p[1]+c[1]));
        }
};

 struct EntityData: public HitboxData
{
    private:
        constexpr static int projDir_x[]{0 ,-1, 0, 1};
        constexpr static int projDir_y[]{-1, 0, 1, 0};

        float v[2];
    
    public:
        EntityData(float a, float b, float c=0, float d=0, float e=0, float f=0): HitboxData{a, b, e, f}, v{c, d} {}
        EntityData(const EntityData& E): HitboxData(E), v{E.v[0], E.v[1]} {}
        EntityData(const HitboxData& H): HitboxData(H), v{0, 0} {}
        EntityData(const PointData& P): HitboxData(P), v{0, 0} {}
         EntityData& operator=(const EntityData& E)
        { 
            HitboxData::operator=(E);
             if(this!=&E)
            {
                c[0]=E.c[0];
                c[1]=E.c[1]; 
            } 
            
            return *this; 
        }
        virtual ~EntityData() {};

        float vx() const { return v[0]; }
        float vy() const { return v[1]; }
        void vx(float x) { v[0] = x; }
        void vy(float y) { v[1] = y; }

         HitboxData projection(unsigned i) const
        { 
            float offX{v[0] < 0 ? -1.0f : 1.0f};
            float offY{v[1] < 0 ? -1.0f : 1.0f};
             return
            {   p[0] + projDir_x[i-1]*v[0]*offX*3.0f,
                p[1] + projDir_y[i-1]*v[1]*offY*3.0f,
                c[0], 
                c[1]
            }; 
        } 
};

/*
    GameList è una lista di coppie di coordinate circolare e ordinata secondo il seguente criterio:
    per ogni coppia di elementi a1 e a2 consecutivi, la coppia precedente a0 e a1 sarà tale che
    se una coordinata è in comune fra a0 e a1, allora la coordinata opposta sarà in comune fra a1 e a2.
    Poichè la lista è circolare, ciò può essere valido anche per la coppia aN a0 (ultimo elemento - primo elemento).
    Inoltre GameList contiene un puntatore alla bitmap, utilizzata per rappresentare graficamente i segmenti
    composti da ogni coppia di elementi della lista.
*/
 class Level;

 typedef list<PointData> perimeter;

 class GameList: private perimeter
{
    friend Level;

    public:
        GameList(): thickness(10), color(al_map_rgb(0, 0, 0)) {}
        GameList(const perimeter& p, float t = 10, ALLEGRO_COLOR c = al_map_rgb(0, 0, 0)): thickness(t), color(c) { for(auto& i : p) pushPoint(i); }
        
        bool pushPoint(const PointData&);
        pair<AXIS, bool> onEdge(const HitboxData&) const;
        bool inArea(const HitboxData&) const;

        AXIS collides(const EntityData&) const;
        bool o_inside(const HitboxData&) const;
        bool c_inside(const HitboxData&) const;

        void o_print(ALLEGRO_BITMAP* buffer=nullptr) const;
        void c_print(ALLEGRO_BITMAP* buffer=nullptr) const;

    private:
        float thickness;
        ALLEGRO_COLOR color;
        
        inline auto successor(perimeter::iterator it)                       { return ++it == end() ? begin() : it; }
        inline auto successor(perimeter::const_iterator it) const           { return ++it == cend() ? cbegin() : it; }
        inline auto successor(perimeter::reverse_iterator it)               { return ++it == rend() ? rbegin() : it; }
        inline auto successor(perimeter::const_reverse_iterator it) const   { return ++it == crend() ? crbegin() : it; }
};

 class Entity
{
    protected:
        ALLEGRO_BITMAP *image;
        EntityData data;
        unsigned lifes;

    public:
        Entity(float x, float y, float vx, float vy, ALLEGRO_BITMAP* p, unsigned l): data(x, y, vx, vy, al_get_bitmap_width(p)+4, al_get_bitmap_height(p)+4), image(p), lifes(l) {}
        virtual ~Entity() {};

        virtual void update(const GameList&) = 0;

        const ALLEGRO_BITMAP *getBitmap() const { return image; }
        ALLEGRO_BITMAP *getBitmap()             { return image; }
         void setBitmap(ALLEGRO_BITMAP* p)       
        {
            image = p;
             if(p!=nullptr)
            {
                data.dx( al_get_bitmap_width(p)+4 );
                data.dy( al_get_bitmap_height(p)+4 );
            }
        }

        bool isAlive() const                    { return lifes > 0; }
        void setAlive(unsigned k)               { lifes = k; } 
        
        const EntityData& getData() const       { return data; }
        //l'intero gruppo di setter e getter vanno ripensati
        float getCord_x() const                 { return data.x()+2; }
        float getCord_y() const                 { return data.y()+2; }
        float getVelocity_x() const             { return data.vx(); }
        float getVelocity_y() const             { return data.vy(); }

        void setCord_x(float cx)                { data.x(cx-2); }
        void setCord_y(float cy)                { data.y(cy-2); }
        //ERRORE: nella classe Player le velocità devono sempre essere le stesse
        void setVelocity_x(float vx)            { data.vx(vx); }
        void setVelocity_y(float vy)            { data.vy(vy); }
};

#endif