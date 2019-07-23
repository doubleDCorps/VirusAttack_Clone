#include"Player.h"

class Enemy: public Entity {
    private:
        bool boss;
    
    public:
        Enemy(float, float, unsigned, unsigned, float, float, bool);
        virtual ~Enemy() {}
        virtual void deathEvent() override;
        virtual void update() override;        
        bool isBoss() const;

};