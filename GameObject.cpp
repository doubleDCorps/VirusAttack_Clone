#include"GameObject.h"

float GameObject::dimension = 5;

int GameObject::shift(int k) {

    return k + (120/GameObject::getSize())*Root::getDim() + (60/GameObject::getSize());
}

pair<int, int> GameObject::shift(int x, int y) {

    return {x + (120/GameObject::getSize()), y + (60/GameObject::getSize())};
}

GameObject::GameObject(float _x, float _y): x(_x), y(_y), image() {}

void GameObject::draw() {

    bool enablePrint = false;

    const auto& val = shift(x, y);
    image.draw(val.first*getSize(), val.second*getSize());
    
    if(enablePrint)
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

void GameObject::reposition(int val) {

    bool enablePrint = true;

    float x = val/Root::getDim();
    float y = val%Root::getDim();

    if(enablePrint)
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

    image.setMyBitmap(x*GameObject::getSize(), y*GameObject::getSize(), c);
}

void GameObject::addToX(float dx) {

    x += dx;
}

void GameObject::addToY(float dy) {

    y += dy;
}

bool GameObject::operator==(const GameObject& G) const {
    
    return abs(x - G.x) <= 1.0f 
       and abs(y - G.y) <= 1.0f;
}

bool GameObject::operator!=(const GameObject& G) const {

    return !operator==(G);
}