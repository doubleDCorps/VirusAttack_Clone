#ifndef ENTITY_H_
#define ENTITY_H_

#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<array>
#include<iostream>
#include<utility>
#include<vector>
#include<list>
#include<cstdlib>
using namespace std;

typedef list<pair<int, int> > perimeter;
enum AXIS : int {none = 0, X = 1, Y = 2};
enum KEYS : int {still = 0, UP = 1, LEFT = 2, DOWN = 3, RIGHT = 4};

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

 struct HitboxData
{
    protected:
        ~HitboxData() {};
    public:
        float c[4];
        HitboxData(float a, float b, float d=0, float e=0): c{a, b, d, e} {}
        pair<float, float> getCord(){ return {c[0], c[1]}; }
        pair<float, float> getDim() { return {c[2], c[3]}; }
};

 struct EntityData: public HitboxData
{
    float v[2];
    EntityData(float a, float b, float c=0, float d=0, float e=0, float f=0): HitboxData(a, b, e, f), v{c, d} {}
    pair<float, float> getVel() { return {v[0], v[1]}; }
};

 class Entity
{
    private:
    protected:
        ALLEGRO_BITMAP *image;
        EntityData data;
        ~Entity() {};
    
    public:
        Entity(float x = 0, float y = 0, float vx = 0, float vy = 0, float wh = 0, float hh = 0, ALLEGRO_BITMAP* p = nullptr):
            data(x, y, vx, vy, wh, hh), image(p) {}

        const ALLEGRO_BITMAP *getBitmap() const { return image; }
        ALLEGRO_BITMAP *getBitmap()             { return image; }
        void setBitmap(ALLEGRO_BITMAP* p)       { image = p; }

        const EntityData& getData() const       { return data; }
        //void update() ? aggiorna le posizioni secondo l'AXIS ricevuto (none in questo caso indica X and Y; per eliminare Enemy)
        float getCord_x() const             { return data.c[0]; }
        float getCord_y() const             { return data.c[1]; }
        float getWidth_hitbox() const       { return data.c[2]; }
        float getHeight_hitbox() const      { return data.c[3]; }
        float getVelocity_x() const         { return data.v[0]; }
        float getVelocity_y() const         { return data.v[1]; }

        void setCord_x(float cx)            { data.c[0] = cx; }
        void setCord_y(float cy)            { data.c[1] = cy; }
        void setWidth_hitbox(float w)       { data.c[2] = w;  }
        void setHeight_hitbox(float h)      { data.c[3] = h;  }
        void setVelocity_x(float vx)        { data.v[0] = vx; }
        void setVelocity_y(float vy)        { data.v[1] = vy; }
};

#endif