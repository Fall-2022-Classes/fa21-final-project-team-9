#include "zombie.h"
#include "chu_init.h"
#define mouseCenter 16
#define	SPEED_UP	2

Zomboid::Zomboid(uint32_t core_base_addr, int sprite_size, int speed): SpriteCore(core_base_addr, sprite_size) {
	bypass(1);
	wr_ctrl(0x00);
	baseSpeed = speed;
}

Zomboid::~Zomboid(){
}

void Zomboid::spawn()
{
	changeColor(randint(0,3));
	get_start();
	if (x<=320)
	{
		spriteID = 1;
	}
	else
	{
		spriteID = 0;
	}
	changeSprite(spriteID);
	dead = false;

	bypass(0);
}

void Zomboid::hide()
{
	bypass(1);
}

bool Zomboid::mouseCollide(int mx, int my)
{
	dead = collide(mx+16, my+16);
	return dead;
}


bool Zomboid::centerCollide()
{
	return collide(320, 240);
}

void Zomboid::updateSpeed()
{
	speedx =(x < 320)? baseSpeed: -baseSpeed;
	speedy =(y < 240)? baseSpeed: -baseSpeed;

	if (315 < x  && x < 325 )
	{
		speedx = 0;
		if (speedy != 0) speedy = (y < 240)? baseSpeed+SPEED_UP: -baseSpeed-SPEED_UP;
	}

	if (235 < y && y < 245)
	{
		speedy = 0;
		if (speedx != 0) speedx = (x < 320)? baseSpeed+SPEED_UP: -baseSpeed-SPEED_UP;
	}
}

void Zomboid:: moveZomb()
{
	switch(spriteID)
	{
	case 0:

		spriteID = 2;
		break;
	case 1:
		spriteID = 3;
		break;
	case 2:
		spriteID = 0;
		break;
	default:
		spriteID = 1;
		break;
	}
	changeSprite(spriteID);
	updateSpeed();
	moveDir();

}



