#ifndef POLY_HPP_
#define POLY_HPP_
#include"Entities.h"
 
 void entities_init(vector<Entity*>& entities);
 void spawn(vector<Entity*>& entities);

 class Level
{
    public:
        Level(unsigned difficulty, ALLEGRO_DISPLAY_MODE settings, ALLEGRO_TIMER* tim);
        
        void loop();

    private:
        inline static perimeter defPerimeter{};
        ALLEGRO_TIMER* timer;
        unsigned numBosses;
        
        static void defPerInit(ALLEGRO_DISPLAY_MODE d);
        void setDifficulty(unsigned d);
        int getArea(const GameList& GL) const;
};

#endif