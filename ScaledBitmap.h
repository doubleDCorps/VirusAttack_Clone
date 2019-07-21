#include<cstring>
#include<allegro5/allegro.h>
#include<allegro5/allegro5.h>
#include<allegro5/allegro_image.h>

class ScaledBitmap {
    private:
        ALLEGRO_BITMAP *bitmap;
        float windowHeight = al_get_display_height(al_get_current_display());
        float windowWidth = al_get_display_width(al_get_current_display());
        float sx = windowWidth / 1280.0f;
        float sy = windowHeight / 720.0f;
        float scale = std::min(sx, sy);
        float scale_W = 1280 * scale;
        float scale_H = 720 * scale;
        float scale_X = (windowWidth - scale_W) / 2.0f;
        float scale_Y = (windowHeight - scale_H) / 2.0f;
    public:
        ScaledBitmap () {
            bitmap = al_create_bitmap(1280, 720);
        }

        void draw() {
            al_draw_bitmap(bitmap, 0, 0, 0);
        }
        void draw_scaled() {
            al_draw_scaled_bitmap(bitmap, 0, 0, 1280, 720, scale_X, scale_Y, scale_W, scale_H, 0);
        }

        ALLEGRO_BITMAP* getMyBitmap() {
            return bitmap;
        }

        void setMyBitmap(string path) {
            bitmap = al_load_bitmap(path.c_str());
        }

};