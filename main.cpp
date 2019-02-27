/*
    Il display verrà realizzato sovrapponendo una sequenza di bitmap con alpha,
    ciascuna legata ad oggetti con una determinata frequenza di aggiornamento:
    la bitmap legata allo sfondo necessiterà di meno aggiornamenti rispetto alla bitmap
    che contiene i nemici ad esempio. Ogni "canale" avrà la sua gestione separata dei dati
    e delle immagini, quindi ogni "canale" sarà responsabile della sua scrittura sul display.
*/
/*
    Entity è trascurabile come classe: se usata, servirà solo ad evitare di riscrivere codice in Player
    ed Enemy, ma non verrà nè istanziata nè usata come tipo del puntatore per i polimorfismi.
    Potenzialmente, Entity conterrebbe:
        - coordinate x, y
        - dimensione hitbox d (ipotizziamo tutte le enità avranno hitbox quadrata)
        - velocità x, y
        - bitmap da stampare
*/
#include"polygon.h"

void menu();
void level();

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
    
    ALLEGRO_EVENT_QUEUE *coda_eventi = al_create_event_queue();
     if(!coda_eventi) { al_destroy_display(display); al_destroy_timer(timer); return -1; }

    Minion minions[12];

    //

    Boss boss;
    Player player;
    GameArea poly(/**/, /**/, /**/, boss, player);

    //

    bool redraw {true};
    bool STOP {false};

    al_register_event_source(coda_eventi, al_get_display_event_source(display));
    al_register_event_source(coda_eventi, al_get_timer_event_source(timer));
    al_register_event_source(coda_eventi, al_get_keyboard_event_source());
    al_start_timer(timer);

    int w;
    int h;

     while(!STOP)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(coda_eventi, &ev);

        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            STOP = true;
         else if(ev.type == ALLEGRO_EVENT_TIMER)
        {
             for(unsigned i{}; i < 12; ++i)
            {
                poly.update();
                 if(poly.getArea()*100/(w*h) <= 30)
                {
                    //hai vinto il livello!
                }
                //if: player.getLifes()==0 do: //hai perso!
                 if(!poly.insideBorder(minions[i].getCord_x(), minions[i].getCord_y(), minions[i].getWidth_hitbox()) )
                {
                    minions[i].setAlive(false);
                }
                //if: minions.size()<12    do: boss.spawn()
                 if(minions[i].getAlive() )
                {
                    minions[i].update(poly.hitsBorder(minions[i].getCord_x(), minions[i].getCord_y(), minions[i].getWidth_hitbox(), minions[i].getHeight_hitbox(), minions[i].getVelocity_x(), minions[i].getVelocity_y()) );
                }
            }

            redraw = true;
        }
         if(redraw && al_is_event_queue_empty(coda_eventi) )
        {
            redraw = false;

            al_clear_to_color(al_map_rgb(255, 255, 255) );
            for(unsigned i{}; i < 12; ++i)
             if(minions[i].getAlive() )
                minions[i].print(al_get_backbuffer() );
            
            al_flip_display();
        }
    }

    for(unsigned i{}; i < 12; ++i)
        cout<<"("<<minions[i].getCord_y()<<", "<<minions[i].getCord_x()<<") "
        <<poly.is_inside(minions[i].getCord_x(), minions[i].getCord_y())<<' '
        <<minions[i].getAlive()<<endl;
    
    for(auto i : bouncer)
        al_destroy_bitmap(i);

    al_destroy_event_queue(coda_eventi);
    al_destroy_display(display);
    al_destroy_timer(timer);

    return 0;
}