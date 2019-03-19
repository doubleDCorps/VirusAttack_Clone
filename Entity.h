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
using namespace std;

typedef list<pair<int, int> > perimeter;
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
 struct HitboxData
{        
        float c[4];
        HitboxData(float a, float b, float d=0, float e=0): c{a, b, d, e} {}
    protected:
        ~HitboxData() {};
};

 struct EntityData: public HitboxData
{
        float v[2];
        EntityData(float a, float b, float c=0, float d=0, float e=0, float f=0): HitboxData{a, b, e, f}, v{c, d} {}
};

 class Entity
{
    protected:
        ALLEGRO_BITMAP *image;
        EntityData data;
        unsigned lifes;

    public:
        Entity(float x, float y, float vx, float vy, ALLEGRO_BITMAP* p, unsigned l):
            data(x, y, vx, vy, al_get_bitmap_width(p)+8, al_get_bitmap_height(p)+8), image(p), lifes(l) {}

        virtual ~Entity() {};

        virtual void update(int argc, bool argf) = 0;    
        //virtual void move(int set) = 0;

        const ALLEGRO_BITMAP *getBitmap() const { return image; }
        ALLEGRO_BITMAP *getBitmap()             { return image; }
         void setBitmap(ALLEGRO_BITMAP* p)       
        {
            image = p;
            data.c[2] = al_get_bitmap_width(p)+8;
            data.c[3] = al_get_bitmap_height(p)+8;
        }

        bool isAlive() const  { return lifes; }
        void setAlive(bool k) { lifes = k ? 1 : 0; } 
        
        const EntityData& getData() const       { return data; }
        float getCord_x() const                 { return data.c[0]+4; }
        float getCord_y() const                 { return data.c[1]+4; }
        float getVelocity_x() const             { return data.v[0]; }
        float getVelocity_y() const             { return data.v[1]; }

        void setCord_x(float cx)                { data.c[0] = cx; }
        void setCord_y(float cy)                { data.c[1] = cy; }
        void setVelocity_x(float vx)            { data.v[0] = vx; }
        void setVelocity_y(float vy)            { data.v[1] = vy; }
};

#endif