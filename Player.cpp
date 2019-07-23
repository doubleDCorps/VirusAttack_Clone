#include"Player.h"

Player::Player(float x, float y, unsigned w, unsigned h, float sx, float sy): 
    Entity(x, y, w, h, sx, sy), trace(), reverse_way() {}

void Player::deathEvent() {

    lifes = lifes > 0 ? --lifes : 0;
    setActive(false);
    trace.clear();
}

void Player::update() {
    
    if(!isActive()) return;
    
    //roba per il reverse algorithm
    float arrx[] = {1.0f, -1.0f, 0.0f,  0.0f};
    float arry[] = {0.0f,  0.0f, 1.0f, -1.0f};

    if(dir != Direction::STILL)
        reposition(pos() + arrx[(int)dir]*getXStep() + arry[(int)dir]*getYStep());

}

void Player::updateKey(int k, ALLEGRO_EVENT_TYPE h) {

    
}

void Player::updateTrace(const GameObject* G) {

    trace.push_back(G);
}

bool Player::isTraceEmpty() const {
    
    return trace.empty();
}

bool Player::contains(const GameObject& param) const {
    
    return find(trace.begin(), trace.end(), &param) != trace.end();
}

bool Player::loseCondition() const {

    return lifes == 0;
}