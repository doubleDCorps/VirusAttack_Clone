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