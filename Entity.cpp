#include "Entity.h"

 void print(ALLEGRO_BITMAP* buffer) const
{
     if(image!=nullptr && buffer!=nullptr)
    {
        //if(al_get_target_bitmap()!=buffer)  al_set_target_bitmap(buffer);
        //drawing operations
    }
}

int Entity::getWidth_hitbox()const { return width_hitbox; }

int Entity::getHeight_hitbox()const { return height_hitbox; }

float Entity::getVelocity_x()const { return velocity_x; }

float Entity::getVelocity_y()const { return velocity_y; }

float Entity::getCord_x()const { return cord_x; }

float Entity::getCord_y()const { return cord_y; }

void Entity::setWidth_hitbox(int w) { width_hitbox=w; }

void Entity::setHeight_hitbox(int h) { height_hitbox=h; }

void Entity::setVelocity_x(float vx) { velocity_x=vx; }

void Entity::setVelocity_y(float vy) { velocity_y=vy; }

void Entity::setCord_x(float cx) { cord_x=cx; }

void Entity::setCord_y(float cy) { cord_y=cy; }