#ifndef POINTDATA_H
#define POINTDATA_H

#include"include.h"

 class PointData
{
    protected:
        float p[2];

    public:
        constexpr static float EPS{0.1F};
        PointData(): p{0.0F, 0.0F} {}
        PointData(float a, float b): p{a, b} {}
        PointData(const PointData& P): p{P.p[0], P.p[1]} {}
         PointData& operator=(const PointData& P)
        { 
             if(this != &P)
            { 
                this->p[0]=P.p[0];
                this->p[1]=P.p[1];
            } 
            return *this;
        }
        virtual ~PointData() {};

        float x() const { return p[0]; }
        float y() const { return p[1]; }
        void x(float x) { p[0] = x > 0 ? x : 0; } 
        void y(float y) { p[1] = y > 0 ? y : 0; }

        bool operator==(const PointData& P) const { return abs(p[0] - P.p[0])<=EPS and abs(p[1] - P.p[1])<=EPS; }
        bool operator!=(const PointData& P) const { return !(*this == P); } 
        /*
            Verifica se due punti sono collineari (la coordinata x oppure y è corrispondente).
            Due punti uguali risulteranno collineari.
        */
         AXIS collinear(const PointData&) const;
        /*
            Verifica se i tre punti si trovano tutti sulla stessa retta (ossia, sono reciprocamente collineari).
        */
         bool collinear(const PointData&, const PointData&) const;
        /*
            Calcola la distanza fra due punti.
        */
         float distance(const PointData&) const;
        /*
            Genera la proiezione del punto sul segmento con estremi i due parametri.
        */
         PointData projection(const PointData&, const PointData&) const;
};

#endif