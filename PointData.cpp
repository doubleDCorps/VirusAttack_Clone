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
        return p[1] - P.p[1];
     else if(condition == Y)
        return p[0] - P.p[0];

    return 0.0F;
}

 PointData PointData::projection(const PointData& P1, const PointData& P2) const 
{
    float nx, ny;
    
    if(p[0] >= min(P1.p[0], P2.p[0]) and p[0] <= max(P1.p[0], P2.p[0]))  
        nx = p[0];
    else                                                                
        nx = P1.p[0];
    
    if(p[1] >= min(P1.p[1], P2.p[1]) and p[1] <= max(P1.p[1], P2.p[1]))  
        ny = p[1];
    else                                                                
        ny = P1.p[1];

    return {nx, ny};
}

 bool PointData::on_segment(const PointData& p, const PointData& r) const 
{ 
    return 
        x() < max(p.x(), r.x()) and x() > min(p.x(), r.x())
    and y() < max(p.y(), r.y()) and y() > min(p.y(), r.y());
} 

 int orientation(const PointData& p, const PointData& q, const PointData& r) 
{ 
    int val = 
        (q.y() - p.y())*(r.x() - q.x()) - 
        (q.x() - p.x())*(r.y() - q.y()); 
  
    if(val == 0) return 0;
  
    return (val > 0) ? 1 : 2; 
}

 bool doIntersect(const pair<PointData, PointData>& l1, const pair<PointData, PointData>& l2)
{ 
    const PointData& p1 = l1.first;
    const PointData& q1 = l1.second;
    const PointData& p2 = l2.first;
    const PointData& q2 = l2.second;
    
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2); 
    int o3 = orientation(p2, q2, p1); 
    int o4 = orientation(p2, q2, q1); 
 
    if(o1 != o2 and o3 != o4)
        return true;
  
    if(o1 == 0 and p2.on_segment(p1, q1))
        return true; 
    if(o2 == 0 and p2.on_segment(p1, q1))
        return true; 
    if(o3 == 0 and p1.on_segment(p2, q2))
        return true; 
    if(o4 == 0 and q1.on_segment(p2, q2))
        return true; 
  
    return false;
}