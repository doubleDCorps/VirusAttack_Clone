#ifndef GAMELIST_H
#define GAMELIST_H

#include"EntityData.h"

/*
    GameList è una lista di coppie di coordinate circolare e ordinata secondo il seguente criterio:
    per ogni coppia di elementi a1 e a2 consecutivi, la coppia precedente a0 e a1 sarà tale che
    se una coordinata è in comune fra a0 e a1, allora la coordinata opposta sarà in comune fra a1 e a2.
    Poichè la lista è circolare, ciò può essere valido anche per la coppia aN a0 (ultimo elemento - primo elemento).
    Inoltre GameList contiene un puntatore alla bitmap, utilizzata per rappresentare graficamente i segmenti
    composti da ogni coppia di elementi della lista.
*/
 class Level;
 typedef list<PointData> perimeter;

 class GameList: private perimeter
{
    friend Level;
    friend ostream& operator<<(ostream&, const GameList&);

    public:
        GameList(): thickness(1), color(al_map_rgb(255, 255, 255)) {}
        GameList(const perimeter& p, float t = 1, ALLEGRO_COLOR c = al_map_rgb(255, 255, 255)): thickness(t), color(c) { for(auto& i : p) pushPoint(i); }
        GameList(const GameList& GL): perimeter(GL), thickness(GL.thickness), color(GL.color) {}
         GameList& operator=(const GameList& GL)
        {
             if(this != &GL)
            {
                perimeter::operator=(GL);
                thickness = GL.thickness;
                color = GL.color;
            }

            return *this;
        }
        //PointData front() const { return perimeter::front(); }
        //void clear() { perimeter::clear(); }
        //bool empty() const { return perimeter::empty(); }
        
        /*
            Aggiungi un punto sul tracciato, se soddisfa le condizioni.
        */        
        bool pushPoint(const PointData&);
        /*
            Verifica se il punto si trova sulla lista.
        */
        pair<AXIS, bool> onEdge(const HitboxData&) const;
        /*
            Se la linea è chiusa, verifica se l'hitbox si trova al suo interno;
            Se la liena è aperta, verifica se l'hitbox si trova a sinistra della linea.
        */
        bool inArea(const HitboxData&) const;

        /*
            Restituisce il punto più vicino a quello ricevuto in input.
        */
        PointData nearestPoint(const PointData&) const;
        
        /*
            Restituisce la coppia di estremi del segmento più vicino al punto dato.
        */
        pair<PointData, PointData> nearestLine(const PointData&) const;
        /*
            Utilizza onEdge per verificare che la collisione sia coerente con la direzione
            di movimento dell'Entity.
        */
        AXIS collides(const EntityData&) const;
        /*
            Verifica se l'hitbox si trova strettamente dentro l'area della linea
            (ovvero: verifica se l'hitbox sia interna all'area senza toccare il bordo).
        */
        bool o_inside(const HitboxData&) const;
        /*
            Verifica se l'hitbox si trovi dentro l'area (bordo incluso).
        */
        bool c_inside(const HitboxData&) const;
        /*
            Verifica che il tracciato sia chiuso.
        */
        bool is_closed() const;
        /*
            Stampa l'oggetto senza l'ultimo tratto (la chiusura).
        */
        void u_print(ALLEGRO_BITMAP* buffer=nullptr) const;
        /*
            Stampa l'oggetto (incluso l'ultimo tratto).
        */
        void l_print(ALLEGRO_BITMAP* buffer=nullptr) const;

    private:
        float thickness;
        ALLEGRO_COLOR color;
        /*
            Restituisce il punto successivo, secondo il critero di chiusura della lista.
        */
        inline auto successor(perimeter::iterator it)                       { return ++it == end() ? begin() : it; }
        inline auto successor(perimeter::const_iterator it) const           { return ++it == cend() ? cbegin() : it; }
        inline auto successor(perimeter::reverse_iterator it)               { return ++it == rend() ? rbegin() : it; }
        inline auto successor(perimeter::const_reverse_iterator it) const   { return ++it == crend() ? crbegin() : it; }
        /*
            Restituisce il punto precedente, secondo il critero di chiusura della lista.
        */
        inline auto predecessor(perimeter::iterator it)                     { return it == begin() ? (++rbegin()).base() : --it; }
        inline auto predecessor(perimeter::const_iterator it) const         { return it == cbegin() ? (++crbegin()).base() : --it; }
        inline auto predecessor(perimeter::reverse_iterator it)             { return it == rbegin() ? make_reverse_iterator(++begin()) : --it; }
        inline auto predecessor(perimeter::const_reverse_iterator it) const { return it == crbegin() ? make_reverse_iterator(++cbegin()) : --it; }
};

#endif