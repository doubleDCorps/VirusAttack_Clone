#include"Level.h"
#include<allegro5/allegro.h>

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
    
    defPerInit(disp_data);
    
    ALLEGRO_DISPLAY *display = al_create_display(disp_data.width, disp_data.height);
     if(!display)
    { 
        al_destroy_timer(timer);
        return -1;
    }
    
    Level level(1, disp_data);

    level.loop();

    al_destroy_display(display);
    al_destroy_timer(timer);

    return 0;
}
