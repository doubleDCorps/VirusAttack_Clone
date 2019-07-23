#include"GameObject.h"

class Hitbox: public GameObject {
    private:
        unsigned width;
        unsigned height;
    
    public:
        Hitbox(float, float, unsigned, unsigned);
        virtual ~Hitbox() {}
        virtual void draw();
        float getW() const;
        float getH() const;
        float getXW() const;
        float getYH() const;
        virtual bool checkForCollision(const Hitbox *const) const;
};