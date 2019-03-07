/*
Entity -> Player -> Enemy -> Boss -> Minion -> Polygon -> main
                              v         v
                             ...       ...
*/
#ifndef POLY_HPP_
#define POLY_HPP_
#include"Enemy.h"
    
inline bool in_range(int first, int lower_bound, int upper_bound) { return first>=lower_bound && first<=upper_bound; }
inline bool hitbox(int x1a, int y1a, int x1b, int y1b, int x2a, int y2a, int x2b, int y2b);
/*
    GameList è una lista di coppie di coordinate circolare e ordinata secondo il seguente criterio:
    per ogni coppia di elementi a1 e a2 consecutivi, la coppia precedente a0 e a1 sarà tale che
    se una coordinata è in comune fra a0 e a1, allora la coordinata opposta sarà in comune fra a1 e a2.
    Poichè la lista è circolare, ciò può essere valido anche per la coppia aN a0 (ultimo elemento - primo elemento).
    Inoltre GameList contiene un puntatore alla bitmap, utilizzata per rappresentare graficamente i segmenti
    composti da ogni coppia di elementi della lista.
*/
 class GameArea;

 class GameList: private perimeter
{
    friend GameArea;

    public:
        GameList(ALLEGRO_BITMAP* m = nullptr, const perimeter& p = {}): pic(m) { for(auto& i : p) push(i.first, i.second); }
        bool push(int x, int y);
        bool is_adj(int x, int y, int w=0, int h=0) const;
        AXIS hits(int x1, int y1, int w1=0, int h1=0, int vx=0, int vy=0) const;
        bool inside(int x, int y, int w=0, int h=0) const;
        //void print(ALLEGRO_BITMAP* buffer) const; // da implementare
        const ALLEGRO_BITMAP* getPic() const    { return pic; }
        void setPic(const ALLEGRO_BITMAP* p)    { pic = p; }
        
    private:
        const ALLEGRO_BITMAP* pic;
        inline auto successor(perimeter::iterator it)                       { return ++it == end() ? begin() : it; }
        inline auto successor(perimeter::const_iterator it) const           { return ++it == cend() ? cbegin() : it; }
        inline auto successor(perimeter::reverse_iterator it)               { return ++it == rend() ? rbegin() : it; }
        inline auto successor(perimeter::const_reverse_iterator it) const   { return ++it == crend() ? crbegin() : it; }
};
/*
    GameArea rappresenta l'area di gioco, formata da:
    -un poligono rappresentante il bordo dell'area di gioco (border);
    -una linea spezzata rappresentante il tracciato del giocatore (trace);
    -un puntatore costante al giocatore (Player);
    -un puntatore costante al boss di livello (Boss);
    Essa gestisce tutte le routine riguardante la collisione col bordo, la posizione rispetto all'area di gioco
    (esterno/interno), l'aggiornamento dei bordi dell'area di gioco durante la partita, e il calcolo dell'area.
*/
 class GameArea
{
    public:
        GameArea(): border(), trace(), Boss(nullptr), Player(nullptr) {}
        GameArea(const perimeter& b, ALLEGRO_BITMAP* m=nullptr, ALLEGRO_BITMAP* n=nullptr, const Entity* B=nullptr, const Entity* P=nullptr)
            : border(m, b), trace(n), Boss(B), Player(P) {}     

        inline AXIS hitsBorder(const EntityData& D) const       { return border.hits(D.c[0], D.c[1], D.c[2], D.c[3], D.v[0], D.v[1]); }
        inline bool insideBorder(const HitboxData& D) const     { return border.inside(D.c[0], D.c[1], D.c[2], D.c[3]); }
        //inline void printBorder(ALLEGRO_BITMAP* buffer) const   { border.print(buffer); }  
        inline AXIS hitsTrace(const EntityData& D) const        { return trace.hits(D.c[0], D.c[1], D.c[2], D.c[3], D.v[0], D.v[1]); }
        inline bool insideTrace(const HitboxData& D) const      { return trace.inside(D.c[0], D.c[1], D.c[2], D.c[3]); }
        //inline void printTrace(ALLEGRO_BITMAP* buffer) const    { trace.print(buffer); }
        bool update();
        void clear() { trace.clear(); }
        int getArea() const;
    
    private:
        GameList border;
        GameList trace;
        const Entity* Boss;
        const Entity* Player;
};

#endif