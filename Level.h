#include"Enemy.h"

class Level {
    private:
        enum class Body {PLAYER, ENEMY, BOSS, NONE};
        enum class Type {BORDER, TRACE, VOID};
        vector<GameObject*> allocation;
        map<int, Hitbox*> borders;
        map<int, Entity*> entities;
        Player* player;
        vector<Enemy*> boss;
        ALLEGRO_TIMER* timer;
        ScaledBitmap background;

        void respawnPlayer();
        void spawnEnemy(const Enemy *const);
        void initMap();
        void updateMap();
        void delMap();
        const Hitbox *const borderGet(int) const;
        const Entity *const entityGet(int) const;
        Type getObj(int) const;
        bool isObj(int) const;
        Body getBody(int) const;
        bool isBody(int) const;
        bool canSpawn() const;
        bool checkForBounds(int) const;

    public:
        static int shift(int);
        static pair<int, int> shift(int, int);

        Level(ALLEGRO_TIMER*);

        int getArea() const;
        int gameLoop();
};