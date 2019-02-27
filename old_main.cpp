#include "polygon.hpp"

 int main()
{
    srand(time(0));
    if(!al_init())
        return -1;
    if(!al_install_keyboard())
        return -1;
    
    ALLEGRO_DISPLAY_MODE disp_data;
     for(unsigned i{}; i < al_get_num_display_modes(); ++i)
    {
        al_get_display_mode(i, &disp_data);
        if(disp_data.width >= 800 && disp_data.width <= 900 && disp_data.height >= 600 && disp_data.height <= 700)
            break;
    }
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    
    ALLEGRO_TIMER *timer = al_create_timer(1.0/disp_data.refresh_rate);
     if(!timer)
        return -1;
    
    ALLEGRO_DISPLAY *display = al_create_display(disp_data.width, disp_data.height);
     if(!display)
    {   
        al_destroy_timer(timer);
        return -1;
    }
    
    ALLEGRO_EVENT_QUEUE *coda_eventi = al_create_event_queue();
     if(!coda_eventi)
    {
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    //START__________DA MUOVERE IN ENTITY E ENEMY

    ALLEGRO_BITMAP* bouncer[12];
    float bouncer_x[12];
    float bouncer_y[12];
    list<pair<float, float>> bouncer_v;
    
    const int enemy_velocity{50};

     for(unsigned i{}; i<12 ; ++i)
    {   
        bouncer[i] = al_create_bitmap(30, 30);
        bouncer_x[i] = float(800)/2 - 30/2;
        bouncer_y[i] = float(600)/2 - 30/2;
    }

     while(bouncer_v.size() < 12)
    {
        bool presente{ false };
        int int_dx{ rand()%(enemy_velocity-12)+2 };
        int int_dy{ enemy_velocity-int_dx };

        for(auto& i : bouncer_v) 
         if(int_dx == i.first && int_dy == i.second)
        {
            presente = true;
            break;
        }

        if(!presente)
            bouncer_v.push_back({int_dx, int_dy});   
    }

    int i{};
     for(auto& k : bouncer_v)
    {
         if(i <= 3)
        {
            k.first = float((k.first)*-1.0)/10;
            k.second = float(k.second)/10;
        }
         else if(i >= 4 && i <= 6)
        {
            k.first  = float(k.first)/10;
            k.second = float(k.second)/10;
        }
         else if(i >= 7 && i <= 9)
        {
            k.first  = float(k.first)/10;
            k.second = float((k.second)*-1.0)/10;
        }
         else if(i >= 10 && i <= 12)
        {
            k.first  = float((k.first)*-1.0)/10;
            k.second = float((k.second)*-1.0)/10;
        }

        ++i;
    }

     for(unsigned i{}; i < 12; ++i)
    {
        al_set_target_bitmap(bouncer[i]);
        al_clear_to_color(al_map_rgb(rand()%256, rand()%256, rand()%256));
    }

        al_set_target_bitmap(al_get_backbuffer(display));
        al_clear_to_color(al_map_rgb(255, 255, 255));

    for(unsigned i{}; i < 12; ++i)
        al_draw_bitmap(bouncer[i], bouncer_x[i], bouncer_y[i], 0);

    //END___________DA MUOVERE IN ENTITY E ENEMY

    al_flip_display();

    Polygon poly;

    bool redraw {true};
    bool STOP {false};

    al_register_event_source(coda_eventi, al_get_display_event_source(display));
    al_register_event_source(coda_eventi, al_get_timer_event_source(timer));
    al_register_event_source(coda_eventi, al_get_keyboard_event_source());
    al_start_timer(timer);

     while(!STOP)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(coda_eventi, &ev);

         if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            STOP = true;
         else if(ev.type == ALLEGRO_EVENT_TIMER)
        {
             for(unsigned i{}; i < bouncer_v.size(); ++i)
            {
                //Enemy[i].update( poly.hitsB( Enemy[i].getData() ) ); sarebbe l'idea
                //START___________DA MUOVERE IN ENTITY E ENEMY 
                 if(poly.hitsB({bouncer_x[i], bouncer_y[i], 30, 30, 
                                next(bouncer_v.begin(), i)->first, next(bouncer_v.begin(), i)->second}) == X)
                {   
                    next(bouncer_v.begin(), i)->first = -next(bouncer_v.begin(), i)->first;
                }
                 if(poly.hitsB({bouncer_x[i], bouncer_y[i], 30, 30, 
                                next(bouncer_v.begin(), i)->first, next(bouncer_v.begin(), i)->second}) == Y)
                {   
                    
                    next(bouncer_v.begin(), i)->second = -next(bouncer_v.begin(), i)->second;
                }
                //END___________DA MUOVERE IN ENTITY E ENEMY 
            }

            //START__________DA MUOVERE IN ENTITY E ENEMY

             for(unsigned i{}; i < 12; ++i)
            {
                bouncer_x[i] += next(bouncer_v.begin(), i)->first;
                bouncer_y[i] += next(bouncer_v.begin(), i)->second;
            }

            //END___________DA MUOVERE IN ENTITY E ENEMY

            redraw = true;
        }

         if(redraw && al_is_event_queue_empty(coda_eventi))
        {
            redraw = false;

            al_clear_to_color(al_map_rgb(255, 255, 255));

             for(unsigned i{}; i < 12; ++i)
            {
                al_draw_bitmap(bouncer[i], bouncer_x[i], bouncer_y[i], 0);
            }

            al_flip_display();
        }
    }

    for(unsigned i{}; i < 12; ++i)
        cout << '(' << bouncer_x[i] << ", " << bouncer_y[i] << ") " << poly.is_inside({bouncer_x[i], bouncer_y[i]}) << endl;
    
    for(auto i : bouncer)
        al_destroy_bitmap(i);

    al_destroy_event_queue(coda_eventi);
    al_destroy_display(display);
    al_destroy_timer(timer);

    return 0;
}