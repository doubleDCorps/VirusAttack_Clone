#include "polygon.h"
/*
    Date due hitbox quadrate, definite da quattro coordinate (due su x e due su y)
    hitbox() verifica se un punto o una parte di una hitbox ha compenetrato l'altra,
    ovvero se una parte delle coordinate di una hitbox è compreso fra la coordinata minore e maggiore 
    dell'altra hitbox (questo contemporaneamente su entrambi gli assi, x e y, per evitare
    che, ad esempio, oggetti con stesse coordinate x ma estremamente distanti lungo l'altro asse y
    siano erroneamente considerati come compenetrazione).
*/
 bool hitbox(int x1a, int y1a, int x1b, int y1b, int x2a, int y2a, int x2b, int y2b)
{
    return
        (in_range(x1a, min(x2a, x2b), max(x2a, x2b)) || in_range(x1b, min(x2a, x2b), max(x2a, x2b)) ||
         in_range(x2a, min(x1a, x1b), max(x1a, x1b)) || in_range(x2b, min(x1a, x1b), max(x1a, x1b)) )
     && (in_range(y1a, min(y2a, y2b), max(y2a, y2b)) || in_range(y1b, min(y2a, y2b), max(y2a, y2b)) ||
         in_range(y2a, min(y1a, y1b), max(y1a, y1b)) || in_range(y2b, min(y1a, y1b), max(y1a, y1b)) );
}
/*
    Se il punto p rispetta le condizioni di ordinamento, viene aggiunto alla fine o all'inizio
    della lista e viene restituito true; altrimenti viene restituito false.
*/
 bool GameList::push(int x, int y)
{
    if(empty()) push_back({x, y});
    else if(size()== 1 && (x == back().first || y == back().second) )                    push_back({x, y});
    else if(size()== 1 && (x == front().first || y == front().second) )                  push_front({x, y});
    else if(size() >= 2 && x == back().first && back().second == (++rbegin())->second)   push_back({x, y});
    else if(size() >= 2 && x == back().first && back().first == (++rbegin())->first)     back() = {x, y};
    else if(size() >= 2 && y == back().second && back().first == (++rbegin())->first)    push_back({x, y});
    else if(size() >= 2 && y == back().second && back().second == (++rbegin())->second)  back() = {x, y};
    else if(size() >= 2 && x == front().first && front().second == (++begin())->second)  push_front({x, y});
    else if(size() >= 2 && x == front().first && front().first == (++begin())->first)    front() = {x, y};
    else if(size() >= 2 && y == front().second && front().first == (++begin())->first)   push_front({x, y});
    else if(size() >= 2 && y == front().second && front().second == (++begin())->second) front() = {x, y};
    else        return false;
    return true;
}
/*
    Data una coppia di coordinate p, verifica se esiste una coppia di elementi della lista tali che il punto ricada nel
    segmento che li connette. 
*/
 bool GameList::is_adj(int x, int y, int w, int h) const
{
    for(auto it{ begin() }; it!=end(); ++it)
     if(hitbox(x, y, x+w, y+h, it->first, it->second, successor(it)->first, successor(it)->second) )
        return true;
    return false;
}
/*
    Per un dato oggetto definito dalla sua hitbox (primi quattro parametri) e dalla sua
    velocità (ultimi due parametri) viene verificata per ogni coppia di punti nella lista
    (ciascuna rappresentante un segmento) se si verifica una collisione fra la hitbox e il
    segmento; se la collisione avviene, si restituisce una variabile che riconosce lungo quale
    asse è avvenuto la collisione (X o Y), altrimenti lo stato "none" (euqivalente a 0).
*/
 AXIS GameList::hits(int x1, int y1, int w1, int h1, int vx, int vy) const
{
    if(empty()) return none;

     for(auto it{ begin() }; it != end(); ++it)
    {
        const int& x2{ it->first }, x3{ successor(it)->first };
        const int& y2{ it->second }, y3{ successor(it)->second };

        if(x2!=x3 && (vy>=0 && y2-y1 > -5 || vy<=0 && y2-y1 < 5) && hitbox(x1, y1, x1+w1, y1+h1, x2, y2, x3, y2+20) )
            return Y;
        
        if(y2!=y3 && (vx>=0 && x2-x1 > -5 || vx<=0 && x2-x1 < 5) && hitbox(x1, y1, x1+w1, y1+h1, x2, y2, x2+20, y3) )
            return X;
    }
    
    return none;
}
/*
    Data una hitbox associata ad un oggetto, si applica lo stesso algoritmo
    usato per il calcolo dell'area per decidere se l'oggetto si trova all'esterno dell'area definita 
    dalla lista: si verifica la presenza o meno di collisioni fra la hitbox e l'area descritta da ogni
    coppia di punti e per quelle in cui la collisione avviene si considera adeguatamente se l'area
    possiede segno "negativo" o "positivo", associando le aree positive a quelle interne al poligono
    e quelle negative alle aree esterne al poligono.
*/
 bool GameList::inside(int x, int y, int w, int h) const
{
    if(empty()) return true;

    int cont{};

     for(auto it{ begin() }; it != end(); ++it)
    {
        int y1{ it->second };
        int y2{ successor(it)->second };

        if(y2-y1!=0)
         if(hitbox(x, y, x+w, y+h, 0, min(y1, y2), it->first, max(y1, y2) ) )
        {
            if(y2-y1 < 0) --cont;
            if(y2-y1 > 0) ++cont;
        }
    }

    return cont <= 0 ? false : true;
}
/*
    Dato un bitmap da trattare come buffer temporaneo, viene targettato per il disegno
    e viene poi successivamente disegnato il poligono rappresentato dalla lista manipolando
    l'immagine che fa da "font" per l'intero bordo.
*//*
 void GameList::print(ALLEGRO_BITMAP* buffer) const
{
    if(empty()) return;

     if(pic!=nullptr && buffer!=nullptr)
    {
        //if(al_get_target_bitmap()!=buffer)  al_set_target_bitmap(buffer);
        //drawing operations
    }
}*/



