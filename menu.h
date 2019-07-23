#include"ScaledBitmap.h"
#include<cstring>
#include<allegro5/allegro.h>
#include<allegro5/allegro5.h>
#include<allegro5/allegro_image.h>
using namespace std;

class Menu {
    private:
        ScaledBitmap buffer;
        ScaledBitmap start_selected;
        ScaledBitmap exit_selected;
        //ALLEGRO_BITMAP *start_selected;
        //ALLEGRO_BITMAP *exit_selected;
        ALLEGRO_DISPLAY *display;
        ALLEGRO_TIMER *timer;
        ALLEGRO_EVENT_QUEUE *coda_eventi;

    public:
        Menu (ALLEGRO_DISPLAY *display, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *coda_eventi): start_selected(), exit_selected(), buffer() {
            this->display = display;
            this->timer = timer;
            this->coda_eventi = coda_eventi;

            start_selected.setMyBitmap("resources/menu/Menu_START_SELECTED.png");
            exit_selected.setMyBitmap("resources/menu/Menu_EXIT_SELECTED.png");
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

                cout << "so al redraw\n";

                if(redraw and al_is_event_queue_empty(coda_eventi)) {
                    cout << "so dentro l'if\n";
                    al_set_target_bitmap(buffer.getMyBitmap());
                    cout << "ho settato la bitmap\n";

                    cout << buffer.windowWidth << "\n"
                    << buffer.windowHeight << "\n"
                    << buffer.sx << "\n"
                    << buffer.sy << "\n"
                    << buffer.scale << "\n"
                    << buffer.scale_W << "\n"
                    << buffer.scale_H << "\n"
                    << buffer.scale_X << "\n"
                    << buffer.scale_Y << "\n";

                    if(state == 1)
                        start_selected.draw();
                    else if(state == 2)
                        exit_selected.draw();
                    cout << "ho disegnato sul buffer\n";
                    al_set_target_backbuffer(display);
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    cout << "ho disegnato buffer\n";
                    buffer.draw_scaled();
                    al_flip_display();
                    redraw=false;
                }

                cout << "fine ciclo\n";
            }
            

            al_flush_event_queue(coda_eventi);
        }
};