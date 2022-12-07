#include "vga_core.h"

#ifndef _PLAYER_H_INCLUDED
#define _PLAYER_H_INCLUDED

#define UP	2
#define DWN	0
#define LFT 3
#define RGT 1

#define FLESH	0
#define LRED	1
#define WHITE	2
#define BRED	3

class Player : public SpriteCore {

public:
	Player(uint32_t core_base_addr, int sprite_size, SpriteCore *sprite);
	~Player();
	SpriteCore *_sprite;
	void rotateSprite();
	void spawn();

	int spriteID=0;
};

#endif  // _PLAYER_H_INCLUDED
