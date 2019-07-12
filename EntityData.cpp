#include"EntityData.h"

 HitboxData EntityData::projection(unsigned i) const
{  
     return
    {   p[0] + projDir_x[i-1]*abs(v[0]),//*3.0F,
        p[1] + projDir_y[i-1]*abs(v[1]),//*3.0F,
        c[0], 
        c[1]
    };
} 
