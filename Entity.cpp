#include"Entity.h"
 
 inline bool in_range(int first, int lower_bound, int upper_bound)
{ 
    return first >= lower_bound && first <= upper_bound;
}
/*
    Date due hitbox quadrate, definite da quattro coordinate (due su x e due su y)
    hitbox() verifica se un punto o una parte di una hitbox ha compenetrato l'altra,
    ovvero se una parte delle coordinate di una hitbox è compreso fra la coordinata minore e maggiore 
    dell'altra hitbox (questo contemporaneamente su entrambi gli assi, x e y, per evitare
    che, ad esempio, oggetti con stesse coordinate x ma estremamente distanti lungo l'altro asse y
    siano erroneamente considerati come compenetrazione).
*/
 inline bool hitbox(int x1a, int y1a, int x1b, int y1b, int x2a, int y2a, int x2b, int y2b)
{
    return
        (in_range(x1a, min(x2a, x2b), max(x2a, x2b)) || in_range(x1b, min(x2a, x2b), max(x2a, x2b)) ||
         in_range(x2a, min(x1a, x1b), max(x1a, x1b)) || in_range(x2b, min(x1a, x1b), max(x1a, x1b)) )
     && (in_range(y1a, min(y2a, y2b), max(y2a, y2b)) || in_range(y1b, min(y2a, y2b), max(y2a, y2b)) ||
         in_range(y2a, min(y1a, y1b), max(y1a, y1b)) || in_range(y2b, min(y1a, y1b), max(y1a, y1b)) );
}