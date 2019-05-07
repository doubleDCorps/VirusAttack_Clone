#ifndef POLY_H_
#define POLY_H_
#include"Entities.h"

 class Level
{
    public:
        Level(unsigned difficulty, ALLEGRO_DISPLAY_MODE settings, ALLEGRO_TIMER* tim);
        /*
            Loop di gioco (escuzione effettiva del livello).
        */
        void loop();
    
    private:
        /*
            Bordo di default per livello (dipende dai settaggi del display).
        */
        inline static perimeter defPerimeter{};
        /*
            Setting della difficoltà: influenza il numero di boss, la velocità dei nemici, e altro.
        */
        unsigned difficulty;
        ALLEGRO_TIMER* timer;
        unsigned numBosses;
        /*
            Parametri usati in loop.
        */
        GameList border;
        GameList trace;
        vector<Entity*> entities;
        Player* player;
        /*
            Inizializza il bordo di default del livello (uguale per tutti).
        */
        static void defPerInit(ALLEGRO_DISPLAY_MODE d);
        /*
            Funzione che regola i parametri che definiscono la difficoltà del livello.
        */
        void setDifficulty(unsigned d);
        /*
            Getter della difficoltà.
        */
        unsigned getDifficulty() const;
        /*
            Inizializza tutte le entità (Player/Boss/altri nemici) in accordo con il display. 
        */
        void entities_init();
        /*
            Se ci sono dei nemici inattivi, uno viene riattivato all'interno dell'area di gioco.
        */
        void spawn();
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
        int getArea(const GameList& GL) const;
};

#endif