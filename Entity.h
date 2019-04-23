#ifndef ENTITY_H_
#define ENTITY_H_

#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include<array>
#include<iostream>
#include<utility>
#include<vector>
#include<list>
#include<cstdlib>
#include<cmath>
#include<map>
#include<algorithm>
#include<cassert>
#include<climits>
using namespace std;

enum AXIS : int {none=0, X=1, Y=2, XY=3};
enum KEYS : int {still=0, UP=1, LEFT=2, DOWN=3, RIGHT=4, SPACE=5};
enum DIRS : int {NW=0, NE=1, SE=2, SW=3};

 inline bool in_range(int first, int lower_bound, int upper_bound) { return first >= lower_bound and first <= upper_bound; } 
/*
    Due strutture dati elementari che definiscono i seguenti dati:
        HitboxData: coordinata x, coordinata y, larghezza, altezza
        EntityData: coordinata x, coordinata y, larghezza, altezza, velocità x, velocità y
    Il polimorfismo viene sfruttato per rendere più lineari alcune chiamate a funzione nella classe Level.
*/
 class PointData
{
    protected:
        float p[2];

    public:
        constexpr static float EPS{0.1F};
        PointData(): p{0.0F, 0.0F} {}
        PointData(float a, float b): p{a, b} {}
        PointData(const PointData& P): p{P.p[0], P.p[1]} {}
         PointData& operator=(const PointData& P)
        { 
             if(this != &P)
            { 
                this->p[0]=P.p[0];
                this->p[1]=P.p[1];
            } 
            return *this;
        }
        virtual ~PointData() {};

        float x() const { return p[0]; }
        float y() const { return p[1]; }
        void x(float x) { p[0] = x > 0 ? x : 0; } 
        void y(float y) { p[1] = y > 0 ? y : 0; }

        bool operator==(const PointData& P) const { return fabs(p[0] - P.p[0])<=EPS and fabs(p[1] - P.p[1])<=EPS; }
        bool operator!=(const PointData& P) const { return !(*this == P); } 
        /*
            Verifica se due punti sono collineari (la coordinata x oppure y è corrispondente).
            Due punti uguali risulteranno collineari.
        */
         AXIS collinear(const PointData&) const;
        /*
            Verifica se i tre punti si trovano tutti sulla stessa retta (ossia, sono reciprocamente collineari).
        */
         bool collinear(const PointData&, const PointData&) const;
        /*
            Genera la proiezione del punto sul segmento con estremi i due parametri.
        */
         PointData projection(const PointData&, const PointData&) const;
};

 class HitboxData: public PointData
{
    protected:
        constexpr static int hitDir_x[]{0, 1, 1, 0};
        constexpr static int hitDir_y[]{0, 0, 1, 1};

        float c[2];
    
    public:
        HitboxData(float a, float b, float d=0, float e=0): PointData{a, b}, c{d, e} {}
        HitboxData(const HitboxData& H): PointData(H), c{H.c[0], H.c[1]} {}
        HitboxData(const PointData& P, float dx=4, float dy=4): PointData({P.x()-dx/2, P.y()-dy/2}), c{dx, dy} {}
        //HitboxData(const PointData& P): PointData(P), c{2, 2} {}
         HitboxData& operator=(const HitboxData& H)
        { 
             if(this!=&H)
            {
                PointData::operator=(H);
                this->c[0]=H.c[0];
                this->c[1]=H.c[1]; 
            } 
            
            return *this; 
        }
        virtual ~HitboxData() {};

        float dx() const { return c[0]; }
        float dy() const { return c[1]; }
        void dx(float x) { c[0] = x; }
        void dy(float y) { c[1] = y; }
        /*
            Genera uno degli estremi della hitbox in base alla variabile presa in input.
        */
        PointData point(unsigned) const;
        /*
            Restituisce il punto che si trova al centro della hitbox.
        */
        PointData center() const;
        /*
            Verifica la presenza di collisioni fra l'oggetto e l'Hitbox D utilizzando un algoritmo "bounding box".
        */
        bool collision(const HitboxData&) const;
};

 struct EntityData: public HitboxData
{
    private:
        constexpr static int projDir_x[]{0 ,-1, 0, 1};
        constexpr static int projDir_y[]{-1, 0, 1, 0};

        float v[2];
    
