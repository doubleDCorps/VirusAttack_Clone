#include<map>
#include<iostream>
#include<set>
#include<vector>
#include<queue>
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
        static void clear();
        static void render();  

        ScaledBitmap();
        ScaledBitmap(const char* path);
        ScaledBitmap(int h, int w);
        ~ScaledBitmap();


        void draw(float x, float y);
        ALLEGRO_BITMAP* getMyBitmap();
        void setMyBitmap(const char* path);
        void setMyBitmap(int x, int y, ALLEGRO_COLOR c);
};