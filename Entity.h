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
using namespace std;

const float EPS = 0.05;
enum AXIS : int {none=0, X=1, Y=2};
enum KEYS : int {still=0, UP=1, LEFT=2, DOWN=3, RIGHT=4};

 inline bool in_range(int first, int lower_bound, int upper_bound)
{ 
    return first >= lower_bound && first <= upper_bound;
}
/*
    Date due hitbox quadrate, definite da quattro coordinate (due su x e due su y)
    hitbox() verifica se un punto o una parte di una hitbox ha compenetrato l'altra,
    ovvero se una parte delle coordinate di una hitbox è compreso fra la coordinata minore e maggiore 
    dell'altra hitbox (questo contemporaneamente su entrambi gli assi, x e y, per evitare
    che, ad esempio, oggetti con stesse coordinate x ma estremamente distanti lungo l'altro asse y
    siano erroneamente considerati come compenetrazione).
*/
 inline bool hitbox(int x1a, int y1a, int x1b, int y1b, int x2a, int y2a, int x2b, int y2b)
{
    return
        (in_range(x1a, min(x2a, x2b), max(x2a, x2b)) || in_range(x1b, min(x2a, x2b), max(x2a, x2b)) ||
         in_range(x2a, min(x1a, x1b), max(x1a, x1b)) || in_range(x2b, min(x1a, x1b), max(x1a, x1b)) )
     && (in_range(y1a, min(y2a, y2b), max(y2a, y2b)) || in_range(y1b, min(y2a, y2b), max(y2a, y2b)) ||
         in_range(y2a, min(y1a, y1b), max(y1a, y1b)) || in_range(y2b, min(y1a, y1b), max(y1a, y1b)) );
}
/*
    Due strutture dati elementari che definiscono i seguenti dati:
        HitboxData: coordinata x, coordinata y, larghezza, altezza
        EntityData: coordinata x, coordinata y, larghezza, altezza, velocità x, velocità y
    Il polimorfismo viene sfruttato per rendere più lineari alcune chiamate a funzione nella classe Level.
*/
 class PointData
{
    private:
        float p[2];

    public:
        PointData(float a, float b): p{a, b} {}
        virtual ~PointData() {};

        float x() const { return p[0]; }
        float y() const { return p[1]; }
        void x(float x) { p[0] = x > 0 ? x : 0; } 
        void y(float y) { p[1] = y > 0 ? y : 0; }

        bool operator==(const PointData& P) const { return abs(p[0] - P.p[0]) < EPS && abs(p[1] - P.p[1]) < EPS; }
        bool operator!=(const PointData& P) const { return !(*this == P); }
};

 class HitboxData: public PointData
{
    private: 
        float c[2];
    
    public:
        HitboxData(float a, float b, float d=0, float e=0): PointData{a,b}, c{d, e} {}
        virtual ~HitboxData() {};

        float dx() const { return c[0]; }
        float dy() const { return c[1]; }
        void dx(float x) { c[0] = x > 0 ? x : 0; }
        void dy(float y) { c[1] = y > 0 ? y : 0; }

        PointData pointNW() const { return {p[0]       , p[1]       }; }
        PointData pointNE() const { return {p[0]+c[0]  , p[1]       }; }
        PointData pointSE() const { return {p[0]+c[0]  , p[1]+c[1]  }; }
        PointData pointSW() const { return {p[0]       , p[1]+c[1]  }; }
        PointData pointCC() const { return {p[0]+c[0]/2, p[1]+c[1]/2}; }
};

 class EntityData: public HitboxData
{
    private:
        float v[2];
    
    public:
        EntityData(float a, float b, float c=0, float d=0, float e=0, float f=0): HitboxData{a, b, e, f}, v{c, d} {}
        virtual ~EntityData() {};

        float vx() const { return v[0]; }
        float vy() const { return v[1]; }
        void vx(float x) { v[0] = x > 0 ? x : 0; }
        void vy(float y) { v[1] = y > 0 ? y : 0; }

        PointData leftProj() const  { return {x()-v[0]  , y()       }; }
        PointData rightProj() const { return {x()+v[0]  , y()       }; }
        PointData upProj() const    { return {x()       , y()-v[1]  }; }
        PointData downProj() const  { return {x()       , y()+v[1]  }; }
        
    //quali qualità posso ricavare da EntityData?   
};

 class Entity
{
    protected:
        ALLEGRO_BITMAP *image;
        EntityData data;
        unsigned lifes;

    public:
        Entity(float x, float y, float vx, float vy, ALLEGRO_BITMAP* p, unsigned l):
            data(x, y, vx, vy, al_get_bitmap_width(p)+4, al_get_bitmap_height(p)+4), image(p), lifes(l) {}

        virtual ~Entity() {};

        virtual void update(int argc, bool argf) = 0;

        const ALLEGRO_BITMAP *getBitmap() const { return image; }
        ALLEGRO_BITMAP *getBitmap()             { return image; }
         void setBitmap(ALLEGRO_BITMAP* p)       
        {
            image = p;
             if(p!=nullptr)
            {
                data.c[2] = al_get_bitmap_width(p)+4;
                data.c[3] = al_get_bitmap_height(p)+4;
            }
        }

        bool isAlive() const                    { return lifes; }
        void setAlive(bool k)                   { lifes = (k ? 1 : 0); } 
        
        //l'intero gruppo di setter e getter vanno ripensati
        const EntityData& getData() const       { return data; }
        float getCord_x() const                 { return data.c[0]+2; }
        float getCord_y() const                 { return data.c[1]+2; }
        float getVelocity_x() const             { return data.v[0]; }
        float getVelocity_y() const             { return data.v[1]; }

        void setCord_x(float cx)                { data.c[0] = cx-2; }
        void setCord_y(float cy)                { data.c[1] = cy-2; }
        void setVelocity_x(float vx)            { data.v[0] = vx; }
        void setVelocity_y(float vy)            { data.v[1] = vy; }
};

#endif