#include "Level.h"

 void entities_init(vector<Entity*>& entities)
{
    entities.push_back(new Player{135, 150, al_create_bitmap(30,30)});                                        //PLAYER
    entities.push_back(new Enemy{float(800)/2-30/2, float(600)/2-30/2, -4.0, -4.0, al_create_bitmap(30,30)}); //BOSS 
    
    for(unsigned i=0; i<12; ++i)
        entities.push_back(new Enemy{float(800)/2-30/2, float(600)/2-30/2, 0, 0, al_create_bitmap(30,30)});   //MINIONS

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
        al_clear_to_color(al_map_rgb(255, 255, 255));
    }
}

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

 void Level::setDifficulty(unsigned difficulty)
{
    numBosses = 1;
}

 void Level::defPerInit(ALLEGRO_DISPLAY_MODE d)
{
    float x{ d.width/2  - 500/2.0f };
    float y{ d.height/2 - 500/2.0f };

    defPerimeter = {
        {x      , y      },
        {x + 500, y      },
        {x + 500, y + 500},
        {x      , y + 500}
    };
}

 Level::Level(unsigned difficulty, ALLEGRO_DISPLAY_MODE settings, ALLEGRO_TIMER* tim)
{
    defPerInit(settings); 
    setDifficulty(difficulty);
    timer = tim;
}

 int Level::getArea(const GameList& GL) const
{
    if(GL.empty()) return -1;

    int area{};
    for(auto it{ GL.begin() }; it!=GL.end(); ++it)
        area += ( it->x() + GL.successor(it)->x() )*
                ( GL.successor(it)->y() - it->y() );
    area/=2;

    return area;
}

 void Level::loop()
{
    GameList border(defPerimeter);
    GameList trace;
    vector<Entity*> entities;
    Player* player;
    //Boss* boss; //potrebbe?
    
    entities_init(entities);
    player = dynamic_cast<Player*>(entities[0]);
    
    ALLEGRO_EVENT_QUEUE *coda_eventi = al_create_event_queue();
     if(!coda_eventi) 
    {
        al_destroy_display(al_get_current_display());
        al_destroy_timer(timer); 
        return; 
    }

    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
    al_clear_to_color(al_map_rgb(255, 255, 255));

    bool redraw {true};
    bool STOP {false};
    int spawn_time{};
    bool trace_condition{false};

    al_register_event_source(coda_eventi, al_get_display_event_source(al_get_current_display()));
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
            /*
                condizioni di uscita
            */
            STOP = (getArea(border)/2500 <= 25) or (!entities[0]->isAlive()); 
            /*
                routines, pt. 2
            */
            if(!player->isSafe())
            for(unsigned i=1; i < entities.size(); ++i)
             if(entities[i]->isAlive() and trace.collides(entities[i]->getData()))
            {
                player->setCenter(border.nearestPoint(player->getData()));
                //player->setAlive(player->isAlive() - 1);
                cout << trace << endl;
                trace.clear();
                break;
            }
            /*
                routines, pt. 1
            */
            for(unsigned i=0; i < entities.size(); ++i)
             if(i < 1 or //manca la condizione che impedisce la collisione con la traccia 
                i > numBosses or 
               (i >= 1 and i <= numBosses and (spawn_time<300 or spawn_time>420) and (spawn_time<2101 or spawn_time>2221)))
                entities[i]->update(border);
            /*
                boss routines
            */
            ++spawn_time;
             if((spawn_time==360 or spawn_time==2161) and entities[1]->isAlive())
            {
                spawn(entities);
                if(spawn_time!=360)
                    spawn_time=360;
            }
            /*
                trace routines
            */
            auto center = player->getData().center();
            if(!player->isSafe())
                trace_condition = true;
            
            trace.pushPoint(center);
             if(trace.size() > 1)
            {
                auto temp = border.nearestLine(trace.front());
                trace.front() = trace.front().projection(temp.first, temp.second);
            }
             if(border.onEdge(trace.back()).first and trace.back() != trace.front())
            {
                 if(trace_condition)
                {   
                    trace_condition = false;

                    auto temp = border.nearestLine(trace.back());
                    trace.back() = trace.back().projection(temp.first, temp.second);
                    
                    cout << trace << endl; 
                    
                    //algoritmo del taglio
                }

                trace.clear();
            }
            /*
                abilita il redraw
            */
            redraw = true;
        }
         else if(ev.type == ALLEGRO_EVENT_KEY_DOWN and ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            STOP = true;
         else if(ev.type == ALLEGRO_EVENT_KEY_DOWN or ev.type == ALLEGRO_EVENT_KEY_UP)
            player->setKey(ev.keyboard.keycode, ev.type);
        /*
            QUESTO DEVE DIVENTARE GESTIONE GRAFICA?
        */
         if(redraw and al_is_event_queue_empty(coda_eventi))
        {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            //redo this print
            al_draw_bitmap(entities[0]->getBitmap(), entities[0]->getCord_x(), entities[0]->getCord_y(), 0);
            //ok
            trace.u_print(al_get_backbuffer(al_get_current_display()));
            //redo this prints
            for(unsigned i = numBosses; i < entities.size(); ++i)
             if(entities[i]->isAlive())
                al_draw_bitmap(entities[i]->getBitmap(), entities[i]->getCord_x(), entities[i]->getCord_y(), 0);
            //ok            
            border.l_print(al_get_backbuffer(al_get_current_display()));
            //ok
            al_flip_display();
        }
    }
    /*
        deallocazione delle risorse locali
    */
     for(int i{}; i<entities.size(); ++i)
    {
        al_destroy_bitmap(entities[i]->getBitmap());
        delete entities[i];
    }
    al_destroy_event_queue(coda_eventi);
    //non so se ci voglia, quando testeremo partite successive vedremo
    al_stop_timer(timer);
}

/*
     if(trace.size() > 1 and 
        trace.push(Player->getData().c[0], Player->getData().c[1]) and
        border.is_adj(trace.back().first, trace.back().second) )
    {
        if(insideTrace(Boss->getData()) )
            for(auto& i : border)
             if(trace.inside(i.first, i.second))  trace.push(i.first, i.second);
        else
            for(auto& i : border)
             if(!trace.inside(i.first, i.second)) trace.push(i.first, i.second);

        border=trace;
        trace.clear();
        return true;
    }
     else if(!trace.empty() or
             (trace.empty() and 
              border.is_adj(Player->getData().c[0], Player->getData().c[1])))
        trace.push( Player->getData().c[0], Player->getData().c[1] );
        
    return false;
*/