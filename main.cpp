
// INIZIALIZZARE ALLEGRO
    //al_init
    //al_keyboard
// INIZIALIZZARE VARIABILI E COMPONENTI
    //disp_data
    //display
    //timer
    //bouncer
    //event queue
// GAME LOOP
    // LOGICA : dobbiamo gestire il movimento, il cambiamento dei bordi, e le collisioni
    // REDRAW
// ALLEGRO_DESTROY

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
using namespace std;

 int main ()
{
    //_________________INIT____________________
    if(!al_init())
    {   
        cerr << "init_error: failed to initialize Allegro!\n";
        return -1;
    }

     if(!al_install_keyboard())
    {
        cerr << "init_error: failed to initialize keyboard!\n";
        return -1;
    }

    //__________VARIABLES_____________________
    ALLEGRO_DISPLAY_MODE disp_data;
    al_get_display_mode(0, &disp_data);
    al_set_new_display_flags(ALLEGRO_WINDOWED);

    bool key[4] = { false, false, false, false };

    ALLEGRO_TIMER *timer = al_create_timer(1.0/disp_data.refresh_rate);
    ALLEGRO_DISPLAY *display = al_create_display(disp_data.width, disp_data.height);
     if(!display)
    {   
        cerr << "disp_error: failed to initialize display!\n";
        al_destroy_timer(timer);
        return -1;
    }
    ALLEGRO_BITMAP *bouncer = al_create_bitmap(32, 32);
    ALLEGRO_EVENT_QUEUE *coda_eventi = al_create_event_queue();



    //________________BOUNCER____________________    
    al_set_target_bitmap(bouncer);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_set_target_bitmap(al_get_backbuffer(display));
    float bouncer_x=float(disp_data.width)/2 - 32/2;
    float bouncer_y=float(disp_data.height)/2 - 32/2;
    al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);
    


    //________________EVENT-QUEUE__________________
    al_register_event_source(coda_eventi, al_get_display_event_source(display));
    al_register_event_source(coda_eventi, al_get_timer_event_source(timer));
    al_register_event_source(coda_eventi, al_get_keyboard_event_source());
    al_clear_to_color(al_map_rgb(255,255,255));
    al_flip_display();
    al_start_timer(timer);


    bool redraw=true;
    bool STOP=false;

    while(!STOP) {

        ALLEGRO_EVENT evento;

        //implementazione MOVESET;

        al_wait_for_event(coda_eventi, &evento);

        if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            STOP = true;

        if(redraw && al_is_event_queue_empty(coda_eventi)) {
            
            redraw = false;
            al_clear_to_color(al_map_rgb(255,255,255));
            al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);
            al_flip_display();
            
            }        
              
    }
    
    al_destroy_event_queue(coda_eventi);
    al_destroy_bitmap(bouncer);
    al_destroy_display(display);
    al_destroy_timer(timer);

    return 0;
}