/*
    Viene calcolata l'area utilizzando il seguente algoritmo:
    dato un poligono chiuso definito da una lista di coordinate (x, y) concatenate,
    dove con concatenate si intende che almeno un elemento di ogni coppia è uguale a quello
    del successivo e che ogni coppia ha l'elemento opposto in comune rispetto alla precedente,
    si sommano le aree rettangolari ottenute da ogni coppia di coordinate concantenate, sfruttando
    la differenza fra le y per determinare il "segno" che indichi se l'area appartiene
    alla figura descritta (positiva) o altrimenti (negatva).
    La somma viene poi dimezzata per ottenere il valore numerico corrispondente all'area del poligono.
*/
 int GameArea::getArea() const
{
    if(border.empty()) return -1;

    int area{};
    for(auto it{ border.begin() }; it!=border.end(); ++it)
        area+=(it->first+border.successor(it)->first)*(border.successor(it)->second-it->second);
    area/=2;

    return area;
}
/*
    Aggiorna automaticamente la traccia all'ultima posizione nota del giocatore;
    se l'ultimo elemento della traccia è allineato con border, si aggiorna la lista border
    in base alla posizione del Boss.
*/
 bool GameArea::update()
{
    if(trace.size() > 1)
     if(trace.push(Player->getCord_x(), Player->getCord_y()) && border.is_adj(trace.back().first, trace.back().second, 10, 10) )
    {
        if(trace.inside(Boss->getCord_x(), Boss->getCord_y()) )
            for(auto i : border)
             if(trace.inside(i.first, i.second))  trace.push(i.first, i.second);
        else
            for(auto i : border)
             if(!trace.inside(i.first, i.second)) trace.push(i.first, i.second);

        border=trace;
        trace.clear();
        return true;
    }

    return false;
}