    public:
        EntityData(float a, float b, float c=0, float d=0, float e=0, float f=0): HitboxData{a, b, e, f}, v{c, d} {}
        EntityData(const EntityData& E): HitboxData(E), v{E.v[0], E.v[1]} {}
        explicit EntityData(const HitboxData& H, float vx=0, float vy=0): HitboxData(H), v{vx, vy} {}
        explicit EntityData(const PointData& P, float dx=2, float dy=2, float vx=0, float vy=0): HitboxData(P, dx, dy), v{vx, vy} {}
        //EntityData(const HitboxData& H): HitboxData(H), v{0, 0} {}
        //EntityData(const PointData& P): HitboxData(P), v{0, 0} {}
         EntityData& operator=(const EntityData& E)
        { 
             if(this!=&E)
            {
                HitboxData::operator=(E);
                this->c[0]=E.c[0];
                this->c[1]=E.c[1]; 
            } 
            
            return *this; 
        }
        virtual ~EntityData() {};

        float vx() const { return v[0]; }
        float vy() const { return v[1]; }
        void vx(float x) { v[0] = x; }
        void vy(float y) { v[1] = y; }
        /*
            Genera una hitbox corrispondente all'oggetto traslato lungo la direzione scelta.
        */
        HitboxData projection(unsigned) const;
};

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
        /*
            Restituisce il punto successivo, secondo il critero di chiusura della lista.
        */
        inline auto successor(perimeter::const_iterator it) const           { return ++it == cend() ? cbegin() : it; }
        /*
            Restituisce il punto successivo, secondo il critero di chiusura della lista.
        */
        inline auto successor(perimeter::reverse_iterator it)               { return ++it == rend() ? rbegin() : it; }
        /*
            Restituisce il punto successivo, secondo il critero di chiusura della lista.
        */
        inline auto successor(perimeter::const_reverse_iterator it) const   { return ++it == crend() ? crbegin() : it; }
};

 class Entity
{
    protected:
        ALLEGRO_BITMAP *image;
        EntityData data;
        unsigned lifes;

    public:
        Entity(float x, float y, float vx, float vy, ALLEGRO_BITMAP* p, unsigned l): data(x, y, vx, vy, al_get_bitmap_width(p)+4, al_get_bitmap_height(p)+4), image(p), lifes(l) {}
        virtual ~Entity() {};

        virtual void update(const GameList&) = 0;
        /*
            Getter della bitmap
            ERRORE: ce ne sono due? 
        */
        const ALLEGRO_BITMAP *getBitmap() const { return image; }
        /*
            Getter della bitmap
            ERRORE: ce ne sono due? 
        */
        ALLEGRO_BITMAP *getBitmap()             { return image; }
        /*
            Setta la bitmap dell'entità a quella ricevuta (aggiornando i dati coerentemente).
        */
         void setBitmap(ALLEGRO_BITMAP* p)       
        {
            image = p;
             if(p!=nullptr)
            {
                data.dx( al_get_bitmap_width(p)+4 );
                data.dy( al_get_bitmap_height(p)+4 );
            }
        }
        /*
            Verifica se l'entità è ancora in vita (attiva).
        */
        unsigned isAlive() const                { return lifes; }
        /*
            Imposta il numero di vite al valore restituito.
            ERRORE: il Player mette un limite più stringente
        */
        void setAlive(unsigned k)               { lifes = k; } 
        /*
            Restituisce l'EntityData contenente le informazioni sull'entità.
        */
        const EntityData& getData() const       { return data; }
        /*
            ERRORE: l'intero gruppo di setter e getter vanno ripensati
        */
        float getCord_x() const                 { return data.x()+2; }
        /*
            ERRORE: l'intero gruppo di setter e getter vanno ripensati
        */
        float getCord_y() const                 { return data.y()+2; }
        /*
            ERRORE: l'intero gruppo di setter e getter vanno ripensati
        */
        float getVelocity_x() const             { return data.vx(); }
        /*
            ERRORE: l'intero gruppo di setter e getter vanno ripensati
        */
        float getVelocity_y() const             { return data.vy(); }
        /*
            setter della coordinata x dell'entità
        */
        void setCord_x(float cx)                { data.x(cx-2); }
        /*
            setter della coordinata y dell'entità
        */
        void setCord_y(float cy)                { data.y(cy-2); }
        /*
            ERRORE: nella classe Player le velocità devono sempre essere le stesse
        */
        void setVelocity_x(float vx)            { data.vx(vx); }
        /*
            ERRORE: nella classe Player le velocità devono sempre essere le stesse
        */
        void setVelocity_y(float vy)            { data.vy(vy); }
};

#endif