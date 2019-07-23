#include"Level.h"

Level::Level(ALLEGRO_TIMER* t): timer(t), borders(), entities(), boss() {}

void Level::respawnPlayer() {
    
    int pivot = -1;
    vector<int> N = Root::makeNeighborhood(player->pos());
    
    int arr[] = {1, -1, Root::getDim(), -Root::getDim()};
    
    for(int i = 0; i < Root::getDim(); ++i) {
     
     for(const auto& it : N) 
      for(int j = 0; j < 4; ++j)
        if(getObj(it + arr[j]*i) == Type::BORDER)
            pivot = it + arr[j]*i;

        if(pivot != -1) {
            player->reposition(pivot);
            player->respawn();
            break;
        }
    }
}

void Level::spawnEnemy(const Enemy *const E) {
    
    for(const auto& it : entities)
     if(getBody(it.second->pos()) == Body::ENEMY and !it.second->isActive()) {
        
        vector<int> N = Root::makeNeighborhood(E->pos());
        
        for(const auto& val : N)
         if(getObj(val) == Type::VOID and getBody(val) == Body::NONE) {
             
            it.second->reposition(val);
            it.second->respawn();
            break;
        
        }
        
        break;
    }
}

void Level::updateMap() {
    
    map<int, Entity*> newEntities;
    map<int, GameObject*> newBorders;
    //stack per il floodfill
    queue<int> positions;
    set<int> voidAnalyzed;
    //si inizializzano sia la stack che la nuova map
    positions.push(boss[0]->pos());
    newEntities[boss[0]->pos()] = boss[0];
    //algoritmo
    while(!positions.empty()) {
        //fissata una posizione
        int next = positions.front();
        positions.pop();
        //se quella posizione non è un bound
        if(getObj(next) != Type::BORDER and getObj(next) != Type::TRACE)
        //si generano tutte le posizioni vicine valide
        for(const auto& element : Root::makeNeighborhood(next))
        //se non è già stato analizzato
        if(newEntities.find(element) == newEntities.end() and newBorders.find(element) == newBorders.end() and voidAnalyzed.find(element) == voidAnalyzed.end()) {
            //si accoda il nuovo valore
            positions.push(element);
            //se il tile non era vuoto, lo si aggiunge
            if(isBody(next))
                newEntities[element] = entities.find(element)->second;
            //altrimenti lo si segna per evitare il reinserimento
            else if(isObj(next))
                newBorders[element] = newBorders.find(element)->second;
            else
                voidAnalyzed.insert(element);
        }
    }
}

void Level::initMap() {
    
    Hitbox* temp = nullptr;
    for(int i = 0; i < Root::getDim(); ++i) {

        float t = i;

        temp = new Hitbox(t*GameObject::getSize(), 0, 1, 1);
        temp->setImage(temp->getW()*GameObject::getSize(), temp->getH()*GameObject::getSize(), al_map_rgb(255, 255, 255));
        allocation.push_back(temp);
        borders[temp->pos()] = temp;

        temp = new Hitbox(0, t*GameObject::getSize(), 1, 1);
        temp->setImage(temp->getW()*GameObject::getSize(), temp->getH()*GameObject::getSize(), al_map_rgb(255, 255, 255));
        allocation.push_back(temp);
        borders[temp->pos()] = temp;

        temp = new Hitbox(t*GameObject::getSize(), (Root::getDim()-1)*GameObject::getSize(), 1, 1);
        temp->setImage(temp->getW()*GameObject::getSize(), temp->getH()*GameObject::getSize(), al_map_rgb(255, 255, 255));
        allocation.push_back(temp);
        borders[temp->pos()] = temp;

        temp = new Hitbox((Root::getDim()-1)*GameObject::getSize(), t*GameObject::getSize(), 1, 1);
        temp->setImage(temp->getW()*GameObject::getSize(), temp->getH()*GameObject::getSize(), al_map_rgb(255, 255, 255));
        allocation.push_back(temp);
        borders[temp->pos()] = temp;
    }

    //allocazione player
    player = new Player(0, 0, 
                        3, 3,
                        4.0f, 4.0f);
    player->setImage(player->getW()*GameObject::getSize(), player->getH()*GameObject::getSize(), al_map_rgb(100, 200, 150));
    allocation.push_back(player);
    entities[player->pos()] = player;
    respawnPlayer();
    
    //allocazione boss
    boss.push_back(new Enemy(( (Root::getDim()-1)*GameObject::getSize() )/2.0f, 
                             ( (Root::getDim()-1)*GameObject::getSize() )/2.0f, 
                             3, 3, 
                             -4.0, -4.0, 
                             true));
    boss[0]->setImage(boss[0]->getW()*GameObject::getSize(), boss[0]->getH()*GameObject::getSize(), al_map_rgb(150, 200, 100));
    allocation.push_back(boss[0]);
    entities[boss[0]->pos()] = boss[0];

    //allocazione nemici
    int v = boss[0]->pos();
    for(const auto& value : Root::makeExtendedNeighborhood(v)) {
        
        int dx = v/Root::getDim() - value/Root::getDim();
        int dy = v%Root::getDim() - value%Root::getDim();

        Entity* temp = new Enemy(0, 0, 3, 3, dx, dy, false);
        temp->reposition(value);
        temp->setImage(temp->getW()*GameObject::getSize(), temp->getH()*GameObject::getSize(), al_map_rgb(200, 100, 150));
        allocation.push_back(temp);
        entities[temp->pos()] = temp;
    }
}

