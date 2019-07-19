#include<allegro5/allegro.h>
#include<allegro5/allegro5.h>
#include<allegro5/allegro_image.h>
using namespace std;

class Menu {
    private:
        ALLEGRO_BITMAP *buffer;
        ALLEGRO_BITMAP *unselected;
        ALLEGRO_BITMAP *start_selected;
        ALLEGRO_BITMAP *exit_selected;
        ALLEGRO_DISPLAY *display;
        ALLEGRO_TIMER *timer;
        ALLEGRO_EVENT_QUEUE *coda_eventi;
        int scale_W, scale_H, scale_X, scale_Y;

    public:
        Menu (ALLEGRO_DISPLAY *display, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *coda_eventi, int scaleW,int scaleH,int scaleX,int scaleY) {
            buffer = al_create_bitmap(1280, 720);
            start_selected = al_load_bitmap("./resources/Menu/Menu_START_SELECTED.png");
            exit_selected = al_load_bitmap("./resources/Menu/Menu_EXIT_SELECTED.png");

            this->display = display;
            this->timer = timer;
            this->coda_eventi = coda_eventi;
            this->scale_H = scaleH;
            this->scale_W = scaleW;
            this->scale_X = scaleX;
            this->scale_Y = scaleY;
        }

        short unsigned wait_choice() {
            cout << "sono in attesa di scelta" << endl;
            short unsigned state = 1;
            al_flush_event_queue(coda_eventi);
            ALLEGRO_EVENT event;
            al_start_timer(timer);
            bool redraw = true;

            cout << "fuori dal while" << endl;
            while(true) {
                cout << "dentro dal while" << endl;
                

                al_wait_for_event(coda_eventi, &event);
                switch(event.type) {
                    case ALLEGRO_EVENT_TIMER:
                        redraw = true;
                        break;
                    case ALLEGRO_EVENT_KEY_CHAR:
                        if(event.keyboard.keycode == ALLEGRO_KEY_UP and state!=1)
                            state--;
                        else if(event.keyboard.keycode == ALLEGRO_KEY_DOWN and state!=2)
                            state++;
                        else if(event.keyboard.keycode == ALLEGRO_KEY_ENTER and state == 1)
                            return 1;
                        else if((event.keyboard.keycode == ALLEGRO_KEY_ENTER and state == 2) or event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                            return 2;
                        break;
                    case ALLEGRO_EVENT_DISPLAY_CLOSE:
                        return 2;
                }

                if(redraw and al_is_event_queue_empty(coda_eventi)) {
                    al_set_target_bitmap(buffer);
                    if(state == 1)
                        al_draw_bitmap(start_selected, 0, 0, 0);
                    else if(state == 2)
                        al_draw_bitmap(exit_selected, 0, 0, 0);
                    
                    al_set_target_backbuffer(display);
                    al_clear_to_color(al_map_rgb(255, 255, 255));
                    //al_draw_bitmap(buffer, 0, 0, 0);
                    al_draw_scaled_bitmap(buffer, 0, 0, 1280, 720, scale_X, scale_Y, scale_W, scale_H, 0);
                    al_flip_display();
                    redraw=false;
                }


            }
            

            al_flush_event_queue(coda_eventi);
        }
};