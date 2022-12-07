/*****************************************************************//**
 * @file Jukebox.h
 *
 * @brief Control sounds and what song is being played
 *
 * @author Kyler Martinez
 * @version v1.0: initial release
 *********************************************************************/

#ifndef _JUKEBOX_H_INCLUDED
#define _JUKEBOX_H_INCLUDED

#include "chu_init.h"
#include "sound.h"
#include "adsr_core.h"

#define MISS_SFX	0
#define MAIN_THEME	1
#define LOSE_THEME	2
#define WIN_THEME 	3
#define AMMO_OUT 	4
#define HIT_SFX		5
#define RELOAD_SFX 	6
#define DMG_SFX		7


class Jukebox{
public:
    /* methods */
    /**
    * constructor.
    *
    * @note an adsr core must be connected to a ddfs core.
    * @note constructor call init() to configure the ddfs core.
    */
    Jukebox(DdfsCore *ddfs, AdsrCore *adsr);
    ~Jukebox();                  // not used

    void changeSong(int song);
    bool playSong(bool loop = false);
    void stopMusic(bool clear, bool p);
    void resumeMusic();
    bool playing, ready, pause;
    int note, max_note;
    AdsrCore *adsr_p;
    DdfsCore *ddfs_p;
    Sound *active_song;

};
#endif  // _SOUND_H_INCLUDED



