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

bool in_range(int first, int lower_bound, int upper_bound);
bool hitbox(int x1a, int y1a, int x1b, int y1b, int x2a, int y2a, int x2b, int y2b);

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
        Entity(float x=0, float y=0, float vx=0, float vy=0, ALLEGRO_BITMAP* p=nullptr, unsigned l=0):
            data(x, y, vx, vy, 0, 0), image(p), lifes(l) {}

        virtual ~Entity() {};
        virtual void update(int argc, bool argf) = 0;

        const ALLEGRO_BITMAP *getBitmap() const { return image; }
        ALLEGRO_BITMAP *getBitmap()             { return image; }
        void setBitmap(ALLEGRO_BITMAP* p)       { image = p; }

        bool isAlive() const { return lifes; }
        
        const EntityData& getData() const       { return data; }
        
        float getCord_x() const                 { return data.c[0]; }
        float getCord_y() const                 { return data.c[1]; }
        float getVelocity_x() const             { return data.v[0]; }
        float getVelocity_y() const             { return data.v[1]; }

        void setCord_x(float cx)                { data.c[0] = cx; }
        void setCord_y(float cy)                { data.c[1] = cy; }
        void setVelocity_x(float vx)            { data.v[0] = vx; }
        void setVelocity_y(float vy)            { data.v[1] = vy; }
};

#endif