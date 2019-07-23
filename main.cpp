#include"Level.h"
#include<allegro5/allegro5.h>
#include<allegro5/allegro.h>

int main(int argc, char** argv) {

    srand(time(0));
    if(!al_init() 
    or !al_init_image_addon() 
    or !al_init_font_addon() 
    or !al_init_ttf_addon() 
    or !al_install_audio() 
    or !al_init_acodec_addon() 
    or !al_install_keyboard() 
    or !al_init_primitives_addon())
            return -1;

    cout << "senza disp_data" << endl;
    ALLEGRO_DISPLAY_MODE disp_data;
    for(unsigned i{}; i < al_get_num_display_modes(); ++i) {
        al_get_display_mode(i, &disp_data);
        if(disp_data.width>=1280 and disp_data.width<=1380 and disp_data.height>=720 and disp_data.height<=820)
            break;
    }
    cout << "con disp_data" << endl;
    cout << disp_data.width << " " << disp_data.height << endl;
    
    ALLEGRO_TIMER *timer = al_create_timer(1.0f/30.0f);
    if(!timer)
        return -1;
    
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    ALLEGRO_DISPLAY *display = al_create_display(disp_data.width, disp_data.height);
    if(!display) { 
        al_destroy_timer(timer);
        return -1;
    }
/*
    ALLEGRO_EVENT_QUEUE *coda_eventi = al_create_event_queue();

    al_register_event_source(coda_eventi, al_get_keyboard_event_source());
    al_register_event_source(coda_eventi, al_get_display_event_source(display));
    al_register_event_source(coda_eventi, al_get_timer_event_source(timer));
    
    al_hide_mouse_cursor(display);
 */
    cout << "fin qua ci siamo\n";
    //Sounds sounds;
    //Level level(1, disp_data, timer, &sounds);
    Level level(timer);
    //Menu menu(display, timer, coda_eventi);
    short unsigned choice = 0;
    /*
    while(true) {
            sounds.playMenuTheme();
            choice = menu.wait_choice();
        
            if(choice == 1) { //START
                sounds.stopMenuTheme();
                sounds.playInLevelTheme();
            
            }
            else if(choice == 2) //EXIT
                break;
        }
        cout << "esco dal while" << endl;
    */

    cout << "game loop\n";

    level.gameLoop();
    /*
        Deallocazione risorse condivise.
    */
    al_destroy_display(display);
    al_destroy_timer(timer);

    return 0;

}