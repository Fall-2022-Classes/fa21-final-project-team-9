/*****************************************************************//**
 * @file adsr_core.h
 *
 * @brief Configure and control adsr core
 *
 * @author p chu
 * @version v1.0: initial release
 *********************************************************************/

#ifndef _SOUND_H_INCLUDED
#define _SOUND_H_INCLUDED

#include "chu_init.h"

class Sound{
public:
   /* methods */
   /**
    * constructor.
    *
    * @note an adsr core must be connected to a ddfs core.
    * @note constructor call init() to configure the ddfs core.
    */
   Sound(int nte, int oct, int dur, int src = 0);
   ~Sound();                  // not used

   int note, octave, duration, source;
};
#endif  // _SOUND_H_INCLUDED



