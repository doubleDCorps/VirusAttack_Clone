#ifndef ENEMY_H_
#define ENEMY_H_

#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<array>
#include<iostream>
#include<utility>
#include<list>
#include<cstdlib>
using namespace std;

typedef list<pair<int,int> > perimeter;
enum AXIS : int {none = 0, X = 1, Y = 2};
enum KEYS : int {none = 0, UP = 1, LEFT = 2, DOWN = 3, RIGHT = 4};

 class Entity
{
    private:
    protected:
        ALLEGRO_BITMAP *image;
        int width_hitbox, height_hitbox;
        float velocity_x, velocity_y, cord_x, cord_y;

        ~Entity() {};
    
    public:
        Entity(int x = 0, int y = 0, int vx = 0, int vy = 0, int wh = 0, int hh = 0, ALLEGRO_BITMAP* p = nullptr):
            cord_x(x), cord_y(y), velocity_x(vx), velocity_y(vy), width_hitbox(wh), height_hitbox(hh), image(p) {}

        void print(ALLEGRO_BITMAP* buffer) const; //da implementare
            
        int getWidth_hitbox() const;
        int getHeight_hitbox() const;
        float getVelocity_x() const;
        float getVelocity_y() const;
        float getCord_x() const;
        float getCord_y() const;
        ALLEGRO_BITMAP *getBitmap() const;

        void setWidth_hitbox(int);
        void setHeight_hitbox(int);
        void setVelocity_x(float);
        void setVelocity_y(float);
        void setCord_x(float);
        void setCord_y(float);
        void setBitmap(ALLEGRO_BITMAP*);
};

#endif