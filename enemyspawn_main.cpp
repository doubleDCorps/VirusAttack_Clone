
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
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
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
     for(unsigned i{}; i < al_get_num_display_modes(); ++i)
    {
        al_get_display_mode(i, &disp_data);
        if(disp_data.width >= 800 && disp_data.width <= 900 && disp_data.height >= 600 && disp_data.height <= 700)
            break;
    }

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

    ALLEGRO_BITMAP *bouncer[12];
    for(unsigned i=0;i<12;i++)
        bouncer[i] = al_create_bitmap(32, 32);

    ALLEGRO_EVENT_QUEUE *coda_eventi = al_create_event_queue();



    //________________BOUNCER____________________
    for(unsigned i=0;i<12;i++)    
    {    
        al_set_target_bitmap(bouncer[i]);
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    al_set_target_bitmap(al_get_backbuffer(display));
    al_clear_to_color(al_map_rgb(255,255,255));
    float bouncer_x[12], bouncer_y[12];
    for(unsigned i=0;i<12;i++) {
        bouncer_x[i] = float(disp_data.width)/2 - 32/2;
        bouncer_y[i] = float(disp_data.height)/2 - 32/2;
        al_draw_bitmap(bouncer[i], bouncer_x[i], bouncer_y[i], 0);
        }
    


    //________________EVENT-QUEUE__________________
    al_register_event_source(coda_eventi, al_get_display_event_source(display));
    al_register_event_source(coda_eventi, al_get_timer_event_source(timer));
    al_register_event_source(coda_eventi, al_get_keyboard_event_source());
    al_flip_display();
    al_start_timer(timer);


    bool redraw=true;
    bool STOP=false;


//____________________________________________________VELOCITA' NEMICI: dove va messa?
    srand(time(0));
    const short enemy_velocity = 25;
    vector<pair<float, float>> bouncer_cord;


while(bouncer_cord.size()<12) {

    bool presente = false;
    int int_dx = rand()%13+2, int_dy = enemy_velocity-int_dx;

    for(unsigned i=0;i<bouncer_cord.size();i++) 
        if(int_dx == bouncer_cord[i].first && int_dy == bouncer_cord[i].second)
            presente = true;

    if(presente == false)
        bouncer_cord.push_back({int_dx, int_dy});   
    }

for(unsigned i=0;i<bouncer_cord.size();i++) {
    if(i<=3) {
        bouncer_cord[i].first=float((bouncer_cord[i].first)*-1)/10;
        bouncer_cord[i].second=float(bouncer_cord[i].second)/10; //max 1.5 min 0.2
        }
    if(i>=4 and i<=6) {
        bouncer_cord[i].first=float(bouncer_cord[i].first)/10;
        bouncer_cord[i].second=float(bouncer_cord[i].second)/10;
        }
    if(i>=7 and i<=9) {
        bouncer_cord[i].first=float(bouncer_cord[i].first)/10;
        bouncer_cord[i].second=float((bouncer_cord[i].second)*-1)/10;
        }
    if(i>=10 and i<=12) {
        bouncer_cord[i].first=float((bouncer_cord[i].first)*-1)/10;
        bouncer_cord[i].second=float((bouncer_cord[i].second)*-1)/10;
        }
    }

for(unsigned i=0;i<bouncer_cord.size();i++)
    cout << bouncer_cord[i].first << " " << bouncer_cord[i].second << endl;;
//______________________________________________________    


    while(!STOP) {

        ALLEGRO_EVENT evento;

        al_wait_for_event(coda_eventi, &evento);

         if(evento.type == ALLEGRO_EVENT_TIMER)
        {
            for(unsigned i=0;i<12;i++) {   
                if(bouncer_x[i] < 0 || bouncer_x[i] > disp_data.width - 32)
                    bouncer_cord[i].first = -bouncer_cord[i].first;

                if(bouncer_y[i] < 0 || bouncer_y[i] > disp_data.height - 32)
                    bouncer_cord[i].second = -bouncer_cord[i].second;

                bouncer_x[i] += bouncer_cord[i].first;
                bouncer_y[i] += bouncer_cord[i].second;
                }

            redraw = true;
        }
         else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            STOP = true;
        }

        if(redraw && al_is_event_queue_empty(coda_eventi)) {

            redraw = false;
            al_clear_to_color(al_map_rgb(255,255,255));

            for(unsigned i=0;i<12;i++)
                al_draw_bitmap(bouncer[i], bouncer_x[i], bouncer_y[i], 0);

            al_flip_display();
            
            }        
              
    }
    
    al_destroy_event_queue(coda_eventi);

    for(unsigned i=0;i<12;i++)
        al_destroy_bitmap(bouncer[i]);

    al_destroy_display(display);
    al_destroy_timer(timer);

    return 0;
}