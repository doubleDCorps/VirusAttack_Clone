#ifndef ENEMY_H_
#define ENEMY_H_

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <array>
using namespace std;

class Entity {
    private:
        ALLEGRO_BITMAP *image;
        int width_hitbox, height_hitbox;
        float velocity_x, velocity_y, cord_x, cord_y;

    protected:

    public:
        Entity(int x = 0, int y = 0, int vx = 0, int vy = 0, int wh = 0, int hh = 0, ALLEGRO_BITMAP* p = nullptr):
            cord_x(x), cord_y(y), velocity_x(vx), velocity_y(vy), width_hitbox(wh), height_hitbox(hh), image(p) {}

        virtual ~Entity()=0;

        virtual void update (AXIS v)=0;
            
        int getWidth_hitbox()const;
        int getHeight_hitbox()const;
        float getVelocity_x()const;
        float getVelocity_y()const;
        float getCord_x()const;
        float getCord_y()const;

        void setWidth_hitbox(int);
        void setHeight_hitbox(int);
        void setVelocity_x(float);
        void setVelocity_y(float);
        void setCord_x(float);
        void setCord_y(float);
};
#endif