void Level::delMap() {

    for(const auto& val : allocation)
        delete val;
}

const GameObject *const Level::borderGet(int val) const {
    
    return isObj(val) ? borders.at(val) : nullptr;
}

const GameObject *const Level::entityGet(int val) const {
    
    return isBody(val) ? entities.at(val) : nullptr;
}

Level::Body Level::getBody(int val) const {

    if(entities.find(val) == entities.end())                return Body::NONE;
    
    else if(player == entities.find(val)->second)           return Body::PLAYER;
    
    else if(((Enemy*)entities.find(val)->second)->isBoss()) return Body::BOSS;
    
    else                                                    return Body::ENEMY;
}

bool Level::isBody(int val) const {
    
    return getBody(val) != Body::NONE;
}

Level::Type Level::getObj(int val) const {
    //se in val non è salvato alcun oggetto
    if(borders.find(val) == borders.end())                  return Type::VOID;
    //se il player ha un riferimento all'obj (quindi è nella trace)
    else if(player->contains(*(borders.find(val)->second))) return Type::TRACE;
    //ultima possibilità: è parte del bordo
    else                                                    return Type::BORDER;
}

bool Level::isObj(int val) const {
    
    return getObj(val) != Type::VOID;
}

bool Level::canSpawn() const {
    
    for(const auto& it : entities)
     if(getBody(it.second->pos()) == Body::ENEMY and !it.second->isActive())
        return true;
    return false;
}

bool Level::checkForBounds(int a) const {
    
    return checkRange(a, 0, Root::getDim(), true, false);
}

int Level::getArea() const {

    int retval = 0;
    //stack per il floodfill
    queue<int> positions;
    set<int> analyzed;
    //si inizializzano sia la stack che la nuova map
    positions.push(boss[0]->pos());
    analyzed.insert(boss[0]->pos());
    
    cout << "\n Level::getArea() preps complete " << boss[0]->pos();
    //algoritmo
    while(!positions.empty()) {
        //fissata una posizione
        int next = positions.front();
        analyzed.insert(next);
        positions.pop();
        
        retval += 1;
        //cout << next << "-" << retval << "\n";
        //si generano tutte le posizioni vicine valide
        for(const auto& element : Root::makeNeighborhood(next))
        //se non è già stato analizzato
         if(analyzed.find(element) == analyzed.end() and getObj(element) != Type::BORDER) {
            positions.push(element);
        }
    }
    cout << "\n Level::getArea() alg complete " 
    << (retval * 100.0f) / (float)Root::getDim()*Root::getDim() << endl;
    
    return 
        (retval * 100.0f) / 
        (float)Root::getDim()*Root::getDim();
}

