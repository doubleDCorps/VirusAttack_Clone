
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


#include"border.hpp"

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
     for(unsigned i{}; i < al_get_num_display_modes(); ++i) //potremmo settare il range minimo di risoluzioni (punto 3)
    {
        al_get_display_mode(i, &disp_data);
        if(disp_data.width >= 800 && disp_data.width <= 1400 && disp_data.height >= 500 && disp_data.height <= 1000)
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
    ALLEGRO_EVENT_QUEUE *coda_eventi = al_create_event_queue();

    //________________BOUNCER____________________    
    ALLEGRO_BITMAP *bouncer = al_create_bitmap(25, 25); //la dimensione del bitmap va fissata
    float bouncer_x = float(disp_data.width)/2 - 25/2;
    float bouncer_y = float(disp_data.height)/2 - 25/2;

    ALLEGRO_BITMAP *bordr = al_create_bitmap(20,20);
    ALLEGRO_BITMAP *tracr = al_create_bitmap(20,20);

    //________________EVENT-QUEUE__________________
    al_register_event_source(coda_eventi, al_get_display_event_source(display));
    al_register_event_source(coda_eventi, al_get_timer_event_source(timer));
    al_register_event_source(coda_eventi, al_get_keyboard_event_source());
    
    //_______________BORDER______________________
    list< pair<int, int> > test_list{{20, 20},   {275, 20},
                                     {275, 77},  {500, 77},
                                     {500, 20},  {600, 20}, 
                                     {600, 200}, {500, 200}, 
                                     {500, 300}, {600, 300},
                                     {600, 400}, {20, 400},
                                     {20, 150},  {120, 150}, 
                                     {120, 80},  {20, 80}};
    list< pair<int, int> > test_cist;
    
    Border tehone(test_list.begin(), test_list.end(), test_cist.begin(), test_cist.end(), nullptr, nullptr);

    //________________FIRST DRAW___________________

    al_set_target_bitmap(bouncer);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_set_target_bitmap(bordr);
    al_clear_to_color(al_map_rgb(100, 100, 0));

    al_set_target_bitmap(tracr);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_set_target_bitmap(al_get_backbuffer(display));
    al_clear_to_color(al_map_rgb(255, 255, 255));
    
    for(auto it : test_list)
        al_draw_bitmap(bordr, it.first, it.second, 0);

    int write_x = test_list.begin()->first;
    int write_y = test_list.begin()->second;
     for(auto it = ++(test_list.begin()); it != test_list.end(); ++it)
    {
        al_draw_bitmap(bordr, write_x, write_y, 0);
        if(write_x == it->first)
         for(unsigned i{}; i <= abs(it->second - write_y); i += 10)
            al_draw_bitmap(bordr, write_x, min(it->second, write_y) + i, 0);
        else if(write_y == it->second)
         for(unsigned i{}; i < abs(it->first - write_x); i += 10)
            al_draw_bitmap(bordr, min(it->first, write_x) + i, write_y, 0);
                    
        write_x = it->first;
        write_y = it->second;
    }

    if(write_x == test_list.begin()->first)
     for(unsigned i{}; i < abs(test_list.begin()->first - write_y); i += 10)
        al_draw_bitmap(bordr, write_x, min(test_list.begin()->second, write_y) + i, 0);
    else if(write_y == test_list.begin()->second)
     for(unsigned i{}; i < abs(test_list.begin()->first - write_x); i += 10)
        al_draw_bitmap(bordr, min(test_list.begin()->first, write_x) + i, write_y, 0);

    al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);
    al_flip_display();
    
    al_start_timer(timer);

    //________________LOOP_______________________
    bool redraw {true};
    bool STOP {false};
    bool touch_x{false};
    bool touch_y{false};
    float bouncer_dx {-4.0};
    float bouncer_dy {4.0};

     while(!STOP)
    {
        ALLEGRO_EVENT evento;

        al_wait_for_event(coda_eventi, &evento);

        Side t;

         if(evento.type == ALLEGRO_EVENT_TIMER)
        {
            t = tehone.in_border({bouncer_x, bouncer_y, 25, 25});
            
            if(t == NONE)
                touch_x = touch_y = false;

             if(!touch_x && t == VERTICAL)
            {
                bouncer_dx = -bouncer_dx;
                touch_x = true;
            }

             if(!touch_y && t == HORIZONTAL)
            {    
                bouncer_dy = -bouncer_dy;
                touch_y = true;
            }

            bouncer_x += bouncer_dx;
            bouncer_y += bouncer_dy;

            redraw = true;
        }
         else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            STOP = true;
        }

         if(redraw && al_is_event_queue_empty(coda_eventi))
        {
            redraw = false;
            al_clear_to_color(al_map_rgb(255,255,255));
            
            for(auto it : test_list)
                al_draw_bitmap(bordr, it.first, it.second, 0);

            int write_x = test_list.begin()->first;
            int write_y = test_list.begin()->second;
             for(auto it = ++(test_list.begin()); it != test_list.end(); ++it)
            {
                al_draw_bitmap(bordr, write_x, write_y, 0);
                if(write_x == it->first)
                 for(unsigned i{}; i <= abs(it->second - write_y); i += 10)
                    al_draw_bitmap(bordr, write_x, min(it->second, write_y) + i, 0);
                else if(write_y == it->second)
                 for(unsigned i{}; i < abs(it->first - write_x); i += 10)
                    al_draw_bitmap(bordr, min(it->first, write_x) + i, write_y, 0);
        
                write_x = it->first;
                write_y = it->second;
            }

            if(write_x == test_list.begin()->first)
             for(unsigned i{}; i < abs(test_list.begin()->first - write_y); i += 10)
                al_draw_bitmap(bordr, write_x, min(test_list.begin()->second, write_y) + i, 0);
            else if(write_y == test_list.begin()->second)
             for(unsigned i{}; i < abs(test_list.begin()->first - write_x); i += 10)
                al_draw_bitmap(bordr, min(test_list.begin()->first, write_x) + i, write_y, 0);

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