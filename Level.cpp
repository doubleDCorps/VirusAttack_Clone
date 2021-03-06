#include "Level.h"

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

 void Level::setDifficulty(unsigned difficulty)
{
    numBosses = 1;
}

 void Level::entities_init()
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

 void Level::spawn()
{   
    for(unsigned i{2}; i < entities.size(); i++)
     if(!entities[i]->isAlive())
    {
        entities[i]->setAlive(true);
        entities[i]->setCord_x(entities[1]->getCord_x());
        entities[i]->setCord_y(entities[1]->getCord_y());
    }
}

 int Level::getArea(const GameList& GL) const
{
    if(GL.size() < 2) return -1;

    int area{};
    for(auto it{ GL.begin() }; it!=GL.end(); ++it)
        area += ( it->x() + GL.successor(it)->x() ) *
                ( GL.successor(it)->y() - it->y() );
    area/=2;

    return abs(area);
}

 Level::Level(unsigned difficulty, ALLEGRO_DISPLAY_MODE settings, ALLEGRO_TIMER* tim, Sounds* sounds): background(), areaBar(), livesBar(), respawnBar()
{
    defPerInit(settings); 
    setDifficulty(difficulty);
    timer = tim;
    this->sounds=sounds;
    background.setMyBitmap("resources/level/background.png");
}

 void Level::loop()
{
    border = defPerimeter;
    //Boss* boss; //potrebbe?
    
    entities_init();
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

    const auto& percentArea     = [&](){ return getArea(border)/2500; };
    const auto& areaCondition   = [&](){ return percentArea() <= 20; };
    const auto& playerCondition = [&](){ return !entities[0]->isAlive(); };

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
            STOP = //areaCondition() or 
            playerCondition();
            if(STOP)
                cout << percentArea();
            /*
                trace routines
            */
            auto center = player->getData().center();
            if(!player->isSafe())
                trace_condition = true;
            
            trace.pushPoint(center);
             if(trace.size() > 1)
            {
//              questo dovrebbe sempre recuperare il punto corrispondente sul bordo
//              in quali casi fallisce? Se fallisce
                auto temp = border.nearestLine(trace.front());
                trace.front() = trace.front().projection(temp.first, temp.second);
            }
//          se il player si trova sul bordo e l'inizio e la fine del tracciato non coincidono
             if(border.onEdge(center).first and trace.back() != trace.front())
            {
//              questo dovrebbe sempre recuperare il punto corrispondente alla fine del
//              tracciato, proiettato sul bordo
//              in quali casi fallisce? Se fallisce
                auto temp = border.nearestLine(trace.back());
                trace.back() = trace.back().projection(temp.first, temp.second);
/*                    
                 if(trace_condition)
                {   
                    trace_condition = false;

                    if(trace.back() == trace.front())
                        trace.pop_back();
                    if(trace.size() >= 2) {
                    cout << "trace" << trace << endl; 
                    cout << "border" << border << endl;
                    
                    //prima partizione
                    GameList first = trace;
                    //seconda partizione
                    GameList second = trace;

                    //si inizializzano i due punti di inserimento
                    perimeter::iterator startFirst, startSecond;
                    perimeter::iterator iFirst, iSecond;
                    perimeter::iterator endFirst, endSecond;
                    short condizione = 0;
                    for(auto it{border.begin()}; it!=border.end(); ++it) {
                        if(trace.front().collinear(*it, *border.successor(it))) {
                            if(condizione == 0) condizione = 1;
                            if(condizione == 1) {
                                endSecond = border.successor(it);
                                endFirst = it; 
                            } else if(condizione == 2) {
                                endSecond = it;
                                endFirst = border.successor(it);      
                            }
                        }
                        if(trace.back().collinear(*it, *border.successor(it))) {
                            if(condizione == 0) condizione = 2;
                            if(condizione == 1) {
                                startSecond = iSecond = it;
                                startFirst = iFirst = border.successor(it);
                            } else if(condizione == 2) {
                                startSecond = iSecond = border.successor(it);
                                startFirst = iFirst = it;
                            }
                        }
                    }

                    //VA VERIFICATO CHE DUE PUNTI NON SIANO SULLA STESSA PARTIZIONE
                    bool specialCase1 = (startFirst == endSecond);
                    bool specialCase2 = (startSecond == endFirst);

                    //si inseriscono gli elementi nelle due liste finchè non sono finiti
                    if(!specialCase1) { 
                    while(endSecond != iFirst) {
                        
                        assert(iFirst != border.end());
                        
                        first.pushPoint(*iFirst);
                        if(condizione == 2)
                            iFirst = border.predecessor(iFirst);
                        else
                            iFirst = border.successor(iFirst);                            
                    }
                    }else if(!specialCase2) {
                    while(endFirst != iSecond) {
                        
                        assert(iSecond != border.end()); 
                        
                        second.pushPoint(*iSecond);
                        if(condizione == 2) 
                            iSecond = border.successor(iSecond);
                        else                  
                            iSecond = border.predecessor(iSecond);
                    }
                    } else if(specialCase1) {
                        
                        if(condizione == 2) {
                            for(auto it{iSecond}; border.successor(it) != iSecond; it = border.successor(it))
                                second.pushPoint(*it);
                            second.pushPoint(*endSecond);
                        } else {
                            for(auto it{iFirst}; border.successor(it) != iFirst; it = border.successor(it))
                                first.pushPoint(*it);
                            first.pushPoint(*endFirst);
                        }
                    } 

                    cout << "first"  << first  << endl;
                    cout << "second" << second << endl;
                    
                    if(first.c_inside(entities[1]->getData()))  border = first;
                    else                                        border = second;
                    }

                    player->clearReverse();
                }
 */
                if(trace_condition) {

                    if(trace.back() == trace.front())
                        trace.pop_back();

                    if(trace.size() >= 2) {

                        trace_condition = false;

                        GameList fst = trace;
                        GameList snd = trace;
                        
                        vector<bool> flags(border.size(), false);
                        
                        bool kill   = false;
                        bool chosen = false;
                        while(!kill) {
                            
                            bool hasDoneSomething = false;
                            for(auto it{border.begin()}; it != border.end(); ++it) {
                                //se non è stato inserito
                                if(!flags[distance(border.begin(), it)]
                                //se non è un punto inutile
                                and !it->collinear(trace.back(), *(++trace.rbegin()))) {  
                                    
                                    if(snd.is_closed() and fst.is_closed()) {
                                    
                                        if(fst.o_inside(entities[1]->getData()) 
                                        and fst.o_inside(entities[1]->getData().projection())) {
                                            
                                            border = fst;
                                            cout << "fst choice" << std::endl << std::endl;
                                            kill   = true;
                                            chosen = true;
                                            break;

                                        } else if(snd.o_inside(entities[1]->getData()) 
                                                and snd.o_inside(entities[1]->getData().projection())) {
                                            
                                            border = snd;
                                            cout << "snd choice" << std::endl << std::endl;
                                            kill   = true;
                                            chosen = true;
                                            break;

                                        } else if(snd.pushPoint(*it)) {
                                            
                                            flags[distance(border.begin(), it)] = true;
                                            hasDoneSomething = true;
                                        
                                        }
                                    
                                    } else if(!fst.is_closed() and fst.pushPoint(*it)) {
                                    
                                        flags[distance(border.begin(), it)] = true;
                                        hasDoneSomething = true;

                                    } else if(!snd.is_closed() and snd.pushPoint(*it)) {
                                    
                                        flags[distance(border.begin(), it)] = true;
                                        hasDoneSomething = true;
                                    
                                    }
                                }
                            }

                            if(!hasDoneSomething) kill = true;
                        /* 
                            cout << "old:" << border << std::endl;
                            cout << "fst:" << fst    << std::endl;
                            cout << "snd:" << snd    << std::endl;
                            for(auto el : flags) cout << " " << el; 
                            cout << std::endl;
                        */                        
                        }
                        
                        cout << "old:" << border << std::endl;
                        cout << "fst:" << fst    << std::endl;
                        cout << "snd:" << snd    << std::endl;
                        
                        if(!chosen) {
                            if(fst.is_closed() 
                            and fst.o_inside(entities[1]->getData())
                            and fst.o_inside(entities[1]->getData().projection())) {
                            
                                border = fst;
                                cout << "fst choice" << std::endl << std::endl;
                        
                            } else if(snd.is_closed() 
                                   and snd.o_inside(entities[1]->getData())
                                   and snd.o_inside(entities[1]->getData().projection())) {
                            
                                border = snd;
                                cout << "snd choice" << std::endl << std::endl;

                            }
                        }

                        player->clearReverse();
                    }
                }

                trace.clear();
            }
            /*
                routines, pt. 2
            */
            if(!player->isSafe())
            for(unsigned i=1; i < entities.size(); ++i)
             if(entities[i]->isAlive() and trace.collides(entities[i]->getData()) and !debug_mode)
            {
                player->setCenter(border.nearestPoint(player->getData()));
                player->setAlive(player->isAlive() - 1); //REDO THIS ASAP
                trace.clear();
                player->clearReverse();
                break;
            }
            /*
                routines, pt. 1
            */
            for(unsigned i=0; i < entities.size(); ++i)
             if(i < 1 or //manca la condizione che impedisce la collisione con la traccia 
                i > numBosses or (i >= 1 and i <= numBosses and (spawn_time<300 or spawn_time>420) and (spawn_time<2101 or spawn_time>2221)))
                entities[i]->update(border);
            /*
                boss routines
            */
            ++spawn_time;
             if((spawn_time == 360 or spawn_time == 2161) and entities[1]->isAlive())
            {
                spawn();
                if(spawn_time != 360)
                    spawn_time = 360;
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
            background.draw_scaled();
            updateLateralPane(spawn_time);

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

    sounds->stopInLevelTheme();

     for(int i{}; i<entities.size(); ++i)
    {
        al_destroy_bitmap(entities[i]->getBitmap());
        delete entities[i];
    }

    al_destroy_event_queue(coda_eventi);
    border.clear();
    trace.clear();
    //non so se ci voglia, quando testeremo partite successive vedremo
    //al_stop_timer(timer);
}

void Level::updateLateralPane(int spawn_time) {
//    /* PANNELLO AREA OCCUPATA */
//    if(perc_area>=0 and perc_area<25)
//        areaBar.setMyBitmap("resources/level/lateralPane/areaBar/0.png");
//    else if(perc_area>=25 and perc_area<50)
//        areaBar.setMyBitmap("resources/level/lateralPane/areaBar/25.png");
//    else if(perc_area>=50 and perc_area<75)
//        areaBar.setMyBitmap("resources/level/lateralPane/areaBar/50.png");
//    else if(perc_area>=75)
//        areaBar.setMyBitmap("resources/level/lateralPane/areaBar/75.png");
//
//
//    /* PANNELLO VITE RIMANENTI */
//    if(lives == 3)
//        areaBar.setMyBitmap("resources/level/lateralPane/livesBar/3.png");
//    else if(lives == 2)
//        areaBar.setMyBitmap("resources/level/lateralPane/livesBar/2.png");
//    else if(lives == 1)
//        areaBar.setMyBitmap("resources/level/lateralPane/livesBar/1.png");
//    else if(lives == 0)
//        areaBar.setMyBitmap("resources/level/lateralPane/livesBar/0.png");
//
//
//    /* PANNELLO TEMPO DI RESPAWN DEI MOB */
//    if(spawn_time >= 360 and spawn_time < 585)   
//        areaBar.setMyBitmap("resources/level/lateralPane/respawnBar/7.png");
//    else if(spawn_time >= 585 and spawn_time < 810)   
//        areaBar.setMyBitmap("resources/level/lateralPane/respawnBar/6.png");
//    else if(spawn_time >= 810 and spawn_time < 1035)   
//        areaBar.setMyBitmap("resources/level/lateralPane/respawnBar/5.png");
//    else if(spawn_time >= 1035 and spawn_time < 1260)   
//        areaBar.setMyBitmap("resources/level/lateralPane/respawnBar/4.png");
//    else if(spawn_time >= 1260 and spawn_time < 1485)   
//        areaBar.setMyBitmap("resources/level/lateralPane/respawnBar/3.png");
//    else if(spawn_time >= 1485 and spawn_time < 1710)   
//        areaBar.setMyBitmap("resources/level/lateralPane/respawnBar/2.png");
//    else if(spawn_time >= 1710 and spawn_time < 1935)   
//        areaBar.setMyBitmap("resources/level/lateralPane/respawnBar/1.png");
//    else if(spawn_time >= 1935)   
//        areaBar.setMyBitmap("resources/level/lateralPane/respawnBar/0.png");
//
//    areaBar.draw();
//    livesBar.draw();
//    respawnBar.draw();    
}