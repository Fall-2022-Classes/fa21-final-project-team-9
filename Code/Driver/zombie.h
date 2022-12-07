#include "vga_core.h"

#ifndef _ZOMBIE_H_INCLUDED
#define _ZOMBIE_H_INCLUDED

class Zomboid : public SpriteCore {

	//initiate with start position, USE RANDOM NUM GEN IN MAIN
	//travel to middle
	//delete upon collision with player or bullet class

public:
	Zomboid(uint32_t core_base_addr, int sprite_size, int speed); //initialize with starting position
	~Zomboid();

	void spawn();
	void hide();
	bool mouseCollide(int mx, int my);
	bool centerCollide();
	void updateSpeed();
	void moveZomb();
	bool dead;
	int spriteID=0;
	int baseSpeed;
};

#endif  // _ZOMBIE_H_INCLUDED
