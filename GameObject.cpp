#include"GameObject.h"

float GameObject::dimension = 5;

GameObject::GameObject(float _x, float _y): x(_x), y(_y), image() {}

void GameObject::draw() {

    image.draw(getX(), getY());
    cout << "GameObject::draw() " << this 
    << ": pos -> " << x << " " << y << endl;
}

float GameObject::getX() const {
    
    return x;
}

float GameObject::getY() const {
    
    return y;
}

int GameObject::pos() const {
/* 
    cout << "GameObject::pos() " << this 
    << ": (" << x << ", " << y << ")"
    << " -> " << x * Root::getDim() + y << endl;
*/
    return x * Root::getDim() + y;
}

bool GameObject::operator==(const GameObject& G) const {
    
    return abs(x - G.x) <= 0.45f 
       and abs(y - G.y) <= 0.45f;
}

void GameObject::reposition(float val) {

    float x = val/(float)Root::getDim();
    float y = val - x;

    cout << "GameObject::reposition(float) " << this
    << ": " << val << " -> (" << x << ", " << y << ")\n";

    reposition(x, y);
}
        
void GameObject::reposition(float x, float y) {
    
    this->x = x;
    this->y = y;
}

void GameObject::reposition(const GameObject& G) {
    
    reposition(G.getX(), G.getY());
}

bool GameObject::checkForCollision(const GameObject *const G) const {

    for(const auto& element : Root::makeNeighborhood(pos()))
        if(element == G->pos())
            return true;
    return false;
}

void GameObject::setSize(float d) {

    dimension = d;

}

float GameObject::getSize() {
    
    return dimension;
}

void GameObject::setImage(string path) {

    image.setMyBitmap(path.c_str());
}

void GameObject::setImage(int x, int y, ALLEGRO_COLOR c) {

    image.setMyBitmap(x, y, c);
}