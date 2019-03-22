#include "Level.h"

/*
    Se il punto p rispetta le condizioni di ordinamento, viene aggiunto alla fine o all'inizio
    della lista e viene restituito true; altrimenti viene restituito false.
*/
 bool GameList::push(int x, int y)
{
    if(empty())                                                                          push_back( {x, y} );
    else if(size() == 1 && (x == back().first || y == back().second) )                   push_back( {x, y} );  //senza questo crasha
    //else if(size() == 1 && (x == front().first || y == front().second) )                 push_front( {x, y} );
    else if(size() >= 2 && x == back().first && back().second == (++rbegin())->second)   push_back( {x, y} );  //senza questo crasha
    else if(size() >= 2 && x == back().first && back().first == (++rbegin())->first)     back() = {x, y};      //chissu cujunija
    else if(size() >= 2 && y == back().second && back().first == (++rbegin())->first)    push_back( {x, y} );  //chissu cujunija
    else if(size() >= 2 && y == back().second && back().second == (++rbegin())->second)  back() = {x, y};      //chissu cujunija
    //else if(size() >= 2 && x == front().first && front().second == (++begin())->second)  push_front( {x, y} );
    //else if(size() >= 2 && x == front().first && front().first == (++begin())->first)    front() = {x, y};
    //else if(size() >= 2 && y == front().second && front().first == (++begin())->first)   push_front( {x, y} );
    //else if(size() >= 2 && y == front().second && front().second == (++begin())->second) front() = {x, y};
    //else        return false;
    return true;
}
/*
    Data una coppia di coordinate p, verifica se esiste una coppia di elementi della lista tali che il punto ricada nel
    segmento che li connette. 
*/
 bool GameList::is_adj(int x, int y, int w, int h) const
{
    for(auto it{ begin() }; it != end(); ++it)
     if(hitbox(x, y, x+w, y+h, it->first, it->second, successor(it)->first, successor(it)->second) )
        return true;
    return false;
}
/*
    Per un dato oggetto definito dalla sua hitbox (primi quattro parametri) e dalla sua
    velocità (ultimi due parametri) viene verificata per ogni coppia di punti nella lista
    (ciascuna rappresentante un segmento) se si verifica una collisione fra la hitbox e il
    segmento; se la collisione avviene, si restituisce una variabile che riconosce lungo quale
    asse è avvenuto la collisione (X o Y), altrimenti lo stato "none" (euqivalente a false).
*/
 AXIS GameList::hits(int x1, int y1, int w1, int h1, int vx, int vy) const
{
    if(empty()) return none;

     for(auto it{ begin() }; it != end(); ++it)
    {
        // coordinate degli estremi del segmento
        int x2{ it->first }, x3{ successor(it)->first };
        int y2{ it->second }, y3{ successor(it)->second };

        // il segmento è orizzontale
        // la velocità e la distanza hanno segno concorde (con uno scarto di sicurezza)
        // verifica se avviene una collisione fra le hitbox

        if(x2 != x3 &&
          (vy>=0 && y2-y1-h1/2 > 0 && hitbox(x1, y1, x1+w1, y1+h1, x2, y2, x3, y2+14))
        ||(vy<=0 && y2-y1-h1/2 < 0 && hitbox(x1, y1, x1+w1, y1+h1, x2, y2, x3, y2-14)))
            return Y;
        
        // il segmento è verticale
        // la velocità e la distanza hanno segno concorde (con uno scarto di sicurezza)
        // verifica se avviene una collisione fra le hitbox

        if(y2 != y3 &&                                                   
          (vx>=0 && x2-x1-w1/2 > 0 && hitbox(x1, y1, x1+w1, y1+h1, x2, y2, x2+14, y3))
        ||(vx<=0 && x2-x1-w1/2 < 0 && hitbox(x1, y1, x1+w1, y1+h1, x2, y2, x2-14, y3)))          
            return X;
    }
    
    return none;
}
/*
    Data una hitbox associata ad un oggetto, si applica lo stesso algoritmo
    usato per il calcolo dell'area per decidere se l'oggetto si trova all'esterno dell'area definita 
    dalla lista: si verifica la presenza o meno di collisioni fra la hitbox e l'area descritta da ogni
    coppia di punti e per quelle in cui la collisione avviene si considera adeguatamente se l'area
    possiede segno "negativo" o "positivo", associando le aree positive a quelle interne al poligono
    e quelle negative alle aree esterne al poligono.
*/
 bool GameList::inside(int x, int y, int w, int h) const
{
    if(empty()) return true;

    int cont{};

     for(auto it{ begin() }; it != end(); ++it)
    {
        int y1{ it->second };
        int y2{ successor(it)->second };

         if(y2-y1!=0 && hitbox(x, y, x+w, y+h, 0, min(y1, y2), it->first, max(y1, y2)) )
        {
            if(y2-y1 < 0) --cont;
            if(y2-y1 > 0) ++cont;
        }
    }

    return !hits(x, y, w, h, 0, 0) && cont <= 0 ? false : true;
}
/*
    Dato un bitmap da trattare come buffer temporaneo, viene targettato per il disegno
    e viene poi successivamente disegnato il poligono rappresentato dalla lista manipolando
    l'immagine che fa da "font" per l'intero bordo.
*/
 void GameList::print(ALLEGRO_BITMAP* buffer) const
{
    if(empty()) return;

     if(buffer!=nullptr)
    {
        al_set_target_bitmap(buffer);
        for(auto it{ begin() }; it != end(); ++it)
         if(successor(it)!=end() || (successor(it)==end() &&
            (it->first == successor(it)->first || it->second == successor(it)->second)))    
        {
            if(it->first == successor(it)->first)
                al_draw_line(it->first, min(it->second, successor(it)->second)-5,
                             it->first, max(it->second, successor(it)->second)+5,
                             al_map_rgb(0, 0, 0), 10);
            else
                al_draw_line(min(it->first-5, successor(it)->first), it->second,
                             max(it->first+5, successor(it)->first), it->second, 
                             al_map_rgb(0, 0, 0), 10);
        }
    }
}
/*
    Viene calcolata l'area utilizzando il seguente algoritmo:
    dato un poligono chiuso definito da una lista di coordinate (x, y) concatenate,
    dove con concatenate si intende che almeno un elemento di ogni coppia è uguale a quello
    del successivo e che ogni coppia ha l'elemento opposto in comune rispetto alla precedente,
    si sommano le aree rettangolari ottenute da ogni coppia di coordinate concantenate, sfruttando
    la differenza fra le y per determinare il "segno" che indichi se l'area appartiene
    alla figura descritta (positiva) o altrimenti (negatva).
    La somma viene poi dimezzata per ottenere il valore numerico corrispondente all'area del poligono.
*/
 int Level::getArea() const
{
    if(border.empty()) return -1;

    int area{};
    for(auto it{ border.begin() }; it!=border.end(); ++it)
        area += (it->first+border.successor(it)->first)*
                (border.successor(it)->second-it->second);
    area/=2;

    return area;
}
/*
    Aggiorna automaticamente la traccia all'ultima posizione nota del giocatore;
    se l'ultimo elemento della traccia è allineato con border, si aggiorna la lista border
    in base alla posizione del Boss.
*///SE QUESTO DIVENTASSE TUTTO IL METODO LEVEL?
 int Level::operator() ()
{
    ALLEGRO_TIMER *timer = al_create_timer(1.0/disp_data.refresh_rate);
     if(!timer)
        return -1;

    ALLEGRO_EVENT_QUEUE *coda_eventi = al_create_event_queue();
     if(!coda_eventi) 
    {
        al_destroy_display(display);
        al_destroy_timer(timer); 
        return -1; 
    }

    vector<Entity*> entities;
    entities_init(entities);
    
    al_set_target_bitmap(al_get_backbuffer(display));
    al_clear_to_color(al_map_rgb(255, 255, 255));

    Level poly(defPerimeter, entities[0], entities[1]);

    bool redraw {true};
    bool STOP {false};
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
            STOP = (poly.getArea()/2500 <= 30) or (!entities[0]->isAlive());
            //STOP = !poly.insideBorder(entities[0]->getData()); //SOLO TEMPORANEO

            //player routines
            AXIS collision = hitsBorder(entites[0]->getData()) );

            //per ogni lato:
                //se si verifica la collisione e la proiezione è fuori dal bordo, 0
                //altrimenti:
                    //bisogna gestire separatamente inside() con e esclusi i bordi
            bool n1;
             if(collision != none)
            {
                n1 = insideBorder(normale1(entities[0]->getCord_x(), entities[0]->getCord_y()));
                if(!n1) entities[0].directions[0] = 0;
                n1 = insideBorder(normale2(entities[0]->getCord_x(), entities[0]->getCord_y()));
                if(!n1) entities[0].directions[1] = 0;
                n1 = insideBorder(normale3(entities[0]->getCord_x(), entities[0]->getCord_y()));
                if(!n1) entities[0].directions[2] = 0;
                n1 = insideBorder(normale4(entities[0]->getCord_x(), entities[0]->getCord_y()));
                if(!n1) entities[0].directions[3] = 0;
            }
            
            entities[0]->update(space ? key : 0);
            //poly.hitsBorder(entities[0]->getData())

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
/*
     if(trace.size() > 1 && 
        trace.push(Player->getData().c[0], Player->getData().c[1]) &&
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
     else if(!trace.empty() ||
             (trace.empty() && 
              border.is_adj(Player->getData().c[0], Player->getData().c[1])))
        trace.push( Player->getData().c[0], Player->getData().c[1] );

    return false;
*/