int Level::gameLoop() {

    ALLEGRO_EVENT_QUEUE *coda_eventi = al_create_event_queue();
     if(!coda_eventi) 
    {
        al_destroy_display(al_get_current_display());
        return -1;
    }

    al_register_event_source(coda_eventi, al_get_display_event_source(al_get_current_display()));
    al_register_event_source(coda_eventi, al_get_timer_event_source(timer));
    al_register_event_source(coda_eventi, al_get_keyboard_event_source());

    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
    al_clear_to_color(al_map_rgb(255, 255, 255));
    al_flip_display();

//    al_hide_mouse_cursor(al_get_current_display()); //nasconde il cursore poichè non utilizzato.
    initMap();

    al_start_timer(timer);

    bool STOP = false;
    bool redraw = true;

    while(!STOP) {
    
        ALLEGRO_EVENT ev;
        al_wait_for_event(coda_eventi, &ev);

        cout << "-pre if ";
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            cout << "-byebye if ";
            
            STOP = true;

        } else if(ev.type == ALLEGRO_EVENT_TIMER) {
            cout << "-ALLEGRO_EVENT_TIMER ";
             
            STOP = (getArea() <= 25) or player->loseCondition();
    
            for(const auto& it : entities) {
                
                it.second->update();
                
                const auto& vel = borderGet(it.second->pos());
                if(vel != nullptr and it.second->locksOnObj(vel)) {
                    
                    if(it.second != player) {
                        
                        if(it.second->checkForCollision(player)) {
                            
                            player->deathEvent();
                            respawnPlayer();
                        
                        }

                        for(const auto& element : Root::makeExtendedNeighborhood(it.second->pos())) {
                        
                            const auto& entvel = (Hitbox*)entityGet(element);
                            const auto& borvel = borderGet(element);
                            if(entvel != nullptr and it.second->checkForCollision(entvel) 
                            or borvel != nullptr and it.second->GameObject::checkForCollision(borvel)) {
                             
                                it.second->bounce(element);
                            }
                        
                            if(getObj(element) == Type::TRACE
                            and it.second->GameObject::checkForCollision(borderGet(element))) {

                                player->deathEvent();
                                respawnPlayer();
                            }
                        }
                    } 
                    
                    if(getBody(it.second->pos()) == Body::BOSS) {

                        if(canSpawn())
                            spawnEnemy((Enemy*)it.second);

                    } else if(getBody(it.second->pos()) == Body::PLAYER) {
                        
                        if(getObj(it.second->pos()) == Type::BORDER) {
                
                            if(!player->isTraceEmpty())
                                updateMap();
                
                        } else if(getObj(it.second->pos()) == Type::VOID) {
                
                            GameObject* temp = new Hitbox(0, 0, 1, 1);
                            temp->reposition(it.second->pos());
                            
                            allocation.push_back(temp);
                            borders[temp->pos()] = temp;
                            
                            player->updateTrace(borderGet(it.second->pos()));
                
                        }
                    }

                    if(getObj(it.second->pos()) == Type::VOID and !checkForBounds(it.second->pos())) {

                        it.second->deathEvent(); 
                    
                    }
                }
            }

            cout << "-abilita il redraw ";

            redraw = true;

        }
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN and ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            cout << "-byebye2 if ";
            
            STOP = true;

        }else if(ev.type == ALLEGRO_EVENT_KEY_DOWN or ev.type == ALLEGRO_EVENT_KEY_UP) {
            cout << "-keyboard if ";

            player->updateKey(ev.keyboard.keycode, ev.type);
        }
        /*
            QUESTO DEVE DIVENTARE GESTIONE GRAFICA?
        */
        cout << "-redraw if ";
         if(redraw and al_is_event_queue_empty(coda_eventi))
        {
            redraw = false;
            
            ScaledBitmap::clear();
            cout << "-redraw: clear done ";
            for(const auto& it : borders)
                it.second->draw();
            cout << "-redraw: borders done ";
            for(const auto& it : entities)
             if(it.second->isActive())
                it.second->draw();
            cout << "-redraw: entities done ";
            ScaledBitmap::render();
            cout << "-redraw: rendering done ";
            al_flip_display();
            cout << "-redraw: all done ";
        }

        cout << endl << endl;
    }

    al_destroy_event_queue(coda_eventi);
    delMap();

    return 2;
}