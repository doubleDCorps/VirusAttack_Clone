#include"HitboxData.h"

 PointData HitboxData::point(unsigned i) const
{ 
    return 
        {p[0] + hitDir_x[i] * c[0],
         p[1] + hitDir_y[i] * c[1] };
}

 PointData HitboxData::center() const
{ 
    return
        {p[0] + c[0]/2.0F, 
         p[1] + c[1]/2.0F };
}

 bool HitboxData::collision(const HitboxData& D) const 
{
    return
        (in_range(p[0],          D.p[0], D.p[0]+D.c[0]) or
         in_range(p[0]+c[0],     D.p[0], D.p[0]+D.c[0]) or
         in_range(D.p[0],        p[0],   p[0]+c[0]    ) or 
         in_range(D.p[0]+D.c[0], p[0],   p[0]+c[0]    ))
        and
        (in_range(p[1],          D.p[1], D.p[1]+D.c[1]) or 
         in_range(p[1]+c[1],     D.p[1], D.p[1]+D.c[1]) or 
         in_range(D.p[1],        p[1],   p[1]+c[1]    ) or 
         in_range(D.p[1]+D.c[1], p[1],   p[1]+c[1]    ));
}