#include"PointData.h"

 AXIS PointData::collinear(const PointData& P) const  
{ 
    bool bx = abs(p[0] - P.p[0])<=EPS;
    bool by = abs(p[1] - P.p[1])<=EPS;
            
    return
        bx and by ? XY :
        bx        ? X  : 
               by ? Y  : 
                    none;
}

 bool PointData::collinear(const PointData& P1, const PointData& P2) const
{
    return
        collinear(P1) != none and
        collinear(P2) != none and 
        P1.collinear(P2) != none;
        //(r1 == XY or r2 == XY or r1 == r2);
}

 float PointData::distance(const PointData& P) const
{
    AXIS condition = collinear(P);

     if(condition == none)
    {
        float x = p[0] - P.p[0];
        float y = p[1] - P.p[1];

        return sqrt(x*x + y*y);
    }
     else if(condition == X)
    {
        return p[1] - P.p[1];
    }
     else if(condition == Y)
    {
        return p[0] - P.p[0];
    }

    return 0.0F;
}

 PointData PointData::projection(const PointData& P1, const PointData& P2) const 
{
    float nx, ny;
    
    if(p[0] >= min(P1.p[0], P2.p[0]) && p[0] <= max(P1.p[0], P2.p[0]))  nx = p[0];
    else                                                                nx = P1.p[0];
    
    if(p[1] >= min(P1.p[1], P2.p[1]) && p[1] <= max(P1.p[1], P2.p[1]))  ny = p[1];
    else                                                                ny = P1.p[1];

    return {nx, ny};
/*
    //DA RIFARE (non funziona come inteso)

    float vx = P1.p[0] - P2.p[0];
    float vy = P1.p[1] - P2.p[1];
    float wx = p[0] - P2.p[0];
    float wy = p[1] - P2.p[1];

    float k = (wx*vx + wy*vy)/(vx*vx + vy*vy);

    vx = k*vx + P2.p[0];
    vy = k*vy + P2.p[1];

    return {in_range(vx, min(P1.p[0], P2.p[0]), max(P1.p[0], P2.p[0])) ? vx : P2.p[0],
            in_range(vy, min(P1.p[1], P2.p[1]), max(P1.p[1], P2.p[1])) ? vy : P2.p[1]};
 */
}