#include"root.h"

class GameObject {
    
    private:
        static float dimension;
        float x;
        float y;
    
    protected:
        ScaledBitmap image;
        void addToX(float);
        void addToY(float);
        
    public:
        static void setSize(float);
        static float getSize();

        GameObject(float, float);
        virtual ~GameObject() {}
        virtual void draw();
        float getX() const;
        float getY() const;
        int pos() const;
        void reposition(int);
        void reposition(float, float);
        void reposition(const GameObject&);
        bool checkForCollision(const GameObject *const) const;
        void setImage(string);
        void setImage(int, int, ALLEGRO_COLOR);

        bool operator==(const GameObject&) const;
        bool operator!=(const GameObject&) const;
};