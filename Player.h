#include"Entity.h"

class Player: public Entity {
    private:
        int lifes = 3;
        bool isMoving = false;
        bool pressed = false;
        Direction dir = Direction::STILL;
        int reverse_cont = 0;
        int pressed_key = 0;
        bool force_reverse_way = false;
        bool first_reverse_step = true;
        vector<const GameObject*> trace;
        vector<pair<int, int>> reverse_way;
        void clearReverse();

    public:
        Player(float, float, unsigned, unsigned, float, float);
        virtual ~Player() {}
        virtual void deathEvent() override;
        virtual void update() override;
        void updateKey(int, ALLEGRO_EVENT_TYPE);
        void updateTrace(const GameObject*const);
        bool isTraceEmpty() const;
        bool contains(const GameObject&) const;
        bool loseCondition() const;
};
