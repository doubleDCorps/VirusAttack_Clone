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
        void setData(const EntityData& E)       { data = E; }
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