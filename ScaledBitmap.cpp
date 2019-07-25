#include"ScaledBitmap.h"

int ScaledBitmap::refCount = 0;
ALLEGRO_BITMAP* ScaledBitmap::buffer = nullptr;

void ScaledBitmap::initBuffer()  { 
            
    if(buffer == nullptr)
        buffer = al_create_bitmap(screenWidth(), screenHeight()); 
}

void ScaledBitmap::delBuffer() {
            
    if(buffer != nullptr) {
        al_destroy_bitmap(buffer);
        buffer = nullptr;
    }
}

float ScaledBitmap::screenWidth()      { return 1280.0f; }
float ScaledBitmap::screenHeight()     { return 720.0f; }

float ScaledBitmap::windowWidth()      { return al_get_display_width(al_get_current_display()); }
float ScaledBitmap::windowHeight()     { return al_get_display_height(al_get_current_display()); }

float ScaledBitmap::sx()               { return windowWidth() / screenWidth(); }
float ScaledBitmap::sy()               { return windowHeight() / screenHeight(); } 
        
float ScaledBitmap::scale()            { return min(sx(), sy()); }
        
float ScaledBitmap::scale_W()          { return screenWidth() * scale(); }
float ScaledBitmap::scale_H()          { return screenHeight() * scale(); }

float ScaledBitmap::scale_X()          { return (windowWidth() - scale_W()) / 2.0f; } 
float ScaledBitmap::scale_Y()          { return (windowHeight() - scale_H()) / 2.0f; }

void ScaledBitmap::clear() {

    bool enablePrint = false;

    if(buffer != nullptr) {
        al_set_target_bitmap(buffer);
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    if(enablePrint)
        cout << "ScaledBitmap::clear() " << " " << buffer << "\n";
}

void ScaledBitmap::render() {

    if(buffer != nullptr) {
        al_set_target_backbuffer(al_get_current_display());
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_scaled_bitmap(buffer, 0, 0, screenWidth(), screenHeight(), scale_X(), scale_Y(), scale_W(), scale_H(), 0);
        al_flip_display();
    }
}

ScaledBitmap::ScaledBitmap(): bitmap(nullptr) { 
            
    if(refCount == 0)
        initBuffer();
    ++refCount;
}

ScaledBitmap::ScaledBitmap(const char* path): bitmap(al_load_bitmap(path)) { 
            
    if(refCount == 0)
        initBuffer();
    ++refCount;
}

ScaledBitmap::ScaledBitmap(int h, int w): bitmap(al_create_bitmap(h, w)) {
            
    if(refCount == 0)
        initBuffer();
    ++refCount;
}

ScaledBitmap::~ScaledBitmap() {
            
    if(bitmap != nullptr)
        al_destroy_bitmap(bitmap);
            
    if(refCount > 0)
        --refCount;
    if(refCount == 0)
        delBuffer();
}

void ScaledBitmap::draw(float x, float y)  {
            
    bool enablePrint = false;

    if(bitmap != nullptr) {
        al_set_target_bitmap(buffer);
        al_draw_bitmap(bitmap, x, y, 0);
    }

    if(enablePrint)
        cout << "ScaledBitmap::draw() " << this << bitmap << " -> " << buffer << endl;
}

ALLEGRO_BITMAP* ScaledBitmap::getMyBitmap() {
            
    return bitmap;
}

void ScaledBitmap::setMyBitmap(const char* path) {
            
    if(bitmap != nullptr)
        al_destroy_bitmap(bitmap);

    bitmap = al_load_bitmap(path);
}

void ScaledBitmap::setMyBitmap(int x, int y, ALLEGRO_COLOR c) {
            
    if(bitmap != nullptr)
        al_destroy_bitmap(bitmap);

    bitmap = al_create_bitmap(x, y);
    al_set_target_bitmap(bitmap);
    al_clear_to_color(c);
}