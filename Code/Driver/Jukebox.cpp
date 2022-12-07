#include "sound.h"
#include "Jukebox.h"
#include "adsr_core.h"
#include "ddfs_core.h"

#define bullet_len 		5
#define ammo_len		2
#define reload_len		3
#define theme_len 		18
#define game_over_len 	18
#define game_win_len	20
#define game_len		20
		 	 	 	// 	 0 C
//         17.3239,   //  1 C#
//         18.3541,   //  2 D
//         19.4454,   //  3 D#
//         20.6017,   //  4 E
//         21.8268,   //  5 F
//         23.1247,   //  6 F#
//         24.4997,   //  7 G
//         25.9565,   //  8 G#
//         27.5000,   //  9 A
//         29.1352,   // 10 A#
//         30.8677   // 11 B


Sound miss_sfx[] = {Sound(0, 4, 4, 3), Sound(5, 4, 4, 3),Sound(7, 3, 4, 3), Sound(5, 1, 4, 3),Sound(9, 4, 8, 3)};
Sound out_ammo_sfx[] = {Sound(2, 4, 20, 3), Sound(6, 4, 20, 3)};
Sound hit_sfx[] = {Sound(0, 5, 4, 3), Sound(5, 5, 4, 3),Sound(7, 4, 4, 3), Sound(5, 2, 4, 3),Sound(9, 5, 8, 3)};
Sound reload_sfx [] = {Sound(9, 5 , 50, 3), Sound(0, 0 , 15, 3), Sound(7, 4 , 50, 3)};
Sound damage_sfx [] = {Sound(1, 2 ,100, 3), Sound(4, 2 , 25, 1), Sound(8, 2 , 25, 3), Sound(6, 2 , 25, 2), Sound(5,2,25,2)};

Sound main_theme[]= {
		Sound(0, 3, 300), Sound(3, 3, 300), Sound(0, 3, 300), Sound(3, 3, 300), Sound(0, 3, 300),
		Sound(1, 3, 300), Sound(2, 3, 300), Sound(3, 3, 300), Sound(4, 3, 300), Sound(5, 3, 300),
		Sound(1, 3, 200), Sound(2, 4, 200), Sound(3, 4, 200), Sound(2, 4, 200), Sound(1, 4, 200),
		Sound(1, 3, 200), Sound(0,3,300), Sound(3, 3, 300)
};

Sound low_hp_theme[]= {
		Sound{0, 3, 100}, Sound(1, 3, 200), Sound(1, 3, 200), Sound(3, 3, 500), Sound(1, 3, 200)};

Sound game_over_theme[game_over_len]= {
		Sound(0, 3, 300, 1), Sound(3, 3, 300, 1), Sound(0, 3, 300, 1), Sound(3, 3, 300, 1), Sound(0, 3, 300, 1),
		Sound(1, 3, 300, 1), Sound(2, 3, 300, 1), Sound(3, 3, 300, 1), Sound(4, 3, 300, 1), Sound(5, 3, 300, 1),
		Sound(1, 3, 100, 1), Sound(2, 4, 100, 1), Sound(3, 4, 100, 1), Sound(2, 4, 100, 1), Sound(1, 4, 100, 1),
		Sound(1, 3, 200, 1), Sound(0, 3, 300, 1), Sound(3, 3, 300, 1)
};

Sound game_win_theme[]= {
		Sound(1, 7, 200), Sound(2, 7, 200), Sound(3, 7, 200), Sound(4, 7, 200), Sound(5, 7, 200),
		Sound(6, 7, 200), Sound(7, 7, 200), Sound(6, 7, 200), Sound(5, 7, 200), Sound(4, 7, 200),
		Sound(8, 6, 200), Sound(11, 6, 200), Sound(8, 6, 200), Sound(10, 6, 200), Sound(1, 7, 200),
		Sound(3, 7, 200), Sound(5, 7, 200), Sound(7, 7, 200), Sound(9, 7, 200), Sound(11, 7, 200)
};

Sound game_theme[]= {
		Sound(1, 1, 200), Sound(2, 1, 200), Sound(3, 1, 200), Sound(2, 1, 100), Sound(1, 1, 100),
		Sound(2, 1, 200, 4), Sound(2, 1, 200, 4), Sound(6, 7, 200), Sound(5, 7, 200), Sound(4, 7, 200),
		Sound(8, 6, 200), Sound(11, 6, 200), Sound(8, 6, 200), Sound(10, 6, 200), Sound(1, 7, 200),
		Sound(3, 7, 200), Sound(5, 7, 200), Sound(7, 7, 200), Sound(9, 7, 200), Sound(11, 7, 200)
};

Jukebox::Jukebox(DdfsCore *ddfs, AdsrCore *adsr) {
   note = 0;
   max_note = 0;
   playing = false;
   adsr_p = adsr;
   ddfs_p = ddfs;
   active_song = main_theme;
   adsr_p->init();
}

Jukebox::~Jukebox() {
}     // not used

void Jukebox::changeSong(int song) {
    ready = true;
    note = 0;

    switch(song) 
    {
        case MISS_SFX: 		max_note = bullet_len; 		active_song = miss_sfx; 		break;
        case MAIN_THEME: 	max_note = theme_len; 		active_song = main_theme; 		break;
        case LOSE_THEME:	max_note = game_over_len; 	active_song = game_over_theme; 	break;
        case WIN_THEME: 	max_note = game_win_len; 	active_song = game_win_theme; 	break;
        case AMMO_OUT: 		max_note = ammo_len; 		active_song = out_ammo_sfx; 	break;
        case HIT_SFX: 		max_note = bullet_len; 		active_song = hit_sfx;	 		break;
        case RELOAD_SFX:	max_note = reload_len;		active_song = reload_sfx;		break;
        case DMG_SFX:		max_note = bullet_len;		active_song = damage_sfx;		break;
        default: 			max_note = 0;				active_song = main_theme;
    }
} 

bool Jukebox::playSong(bool loop) {
    if (!ready ||pause) return true;

    ddfs_p ->set_unit_source((active_song+note)->source);
    adsr_p->bypass();
    if (adsr_p->play_note_p(active_song+note)){
    	uart.disp(note);
    	note++;
    }
    if (note == max_note && !loop) stopMusic(true, false);
    else if (note == max_note && loop) note = 0;

    return note == max_note && !loop;
}   

void Jukebox::stopMusic(bool clear, bool p)
{
    adsr_p->stop();
    playing = false;
    pause = p;
    if (clear)
    {
        note = 0;
        ready = false;
    }
}

void Jukebox::resumeMusic()
{
    playing = true;
    pause = false;
}
