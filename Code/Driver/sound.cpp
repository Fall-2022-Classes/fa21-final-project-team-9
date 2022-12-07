#include "sound.h"

Sound::Sound(int nte, int oct, int dur, int src) {
   note = nte;
   octave = oct;
   duration = dur;
   source = src;
}
Sound::~Sound() {
}     // not used
