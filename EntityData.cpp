#include"EntityData.h"

 HitboxData EntityData::projection(int d1, int d2) const
{
    int nd1 = d1 > 0 ? 1 : d1 < 0 ? -1 : 0; 
    int nd2 = d2 > 0 ? 1 : d2 < 0 ? -1 : 0;

     return
    {   p[0] + nd1*abs(v[0]),//*3.0F,
        p[1] + nd2*abs(v[1]),//*3.0F,
        c[0], 
        c[1]
    };
}

 HitboxData EntityData::projection(unsigned i) const
{  
    return projection(projDir_x[i-1], projDir_y[i-1]);
} 

 HitboxData EntityData::projection() const
{
    int d1 = v[0] > 0 ? 1 : v[0] < 0 ? -1 : 0;
    int d2 = v[1] > 0 ? 1 : v[1] < 0 ? -1 : 0;

    return projection(d1, d2);
} 