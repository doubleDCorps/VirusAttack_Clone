#include"Level.h"

int Level::shift(int k) {

    return k + (120/GameObject::getSize())*Root::getDim() + (60/GameObject::getSize());
}

pair<int, int> Level::shift(int x, int y) {

    return {x + (120/GameObject::getSize()), y + (60/GameObject::getSize())};
}

Level::Level(ALLEGRO_TIMER* t): timer(t), borders(), entities(), boss() {}

void Level::respawnPlayer() {

    bool enablePrint = false;
    
    int pivot = -1;
    vector<int> N = Root::makeNeighborhood(player->pos());
    
    if(enablePrint) {
        cout << "Level::respawnPlayer(): vicinato -> ";
        for(const auto& it : N) 
            cout << " " << it;
        cout << endl << endl;
    }

    int arr[] = {1, -1, Root::getDim(), -Root::getDim()};
    
    for(int i = 0; i < Root::getDim(); ++i) {
     
     for(const auto& it : N) 
      for(int j = 0; j < 4; ++j) {
        
        if(enablePrint) {
            cout << "Level::respawnPlayer(): " << player << ": " 
            << Level::shift(it + arr[j]*i) << " -> " 
            << Level::shift(it + arr[j]*i)/Root::getDim() << " "
            << Level::shift(it + arr[j]*i) - Level::shift(it + arr[j]*i)/Root::getDim()
            << "\n";
        }
        
        if(getObj(it + arr[j]*i) == Type::BORDER) {
            pivot = it + arr[j]*i;

            if(pivot != -1) {
                
                player->reposition(pivot);
                player->respawn();

                if(enablePrint)
                    cout << "lo riposiziono in : " << pivot <<  " -> " <<  player -> pos() << endl;
                
                return;
            }
        }
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
            return;
        }
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
    pair<int, int> val;
    for(int i = 0; i < Root::getDim(); ++i) {

        float t = i;

        val = Level::shift(t, 0);
        temp = new Hitbox(val.first, val.second, 1, 1);
        temp->setImage(temp->getW(), temp->getH(), al_map_rgb(255, 255, 255));
        allocation.push_back(temp);
        borders[temp->pos()] = temp;

        val = Level::shift(0, t);
        temp = new Hitbox(val.first, val.second, 1, 1);
        temp->setImage(temp->getW(), temp->getH(), al_map_rgb(255, 255, 255));
        allocation.push_back(temp);
        borders[temp->pos()] = temp;

        val = Level::shift(t, (Root::getDim()-1));
        temp = new Hitbox(val.first, val.second, 1, 1);
        temp->setImage(temp->getW(), temp->getH(), al_map_rgb(255, 255, 255));
        allocation.push_back(temp);
        borders[temp->pos()] = temp;

        val = Level::shift((Root::getDim()-1), t);
        temp = new Hitbox(val.first, val.second, 1, 1);
        temp->setImage(temp->getW(), temp->getH(), al_map_rgb(255, 255, 255));
        allocation.push_back(temp);
        borders[temp->pos()] = temp;
    }

    //allocazione player
    val = Level::shift(((Root::getDim()-1))/2.0f, ((Root::getDim()-1))/2.0f);
    player = new Player(val.first, val.second, 
                        3, 3,
                        4.0f, 4.0f);
    player->setImage(player->getW(), player->getH(), al_map_rgb(100, 200, 150));
    allocation.push_back(player);
    cout << "entro nel respawn\n";
    respawnPlayer();
    entities[player->pos()] = player;
    
    //allocazione boss
    boss.push_back(new Enemy(val.first, val.second, 
                             3, 3, 
                             -4.0, -4.0, 
                             true));
    boss[0]->setImage(boss[0]->getW(), boss[0]->getH(), al_map_rgb(150, 200, 100));
    allocation.push_back(boss[0]);
    entities[boss[0]->pos()] = boss[0];

    
    cout << "Level::initMap(): enemy list\n"; 
    //allocazione nemici
    int v = boss[0]->pos();
    for(const auto& value : Root::makeExtendedNeighborhood(v)) {
        
        float dx = float(v/Root::getDim() - value/Root::getDim());
        float dy = float(v%Root::getDim() - value%Root::getDim());

        dx = (dx == 0) ? dy*2.0f : dx*4.0f;
        dy = (dy == 0) ? dx*2.0f : dy*4.0f;

        Entity* temp = new Enemy(0, 0, 3, 3, dx, dy, false);
        temp->reposition(value);
        temp->setImage(temp->getW(), temp->getH(), al_map_rgb(200, 100, 150));
        allocation.push_back(temp);
        entities[temp->pos()] = temp;
    
        cout << " - " << temp << ": " << dx << ":" << dy << "\n"; 
    }
}

void Level::delMap() {

    for(const auto& val : allocation)
        delete val;
}

const Hitbox *const Level::borderGet(int val) const {
    
    return isObj(val) ? borders.at(val) : nullptr;
}

const Entity *const Level::entityGet(int val) const {
    
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
    
    cout << "Level::getArea() preps complete " << boss[0]->pos()
    << " -> "<< boss[0]->getX() << " " << boss[0]->getY() << "\n";
    //algoritmo
    while(!positions.empty()) {
        //fissata una posizione
        int next = positions.front();
        analyzed.insert(next);
        positions.pop();
        
        retval += 1;
        //cout <<"Level::getArea() " << next << "-" << retval << "\n";
        //si generano tutte le posizioni vicine valide
        for(const auto& element : Root::makeExtendedNeighborhood(next))
        //se non è già stato analizzato
         if(analyzed.find(element) == analyzed.end() and getObj(element) != Type::BORDER) {
            positions.push(element);
        }
    }
    cout << "\n Level::getArea() alg complete " 
    << (retval * 100) / Root::getDim()*Root::getDim() << endl;
    
    return 
        (retval * 100) / Root::getDim()*Root::getDim();
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
             
            //STOP = (getArea() <= 25) or player->loseCondition();
    
            for(const auto& it : entities) {
                
                it.second->update();
                
                const auto& entvel = entityGet(it.second->pos());
                const auto& borvel = borderGet(it.second->pos());
                        
                cout << "\n THIS entvel: " << entvel << " borvel: " << borvel << endl;
                
                if(entvel != nullptr and it.second->checkForCollision(entvel) 
                or borvel != nullptr and it.second->checkForCollision(borvel)) {
                    
                    it.second->bounce(it.second->pos());    

                    if(*it.second != *player) {
                        
                        if(*entvel == *player) {
                            
                            player->deathEvent();
                            respawnPlayer();
                        
                        }

                        if(borvel != nullptr and getObj(it.second->pos()) == Type::TRACE) {

                            player->deathEvent();
                            respawnPlayer();
                        }

                        for(const auto& element : Root::makeExtendedNeighborhood(it.second->pos())) {
                        
                            const auto& entvel = entityGet(element);
                            const auto& borvel = borderGet(element);

                            cout << "\n " << element << " entvel: " << entvel << " borvel: " << borvel << endl;
                            if(entvel != nullptr and it.second->checkForCollision(entvel) 
                            or borvel != nullptr and it.second->checkForCollision(borvel)) {
                             
                                cout << "it's gonna bounce\n";
                                it.second->bounce(element);
                            }
                        
                            if(getObj(element) == Type::TRACE and borvel != nullptr
                            and it.second->checkForCollision(borvel)) {

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
                
                            Hitbox* temp = new Hitbox(0, 0, 1, 1);
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
            for(const auto& it : borders)
                it.second->draw();
            for(const auto& it : entities)
             if(it.second->isActive())
                it.second->draw();
            ScaledBitmap::render();
        }

        cout << endl << endl;
    }

    al_destroy_event_queue(coda_eventi);
    delMap();

    return 2;
}