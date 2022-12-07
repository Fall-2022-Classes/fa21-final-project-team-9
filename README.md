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
7. Zomboids
8. Player
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
<br>  
<br>  
<br>

