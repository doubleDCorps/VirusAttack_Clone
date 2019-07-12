#ifndef ENTITY_H_
#define ENTITY_H_

#include"GameList.h"

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