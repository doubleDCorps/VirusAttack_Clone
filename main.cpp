#include"Level.h"
#include<allegro5/allegro.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void enemy_init(vector<Entity*>&);
void level(ALLEGRO_DISPLAY*, ALLEGRO_TIMER*);
void spawn(vector<Entity*>&);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 int main(int argc, char** argv)
{
    srand(time(0));
    if(!al_init() or !al_install_keyboard() or !al_init_primitives_addon()) return -1;
   
    ALLEGRO_DISPLAY_MODE disp_data;
     for(unsigned i{}; i < al_get_num_display_modes(); ++i)
    {
        al_get_display_mode(i, &disp_data);
        if(disp_data.width>=800 and disp_data.width<=900 and disp_data.height>=600 and disp_data.height<=700)
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
    
    level(display, timer);

    al_destroy_display(display);
    al_destroy_timer(timer);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 void entities_init(vector<Entity*>& entities)
{
    entities.push_back(new Player{275, 70, 4, al_create_bitmap(30,30)});                                        //PLAYER
    entities.push_back(new Enemy{float(800)/2-30/2, float(600)/2-30/2, -4.0, -4.0, al_create_bitmap(30,30)});   //BOSS 
    
    for(unsigned i=0; i<12; ++i)
        entities.push_back(new Enemy{float(800)/2-30/2, float(600)/2-30/2, 0, 0, al_create_bitmap(30,30)});     //MINIONS

    const int enemy_velocity{25};

    int cont_minion=2;
     while(cont_minion < 14)
    {
        bool presente{ false };
        int int_dx{ rand()%(enemy_velocity-12)+2 };
        int int_dy{ enemy_velocity-int_dx };

        for(unsigned i{2}; i<14; ++i)
         if(int_dx == entities[i]->getVelocity_x() and int_dy == entities[i]->getVelocity_y() )
        {
            presente = true;
            break;
        }

         if(!presente)
        {
            entities[cont_minion]->setVelocity_x(int_dx);
            entities[cont_minion]->setVelocity_y(int_dy);
            cont_minion++;
        }
    }

     for(unsigned i{2}; i<14; ++i)
    {
         if(i <= 4)
        {
            entities[i]->setVelocity_x(float((entities[i]->getVelocity_x())*-1.0)/10);
            entities[i]->setVelocity_y(float(entities[i]->getVelocity_y())/10);
        }
         else if(i >= 5 and i <= 7)
        {
            entities[i]->setVelocity_x(float(entities[i]->getVelocity_x())/10);
            entities[i]->setVelocity_y(float(entities[i]->getVelocity_y())/10);
        }
         else if(i >= 8 and i <= 10)
        {
            entities[i]->setVelocity_x(float(entities[i]->getVelocity_x())/10);
            entities[i]->setVelocity_y(float((entities[i]->getVelocity_y())*-1.0)/10); 
        }
         else if(i >= 11 and i <= 13)
        {
            entities[i]->setVelocity_x(float((entities[i]->getVelocity_x())*-1.0)/10);
            entities[i]->setVelocity_y(float((entities[i]->getVelocity_y())*-1.0)/10);
        }
    }

    al_set_target_bitmap(entities[0]->getBitmap()); //PLAYER_INIT
    al_clear_to_color(al_map_rgb(0, 255, 0));

    al_set_target_bitmap(entities[1]->getBitmap()); //BOSS_INIT
    al_clear_to_color(al_map_rgb(255, 0, 0));

     for(unsigned i{2};i<14;i++)
    {
        entities[i]->setAlive(false);
        al_set_target_bitmap(entities[i]->getBitmap());
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 void level(ALLEGRO_DISPLAY* display, ALLEGRO_TIMER* timer)
{
    ALLEGRO_EVENT_QUEUE *coda_eventi = al_create_event_queue();
     if(!coda_eventi) 
    {
        al_destroy_display(display);
        al_destroy_timer(timer); 
        return; 
    }

    vector<Entity*> entities;
    entities_init(entities);
    
    al_set_target_bitmap(al_get_backbuffer(display));
    al_clear_to_color(al_map_rgb(255, 255, 255));

    //si può implementare un inizializzazione parametrica, fissando le dimensioni a 500x500 e sfruttando ALLEGRO_DISPLAY_DATA?
    perimeter p{ {50, 25}, {550, 25}, {550, 525}, {50, 525} };

    Level poly(p, entities[0], entities[1]);

    bool redraw {true};
    bool STOP {false};
    int w{ 500 };
    int h{ 500 };
    bool space {false};
    KEYS key{still};
    bool state_changed{true};
    int spawn_time{0};
    bool first_spawn{true};

    al_register_event_source(coda_eventi, al_get_display_event_source(display));
    al_register_event_source(coda_eventi, al_get_timer_event_source(timer)); //Fusilli
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
            //condizioni di uscita
            STOP = (poly.getArea()*100/(w*h) <= 30) or (!entities[0]->isAlive());
            //STOP = !poly.insideBorder(entities[0]->getData()); //SOLO TEMPORANEO

            //player routines
            entities[0]->update( space ? key : 0, poly.hitsBorder(entities[0]->getData()) );

            //minions routines
            for(unsigned i=2; i < entities.size(); ++i) //Pasta fresca
             if(entities[i]->isAlive())
            {
                int  param1{ poly.hitsBorder(entities[i]->getData()) };
                bool param2{ state_changed ? poly.insideBorder(entities[i]->getData()) : true };
                
                entities[i]->update(param1, param2);
            } //Gnocchi
            
            //boss routines
            spawn_time++;
             if((spawn_time<300 or spawn_time>420) and (spawn_time<2101 or spawn_time>2221))
            {
                int  param1{ poly.hitsBorder(entities[1]->getData()) };
                bool param2{ state_changed ? poly.insideBorder(entities[1]->getData()) : true };

                entities[1]->update(param1, param2);
            }
             if(spawn_time==360 or spawn_time==2161)
            {
                spawn(entities);
                if(spawn_time!=360)
                    spawn_time=360;
            }

            //state_changed = poly.update(); //DA FIXARE
            redraw = true;
        }
         else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
         switch(ev.keyboard.keycode)
        {
            case ALLEGRO_KEY_ESCAPE: STOP = true;                  break;
            case ALLEGRO_KEY_UP:     if(key != UP)    key = UP;    break;
            case ALLEGRO_KEY_DOWN:   if(key != DOWN)  key = DOWN;  break;
            case ALLEGRO_KEY_LEFT:   if(key != LEFT)  key = LEFT;  break;
            case ALLEGRO_KEY_RIGHT:  if(key != RIGHT) key = RIGHT; break;
            case ALLEGRO_KEY_SPACE:  space = true;                 break;
            default:;
        }
         else if(ev.type == ALLEGRO_EVENT_KEY_UP)
         switch(ev.keyboard.keycode)
        {
            case ALLEGRO_KEY_UP:     if(key == UP)    key = still; break;
            case ALLEGRO_KEY_DOWN:   if(key == DOWN)  key = still; break;
            case ALLEGRO_KEY_LEFT:   if(key == LEFT)  key = still; break;
            case ALLEGRO_KEY_RIGHT:  if(key == RIGHT) key = still; break;
            case ALLEGRO_KEY_SPACE:  space = false;                break;
        }
         
         if(redraw and al_is_event_queue_empty(coda_eventi) )
        {
            redraw = false;

            al_clear_to_color(al_map_rgb(255, 255, 255));

            //redo this prints
            for(auto& it : entities)
                if(it->isAlive())
                    al_draw_bitmap(it->getBitmap(), it->getCord_x(), it->getCord_y(), 0);

            //ok
            poly.printTrace(al_get_backbuffer(display));
            poly.printBorder(al_get_backbuffer(display));
            
            al_flip_display();
        }
    }

     for(int i{}; i<entities.size(); ++i)
    {
        al_destroy_bitmap(entities[i]->getBitmap());
        delete entities[i];
    }

    al_destroy_event_queue(coda_eventi);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 void spawn(vector<Entity*>& entities)
{   
    for(unsigned i{2}; i < entities.size(); i++)
     if(!entities[i]->isAlive())
    {
        entities[i]->setAlive(true);
        entities[i]->setCord_x(entities[1]->getCord_x());
        entities[i]->setCord_y(entities[1]->getCord_y());
    }
}