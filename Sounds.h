#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
using namespace std;

class Sounds
{
    private:
        ALLEGRO_SAMPLE* menu_voice_navigation;
        ALLEGRO_SAMPLE* you_win;
        ALLEGRO_SAMPLE* player_death;
        ALLEGRO_SAMPLE* mob_death;
        ALLEGRO_SAMPLE* cut;
        ALLEGRO_SAMPLE* menu_theme;
        ALLEGRO_SAMPLE* inLevel_theme;
        ALLEGRO_SAMPLE* flowey_theme;
        ALLEGRO_SAMPLE* mob_spawn;
        ALLEGRO_SAMPLE_INSTANCE* menu_looped_theme;
        ALLEGRO_SAMPLE_INSTANCE* inLevel_looped_theme;
        ALLEGRO_SAMPLE_INSTANCE* flowey_looped_theme;
        ALLEGRO_SAMPLE_INSTANCE* looped_cut;

    public:
    Sounds() { 
        al_reserve_samples(5);
        menu_voice_navigation = al_load_sample("resources/audio/menu_voice_navigation.wav");
        you_win = al_load_sample("resources/audio/you_win.wav");
        player_death = al_load_sample("resources/audio/player_death.wav");
        mob_death = al_load_sample("resources/audio/mob_death.wav");
        mob_spawn = al_load_sample("resources/audio/mob_spawn.wav");
        cut = al_load_sample("resources/audio/cut.wav");
        menu_theme = al_load_sample("resources/audio/menu_theme.wav");
        inLevel_theme = al_load_sample("resources/audio/inLevel_theme.wav");
        flowey_theme = al_load_sample("resources/audio/flowey_theme.wav");

        menu_looped_theme = al_create_sample_instance(menu_theme);
        inLevel_looped_theme = al_create_sample_instance(inLevel_theme);
        flowey_looped_theme = al_create_sample_instance(flowey_theme);
        looped_cut = al_create_sample_instance(cut);
        al_attach_sample_instance_to_mixer(menu_looped_theme, al_get_default_mixer());
        al_attach_sample_instance_to_mixer(inLevel_looped_theme, al_get_default_mixer());
        al_attach_sample_instance_to_mixer(flowey_looped_theme, al_get_default_mixer());
        al_attach_sample_instance_to_mixer(looped_cut, al_get_default_mixer());
    }

    void playNavigation()
    {
        al_play_sample(menu_voice_navigation, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE,NULL); //ONCE oppure LOOP
    }

    void playYouWin()
    {
        al_play_sample(you_win, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE,NULL);
    }

    void playPlayerDeath()
    {
        al_play_sample(player_death, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE,NULL); 
    }

    void playMobDeath()
    {
        al_play_sample(mob_death, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE,NULL); 
    }

    void playModSpawn()
    {
        al_play_sample(mob_spawn, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE,NULL); 
    }

    void playMenuTheme()
    {
        if (!al_get_sample_instance_playing(menu_looped_theme))
          al_play_sample_instance(menu_looped_theme);
    }

    void stopMenuTheme() 
    {
        al_stop_sample_instance(menu_looped_theme);
    }

    void playInLevelTheme()
    {
        if (!al_get_sample_instance_playing(inLevel_looped_theme))
          al_play_sample_instance(inLevel_looped_theme);
    }

    void stopInLevelTheme() 
    {
        al_stop_sample_instance(inLevel_looped_theme);
    }

    void playFloweyTheme()
    {
        if (!al_get_sample_instance_playing(flowey_looped_theme))
          al_play_sample_instance(flowey_looped_theme);
    }

    void stopFloweyTheme() 
    {
        al_stop_sample_instance(flowey_looped_theme);
    }

    void playCut()
    {
        if (!al_get_sample_instance_playing(looped_cut))
          al_play_sample_instance(looped_cut);
    }

    void stopCut() 
    {
        al_stop_sample_instance(looped_cut);
    }

    void stopsounds(){al_stop_samples();}

    ~Sounds()
    {
        al_destroy_sample(menu_voice_navigation);
        al_destroy_sample(you_win);
        al_destroy_sample(player_death);
        al_destroy_sample(mob_death);
        al_destroy_sample(cut);
        al_destroy_sample(menu_theme);
        al_destroy_sample(inLevel_theme);
        al_destroy_sample(flowey_theme);
        al_destroy_sample(mob_spawn);
        al_destroy_sample_instance(menu_looped_theme);
        al_destroy_sample_instance(inLevel_looped_theme);
        al_destroy_sample_instance(flowey_looped_theme);
        al_destroy_sample_instance(looped_cut);

        al_uninstall_audio();
    }

};