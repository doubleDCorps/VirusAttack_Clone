#include"Hitbox.h"

class Entity: public Hitbox {
    private:
        bool active;
        float xspeed;
        float yspeed;

    protected:
        enum class Direction {RIGHT = 0, LEFT, DOWN, UP, STILL};
        void setActive(bool);

    public:
        Entity(float, float, unsigned, unsigned, float, float);
        virtual ~Entity() {}
        virtual void deathEvent()    = 0;
        virtual void update()        = 0;
        bool isActive() const;
        void respawn();
        float getXStep() const;
        float getYStep() const;
        void bounce(int val);
        bool locksOnObj(const Hitbox *const) const;
        virtual bool checkForCollision(const Hitbox *const) const;
};