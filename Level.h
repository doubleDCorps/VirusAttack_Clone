#ifndef POLY_HPP_
#define POLY_HPP_
#include"Entities.h"

 initializer_list<perimeter> defPerInit(ALLEGRO_DISPLAY_MODE d)
{
    int x{ d.width/2  - 500/2 }, y{ d.height/2 - 500/2 };

    return {
        {x      , y      },
        {x + 500, y      },
        {x + 500, y + 500},
        {x      , y + 500}
    };
}

 void spawn(vector<Entity*>& entities)
{   
    for(unsigned i{2}; i < entities.size(); i++)
     if(!entities[i]->isAlive())
    {
        entities[i]->setAlive(true);
        entities[i]->setCord_x(entities[1]->getCord_x());
        entities[i]->setCord_y(entities[1]->getCord_y());
    }
}

  void entities_init(const Player& player, vector<Entity*>& entities)
{
    player = {275, 70, 4, al_create_bitmap(30,30)};                                        //PLAYER
    entities.push_back(new Enemy{float(800)/2-30/2, float(600)/2-30/2, -4.0, -4.0, al_create_bitmap(30,30)});   //BOSS 
    
    for(unsigned i=0; i<12; ++i)
        entities.push_back(new Enemy{float(800)/2-30/2, float(600)/2-30/2, 0, 0, al_create_bitmap(30,30)});     //MINIONS

    const int enemy_velocity{25};

    int cont_minion=2;
     while(cont_minion < 13)
    {
        bool presente{ false };
        int int_dx{ rand()%(enemy_velocity-12)+2 };
        int int_dy{ enemy_velocity-int_dx };

        for(unsigned i{1}; i<13; ++i)
         if(int_dx == entities[i]->getVelocity_x() and int_dy == entities[i]->getVelocity_y() )
        {
            presente = true;
            break;
        }

         if(!presente)
        {
            entities[cont_minion]->setVelocity_x(int_dx);
            entities[cont_minion]->setVelocity_y(int_dy);
            cont_minion++;
        }
    }

     for(unsigned i{1}; i<13; ++i)
    {
         if(i <= 4)
        {
            entities[i]->setVelocity_x(float((entities[i]->getVelocity_x())*-1.0)/10);
            entities[i]->setVelocity_y(float(entities[i]->getVelocity_y())/10);
        }
         else if(i >= 5 and i <= 7)
        {
            entities[i]->setVelocity_x(float(entities[i]->getVelocity_x())/10);
            entities[i]->setVelocity_y(float(entities[i]->getVelocity_y())/10);
        }
         else if(i >= 8 and i <= 10)
        {
            entities[i]->setVelocity_x(float(entities[i]->getVelocity_x())/10);
            entities[i]->setVelocity_y(float((entities[i]->getVelocity_y())*-1.0)/10); 
        }
         else if(i >= 11 and i <= 13)
        {
            entities[i]->setVelocity_x(float((entities[i]->getVelocity_x())*-1.0)/10);
            entities[i]->setVelocity_y(float((entities[i]->getVelocity_y())*-1.0)/10);
        }
    }

    al_set_target_bitmap(entities[0]->getBitmap()); //PLAYER_INIT
    al_clear_to_color(al_map_rgb(0, 255, 0));

    al_set_target_bitmap(entities[1]->getBitmap()); //BOSS_INIT
    al_clear_to_color(al_map_rgb(255, 0, 0));

     for(unsigned i{1};i<13;i++)
    {
        entities[i]->setAlive(false);
        al_set_target_bitmap(entities[i]->getBitmap());
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }
}

/*
    GameList è una lista di coppie di coordinate circolare e ordinata secondo il seguente criterio:
    per ogni coppia di elementi a1 e a2 consecutivi, la coppia precedente a0 e a1 sarà tale che
    se una coordinata è in comune fra a0 e a1, allora la coordinata opposta sarà in comune fra a1 e a2.
    Poichè la lista è circolare, ciò può essere valido anche per la coppia aN a0 (ultimo elemento - primo elemento).
    Inoltre GameList contiene un puntatore alla bitmap, utilizzata per rappresentare graficamente i segmenti
    composti da ogni coppia di elementi della lista.
*/
 class Level;

 class GameList: private list<PointData>
{
    friend Level;

    public:
        GameList(const list<PointData>& p = {}) { for(auto& i : p) push(i.first, i.second); }
        
        bool pushPoint(const PointData&);
        bool onEdge(const HitboxData&) const;
        bool inArea(const HitboxData&) const;

        AXIS collides(const EntityData&) const;     //implementata utilizzando onEdge()
        bool o_inside(const HitboxData&) const;     //implementata utilizzando onEdge() e inArea()
        bool c_inside(const HitboxData&) const;     //implementata utilizzando onEdge() e inArea()

        void print(ALLEGRO_BITMAP* buffer=nullptr) const;

    private:
        inline auto successor(list<PointData>::iterator it)                       { return ++it == end() ? begin() : it; }
        inline auto successor(list<PointData>::const_iterator it) const           { return ++it == cend() ? cbegin() : it; }
        inline auto successor(list<PointData>::reverse_iterator it)               { return ++it == rend() ? rbegin() : it; }
        inline auto successor(list<PointData>::const_reverse_iterator it) const   { return ++it == crend() ? crbegin() : it; }
};
/*
    Level rappresenta l'area di gioco, formata da:
    -un poligono rappresentante il bordo dell'area di gioco (border);
    -una linea spezzata rappresentante il tracciato del giocatore (trace);
    -un puntatore costante al giocatore (Player);
    -un puntatore costante al boss di livello (Boss);
    Essa gestisce tutte le routine riguardante la collisione col bordo, la posizione rispetto all'area di gioco
    (esterno/interno), l'aggiornamento dei bordi dell'area di gioco durante la partita, e il calcolo dell'area.
*/
 class Level
{
    public:
        Level(unsigned diff, ALLEGRO_DISPLAY_MODE mod): difficulty(diff), defPerimeter(defPerInit(mod))
        { 
            /*inizializza numBosses*/
        }
        //Level(): border(), trace(), Boss(nullptr), Player(nullptr) {}
        //Level(const perimeter& b, const Entity* P=nullptr, const Entity* B=nullptr): border(b), trace(), Boss(B), Player(P) {}     

        //se si sposta il gameLoop in Level::update, le funzioni qui sotto potrebbero passare a private:

        const GameList& getBorder() const { return border; }
        const GameList& getTrace() const  { return trace; }
        
        inline AXIS hitsBorder(const EntityData& D) const        { return border.hits(D.c[0], D.c[1], D.c[2], D.c[3], D.v[0], D.v[1]); }
        inline bool insideBorder(const HitboxData& D) const      { return border.inside(D.c[0], D.c[1], D.c[2], D.c[3]); }
        inline void printBorder(ALLEGRO_BITMAP* b=nullptr) const { border.print(b); }

        //inline AXIS hitsTrace(const EntityData& D) const         { return trace.hits(D.c[0], D.c[1], D.c[2], D.c[3], D.v[0], D.v[1]); }
        //inline bool insideTrace(const HitboxData& D) const       { return trace.inside(D.c[0], D.c[1], D.c[2], D.c[3]); }
        inline void printTrace(ALLEGRO_BITMAP* b=nullptr) const  { trace.print(b); }
        
        //

        void loop();
        void clear() { trace.clear(); }
        int getArea() const;
    
    private:
        unsigned difficulty;
        static perimeter defPerimeter;
        unsigned numBosses;
        //
};

#endif