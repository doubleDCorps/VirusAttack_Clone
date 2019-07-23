#include<iostream>
#include<algorithm>
#include<string>
#include<allegro5/allegro5.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>
#include<allegro5/allegro_ttf.h>

using namespace std;

class ScaledBitmap {
    
    private:
        static int refCount;

        static ALLEGRO_BITMAP *buffer;
        
        static void initBuffer();

        static void delBuffer();

        ALLEGRO_BITMAP *bitmap;

    public:    
        static float screenWidth();
        static float screenHeight();

        static float windowWidth();
        static float windowHeight();

        static float sx();
        static float sy(); 
        
        static float scale();
        
        static float scale_W();
        static float scale_H();

        static float scale_X(); 
        static float scale_Y();
    
        ScaledBitmap(): bitmap(nullptr) { 
            
            if(refCount == 0)
                initBuffer();
            ++refCount;
        }

        ScaledBitmap(const char* path): bitmap(al_load_bitmap(path)) { 
            
            if(refCount == 0)
                initBuffer();
            ++refCount;
        }

        ScaledBitmap(int h, int w): bitmap(al_create_bitmap(h, w)) {
            
            if(refCount == 0)
                initBuffer();
            ++refCount;
        }

        ~ScaledBitmap() {
            
            if(bitmap != nullptr)
                al_destroy_bitmap(bitmap);
            
            if(refCount > 0)
                --refCount;
            if(refCount == 0)
                delBuffer();
        }

        static void clear();

        static void render();

        void draw(float x, float y) {
            
            if(bitmap != nullptr) {
                al_set_target_bitmap(buffer);
                al_draw_bitmap(bitmap, x, y, 0);
            }

            cout << "ScaledBitmap::draw() " << this << bitmap << endl;
        }

        ALLEGRO_BITMAP* getMyBitmap() {
            
            return bitmap;
        }

        void setMyBitmap(const char* path) {
            
            if(bitmap != nullptr)
                al_destroy_bitmap(bitmap);
            bitmap = al_load_bitmap(path);
        }

        void setMyBitmap(int x, int y, ALLEGRO_COLOR c) {
            
            if(bitmap != nullptr)
                al_destroy_bitmap(bitmap);
            bitmap = al_create_bitmap(x, y);
            al_set_target_bitmap(bitmap);
            al_clear_to_color(c);
        }
};