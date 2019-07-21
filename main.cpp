#include"Level.h"
#include"menu.h"
#include<iostream>
 
 int main(int argc, char** argv)
{


    cout << "sono depresso" << endl;
    /*
        Inizializzazione stati di Allegro e srand per alcune routines.
    */
    srand(time(0));
    if(!al_init() or
       !al_init_image_addon() or
       !al_install_audio() or
       !al_init_acodec_addon() or
       !al_install_keyboard() or
       !al_init_primitives_addon())
            return -1;
    /*
        Inizializzazione del display (legge la risoluzione da quelle di default del dispositivo).
    */


    cout << "senza disp_data" << endl;
    ALLEGRO_DISPLAY_MODE disp_data;
     for(unsigned i{}; i < al_get_num_display_modes(); ++i)
    {
        al_get_display_mode(i, &disp_data);
        if(disp_data.width>=1280 and disp_data.width<=1380 and disp_data.height>=720 and disp_data.height<=820)
            break;
    }
    cout << "con disp_data" << endl;
    cout << disp_data.width << " " << disp_data.height << endl;
    
    ALLEGRO_TIMER *timer = al_create_timer(1.0/30);
     if(!timer)
        return -1;
    
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    ALLEGRO_DISPLAY *display = al_create_display(disp_data.width, disp_data.height);
     if(!display)
    { 
        al_destroy_timer(timer);
        return -1;
    }

    ALLEGRO_EVENT_QUEUE *coda_eventi = al_create_event_queue();

    /*
        Istanzia il livello (qui in futuro istanze di menu etc.?)
    */

    al_register_event_source(coda_eventi, al_get_keyboard_event_source());
    al_register_event_source(coda_eventi, al_get_display_event_source(display));
    al_register_event_source(coda_eventi, al_get_timer_event_source(timer));
    
    al_hide_mouse_cursor(display); //nasconde il cursore poichè non utilizzato.

    Sounds sounds;
    Level level(1, disp_data, timer, &sounds);
    Menu menu(display, timer, coda_eventi);
    short unsigned choice = 0;

   while(true) {
        sounds.playMenuTheme();
        choice = menu.wait_choice();
        
        if(choice == 1) { //START
            sounds.stopMenuTheme();
            sounds.playInLevelTheme();
            level.loop();
        }
        else if(choice == 2) //EXIT
            break;
    }
    cout << "esco dal while" << endl;


    /*
        Deallocazione risorse condivise.
    */

    al_destroy_display(display);
    al_destroy_timer(timer);

    return 0;
}
/*
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
    int reverse_cont{0};
    int pressed_key{0};
    vector<pair<int, int>>reverse_way;
    bool first_reverse_step{true};

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
            //condizioni di uscita
            STOP = (poly.getArea()*100/(w*h) <= 30) or (!entities[0]->isAlive());
            
            //player routines
            entities[0]->update( space ? key : 0, poly.hitsBorder(entities[0]->getData()) );

            //minions routines
            for(unsigned i=2; i < entities.size(); ++i)
             if(entities[i]->isAlive())
            {
                int  param1{ poly.hitsBorder(entities[i]->getData()) };
                bool param2{ state_changed ? poly.insideBorder(entities[i]->getData()) : true };
                
                entities[i]->update(param1, param2);
            }

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

            state_changed = poly.update();
            redraw = true;


            if(space && !(poly.hitsBorder(entities[0]->getData()))) {
                if((key == UP || key == DOWN || key == LEFT || key == RIGHT) && first_reverse_step) {
                    first_reverse_step=false;
                    pressed_key=key;
                    }

                if(key == UP && !first_reverse_step && pressed_key == UP)
                    reverse_cont++;
                else if(key == UP && !first_reverse_step && pressed_key != UP) {
                    push_reverse_way(reverse_cont, reverse_way, pressed_key);
                    pressed_key = UP;
                    reverse_cont++;
                    }
                
                if(key == DOWN && !first_reverse_step && pressed_key == DOWN)
                    reverse_cont++;
                else if(key == DOWN && !first_reverse_step && pressed_key != DOWN) {
                    push_reverse_way(reverse_cont, reverse_way, pressed_key);
                    pressed_key = DOWN;
                    reverse_cont++;
                    }

                if(key == LEFT && !first_reverse_step && pressed_key == LEFT)
                    reverse_cont++;
                else if(key == LEFT && !first_reverse_step && pressed_key != LEFT) {
                    push_reverse_way(reverse_cont, reverse_way, pressed_key);
                    pressed_key = LEFT;
                    reverse_cont++;
                    }

                if(key == RIGHT && !first_reverse_step && pressed_key == RIGHT)
                    reverse_cont++;
                else if(key == RIGHT && !first_reverse_step && pressed_key != RIGHT) {
                    push_reverse_way(reverse_cont, reverse_way, pressed_key);
                    pressed_key = RIGHT;
                    reverse_cont++;
                    }
                }
            else if(poly.hitsBorder(entities[0]->getData()))
                reverse_way.clear();
            else {
                push_reverse_way(reverse_cont, reverse_way, pressed_key);
                first_reverse_step=true;

                if(reverse_way.size()!=0) { //ALGORITMO CAMMINO INVERSO
                    if(reverse_way[reverse_way.size()-1].first>0)
                        entities[0]->update(reverse_way[reverse_way.size()-1].second, true);

                    reverse_way[reverse_way.size()-1].first--;
                    if(reverse_way[reverse_way.size()-1].first==0) 
                        reverse_way.pop_back();
                    }
                }

        } //FINE ALLEGRO_EVENT_TIMER


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

void push_reverse_way(int &reverse_cont, vector<pair<int, int>> &reverse_way, int &pressed_key) {
    if(reverse_cont!=0) {
        if(pressed_key==1) //UP
            reverse_way.push_back({reverse_cont, 3});
        else if(pressed_key==2) //LEFT
            reverse_way.push_back({reverse_cont, 4});
        else if(pressed_key==3) //DOWN
            reverse_way.push_back({reverse_cont, 1});
        else if(pressed_key==4) //RIGHT
            reverse_way.push_back({reverse_cont, 2});

        reverse_cont=0;
        }
}
*/