# Final Project: Night Of The Living Undead
## Team 9: Jason Luc and Kyler Martinez

### *Code Organization*
1. HDL
2. Driver
3. Test Application

### *Table of Contents*
1. Introduction
2. Audio Cores
3. Audio Drivers 
4. Music
5. Sprite Cores
6. Spirte Drivers
7. Game Characters
8. Display
9. Application Overview
10. Gamplay
11. Demo
<br>  

### Introduction
We made a zombie shooter last stand top down cool griddy filled game. (Change this lol)

<br>  

### Audio Cores
The audio cores used in the project are based on the ones devloped by (INSERT NAME) Chu and were provided by the textbook (INSERT THE NAME KYLER) but have been modifed to suit the needs of the project. Orignally the cores were set to produce sounds based on a sine wave. This has been altered to support four possible signals, a sine wave, a sawtooth wave, a random (noise) wave, and a square wave. The DFFS audio core outputs one of these signals to the ADSR core and a register control which signal is to be used. The user can write to this register to change which audio source they wish to use.

<br>  

### Audio Driver
The DDFS and ADSR cores are controlled by the appropiate drivers. The original drivers were devloped by (THE AUTHOR) but have been modified. The DDFS has been modified to allow the programer to change which audio source has been selected to output music. The ADSR driver has been modified to include functions to play notes that return a boolean that indictates whether the note is finished. A version of this function is implemented to accept a *Sound* object as an input.

<br>

### Music
#### Sound Class
The sound class is an object that stores the note of the sound, the octave the note is in, the duration of the note, and the audio source of the note.

<br>  

#### Jukebox Class
The Jukebox class is the primary class used to control the music in the game. The class contains the melodies for music sequences such as the title theme, game over theme, and the various sound effects used. There are defintions for the different sound effects and songs avalible to be used.
<br>  

The four primary methods are:
*  *changeSong* : set the Jukebox to play the supplied song.
*  *playSong* : play the current song loaded and return whether it is done. If the user wants to loop, restart the song.
*  *stopMusic* : stop the playing of the song. Pause or clear the Jukebox if the user chooses to.
*  *resumeMusic* : resume the song from a pause.
  

<br>

### Sprite Core
Words

<br>

### Sprite Driver
Words

<br>

### Game Characters
#### Zomboid
Words

#### Player
Words

<br>

### Display
#### Background
Words

#### On Screen Display
Words

<br>

### Application Overview
<br>

### Gameplay
<br>

### Demo
<br>
