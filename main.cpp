#include"polygon.h"

void menu();
void enemy_init(const vector<Minion>&, ALLEGRO_DISPLAY*);
void level(ALLEGRO_DISPLAY*, ALLEGRO_TIMER*);

 int main()
{
    srand(time(0));
    if(!al_init())              return -1;
    if(!al_install_keyboard())  return -1;
    
    ALLEGRO_DISPLAY_MODE disp_data;
     for(unsigned i{}; i < al_get_num_display_modes(); ++i)
    {
        al_get_display_mode(i, &disp_data);
        if(disp_data.width>=800 && disp_data.width<=900 && disp_data.height>=600 && disp_data.height<=700)
            break;
    }
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    
    ALLEGRO_TIMER *timer = al_create_timer(1.0/disp_data.refresh_rate);
     if(!timer) return -1;
    
    ALLEGRO_DISPLAY *display = al_create_display(disp_data.width, disp_data.height);
     if(!display) { al_destroy_timer(timer); return -1; }
    
    level(display, timer);

    al_destroy_display(display);
    al_destroy_timer(timer);

    return 0;
}

 void enemy_init(vector<Minion>& minions, ALLEGRO_DISPLAY* display)
{
    const int enemy_velocity{25};

    int cont_minion=0;
     while(cont_minion<12)
    {
        bool presente{ false };
        int int_dx{ rand()%(enemy_velocity-12)+2 };
        int int_dy{ enemy_velocity-int_dx };

        for(unsigned i{};i<12;i++)
             if(int_dx==minions[i].getVelocity_x() && int_dy==minions[i].getVelocity_y())
            {
                presente = true;
                break;
            }

         if(!presente)
        {
            minions[cont_minion].setVelocity_x(int_dx);
            minions[cont_minion].setVelocity_y(int_dy);
            cont_minion++;
        }
    }

     for(unsigned i{};i<12;i++)
    {
         if(i <= 3)
        {
            minions[i].setVelocity_x(float((minions[i].getVelocity_x())*-1.0)/10);
            minions[i].setVelocity_y(float(minions[i].getVelocity_y())/10);
        }
         else if(i >= 4 && i <= 6)
        {
            minions[i].setVelocity_x(float(minions[i].getVelocity_x())/10);
            minions[i].setVelocity_y(float(minions[i].getVelocity_y())/10);
        }
         else if(i >= 7 && i <= 9)
        {
            minions[i].setVelocity_x(float(minions[i].getVelocity_x())/10);
            minions[i].setVelocity_y(float((minions[i].getVelocity_y())*-1.0)/10);
        }
         else if(i >= 10 && i <= 12)
        {
            minions[i].setVelocity_x(float((minions[i].getVelocity_x())*-1.0)/10);
            minions[i].setVelocity_y(float((minions[i].getVelocity_y())*-1.0)/10);
        }

        ++i;
    }

     for(unsigned i{}; i < 12; ++i)
    {
        al_set_target_bitmap(minions[i].getBitmap());
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    al_set_target_bitmap(al_get_backbuffer(display));
    al_clear_to_color(al_map_rgb(255, 255, 255));

    for(unsigned i{}; i < 12; ++i)
        al_draw_bitmap(minions[i].getBitmap(), minions[i].getCord_x(), minions[i].getCord_y(), 0);

    al_flip_display();
}

 void level(ALLEGRO_DISPLAY* display, ALLEGRO_TIMER* timer)
{
    ALLEGRO_EVENT_QUEUE *coda_eventi = al_create_event_queue();
     if(!coda_eventi) 
    { al_destroy_display(display); al_destroy_timer(timer); return; }

    vector<Minion> minions(12, {float(800)/2 - 30/2, float(600)/2 - 30/2, 0, 0, 30+2*2, 30+2*2, al_create_bitmap(30,30), true});

    enemy_init(minions, display);

    Boss boss(float(800)/2 - 30/2, float(600)/2 - 30/2, -0.5, -0.5, 30+2*2, 30+2*2, al_create_bitmap(30,30));
    
    al_set_target_bitmap(boss.getBitmap());
    al_clear_to_color(al_map_rgb(255, 0, 0));
    al_set_target_bitmap(al_get_backbuffer(display));
    
    Player player(25, 25, 0, 0, 30, 30, nullptr);

    perimeter p = { {25, 25}, {525, 25}, {525, 525}, {25, 525} };

    int w = 500;
    int h = 500;
    ALLEGRO_BITMAP* t = nullptr; //DA IMPLEMENTARE!
    GameArea poly(p, t, t, &boss, &player);

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
            poly.update();
             for(unsigned i{}; i < 12; ++i)
            {
                 if(poly.getArea()*100/(w*h) <= 30)
                {
                    STOP = true;
                    break;
                }
                 if(!player.getLifes())
                {
                    STOP = true;
                    break;
                }
                 if(!poly.insideBorder(minions[i].getData() ) )
                {
                    minions[i].setAlive(false);
                }
                 if(minions[i].getAlive() )
                {
                    minions[i].update(poly.hitsBorder(minions[i].getData() ) );
                }
                boss.update(poly.hitsBorder(boss.getData() ) );
            }

            redraw = true;
        }
         if(redraw && al_is_event_queue_empty(coda_eventi) )
        {
            redraw = false;

            al_clear_to_color(al_map_rgb(255, 255, 255) );
            for(auto it : minions)
             if(it.getAlive())
            {
                al_draw_bitmap(it.getBitmap(), it.getCord_x(), it.getCord_y(), 0); //questo va nel print dei minion
            };
            al_draw_bitmap(boss.getBitmap(), boss.getCord_x(), boss.getCord_y(), 0); //questo va nel print del boss
            
            al_flip_display();
        }
    }
    
    for(unsigned i{}; i < 12; ++i)
        cout<<"("<<minions[i].getCord_x()<<", "<<minions[i].getCord_y()<<") "
        <<"speed: "<<"("<<minions[i].getVelocity_x()<<", "<<minions[i].getVelocity_y()<<") "
        <<poly.insideBorder(minions[i].getData() )<<' '
        <<minions[i].getAlive()<<endl;

    //al_rest(2.0);

    for(auto i : minions)
        al_destroy_bitmap(i.getBitmap());
    al_destroy_bitmap(boss.getBitmap());

    al_destroy_event_queue(coda_eventi);
    //al_stop_timer(timer);

    return;
}