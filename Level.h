#ifndef POLY_HPP_
#define POLY_HPP_
#include"Entities.h"
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
        GameList(const perimeter& p = {}) { for(auto& i : p) push(i.first, i.second); }
        bool push(int x, int y);
        bool is_adj(int x, int y, int w=0, int h=0) const;
        AXIS hits(int x1, int y1, int w1=0, int h1=0, int vx=0, int vy=0) const;
        bool inside(int x, int y, int w=0, int h=0) const;
        void print(ALLEGRO_BITMAP* buffer=nullptr) const;

    private:
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
        GameArea(const perimeter& b, const Entity* P=nullptr, const Entity* B=nullptr)
            : border(b), trace(), Boss(B), Player(P) {}     

        inline AXIS hitsBorder(const EntityData& D) const       { return border.hits(D.c[0], D.c[1], D.c[2], D.c[3], D.v[0], D.v[1]); }
        inline bool insideBorder(const HitboxData& D) const     { return border.inside(D.c[0], D.c[1], D.c[2], D.c[3]); }
        inline void printBorder(ALLEGRO_BITMAP* buffer=nullptr) const   { border.print(buffer); }

        inline AXIS hitsTrace(const EntityData& D) const        { return trace.hits(D.c[0], D.c[1], D.c[2], D.c[3], D.v[0], D.v[1]); }
        inline bool insideTrace(const HitboxData& D) const      { return trace.inside(D.c[0], D.c[1], D.c[2], D.c[3]); }
        inline void printTrace(ALLEGRO_BITMAP* buffer=nullptr) const    { trace.print(buffer); }
        
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