#ifndef ENTITYDATA_H
#define ENTITYDATA_H

#include"HitboxData.h"

 struct EntityData: public HitboxData
{
    private:
        constexpr static int projDir_x[]{ 0,-1, 0, 1};
        constexpr static int projDir_y[]{-1, 0, 1, 0};
        float v[2];
    
    public:
        EntityData(float a, float b, float c=0, float d=0, float e=0, float f=0): HitboxData{a, b, e, f}, v{c, d} {}
        EntityData(const EntityData& E): HitboxData(E), v{E.v[0], E.v[1]} {}
        explicit EntityData(const HitboxData& H, float vx=0, float vy=0): HitboxData(H), v{vx, vy} {}
        explicit EntityData(const PointData& P, float dx=2, float dy=2, float vx=0, float vy=0): HitboxData(P, dx, dy), v{vx, vy} {}
        //EntityData(const HitboxData& H): HitboxData(H), v{0, 0} {}
        //EntityData(const PointData& P): HitboxData(P), v{0, 0} {}
         EntityData& operator=(const EntityData& E)
        { 
             if(this!=&E)
            {
                HitboxData::operator=(E);
                this->c[0]=E.c[0];
                this->c[1]=E.c[1]; 
            } 
            
            return *this; 
        }
        virtual ~EntityData() {};

        float vx() const { return v[0]; }
        float vy() const { return v[1]; }
        void vx(float x) { v[0] = x; }
        void vy(float y) { v[1] = y; }
        /*
            Genera una hitbox corrispondente all'oggetto traslato lungo la direzione scelta.
        */
        HitboxData projection(int, int) const;
        HitboxData projection(unsigned) const;
        HitboxData projection() const;
};

#endif