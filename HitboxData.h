#ifndef HITBOXDATA_H
#define HITBOXDATA_H

#include"PointData.h"

 class HitboxData: public PointData
{
    protected:
        constexpr static int hitDir_x[]{0, 1, 1, 0};
        constexpr static int hitDir_y[]{0, 0, 1, 1};

        float c[2];
    
    public:
        HitboxData(float a, float b, float d=0, float e=0): PointData{a, b}, c{d, e} {}
        HitboxData(const HitboxData& H): PointData(H), c{H.c[0], H.c[1]} {}
        HitboxData(const PointData& P, float dx=4, float dy=4): PointData({P.x()-dx/2, P.y()-dy/2}), c{dx, dy} {}
        //HitboxData(const PointData& P): PointData(P), c{2, 2} {}
         HitboxData& operator=(const HitboxData& H)
        { 
             if(this!=&H)
            {
                PointData::operator=(H);
                this->c[0]=H.c[0];
                this->c[1]=H.c[1]; 
            } 
            
            return *this; 
        }
        virtual ~HitboxData() {};

        float dx() const { return c[0]; }
        float dy() const { return c[1]; }
        void dx(float x) { c[0] = x; }
        void dy(float y) { c[1] = y; }
        /*
            Genera uno degli estremi della hitbox in base alla variabile presa in input.
        */
        PointData point(unsigned) const;
        /*
            Restituisce il punto che si trova al centro della hitbox.
        */
        PointData center() const;
        /*
            Verifica la presenza di collisioni fra l'oggetto e l'Hitbox D utilizzando un algoritmo "bounding box".
        */
        bool collision(const HitboxData&) const;
};

#endif