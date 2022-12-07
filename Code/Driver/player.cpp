#include "player.h"
#include "chu_init.h"
//update sprite based on mouse position


Player::Player(uint32_t core_base_addr, int sprite_size, SpriteCore *sprite): SpriteCore(core_base_addr, sprite_size) {
	bypass(1);
	wr_ctrl(0x00);
	_sprite = sprite;
}

Player::~Player(){

}

void Player::spawn()
{
	gotoXY(320, 240);
	bypass(0);
}
void Player::rotateSprite(){
	int xpos = _sprite->x;
	int ypos = _sprite->y;

	if (xpos>=320-64 && xpos<=320+64)
	{
		if (ypos<=240) {
			spriteID = UP;
		}
		else {
			spriteID = DWN;
		}
	}
	else
	{
		if (xpos<320) {
					spriteID = LFT;
				}
				else {
					spriteID = RGT;
				}
	}

	changeSprite(spriteID);
}

