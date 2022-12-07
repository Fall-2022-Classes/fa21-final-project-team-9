/*****************************************************************//**
 * @file NightOfTheLivingUndead.cpp
 *
 * @brief Night of the Living Undead Game
 *
 * @author p chu, Jason Loc, Kyler Martinez
 * @version v1.0: initial release
 *********************************************************************/

//#define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "vga_core.h"
#include "sseg_core.h"
#include "ddfs_core.h"
#include "adsr_core.h"
#include "sound.h"
#include "vga_core.h"
#include "ps2_core.h"
#include "Jukebox.h"
#include "zombie.h"
#include "player.h"

#define MAX_SCORE 	50
#define WIN_SCORE	25
#define MAX_ZOMB 	2
#define ZOMB1_SPEED 5
#define ZOMB2_SPEED	3
#define	SPRITE_LEN	32
#define LIVES		3
#define	AMMO		6

// external core instantiation
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
FrameCore frame(FRAME_BASE);
GpvCore bar(get_sprite_addr(BRIDGE_BASE, V7_BAR));
GpvCore gray(get_sprite_addr(BRIDGE_BASE, V6_GRAY));

Zomboid ghost2(get_sprite_addr(BRIDGE_BASE, V4_USER4), SPRITE_LEN, ZOMB1_SPEED);
Zomboid ghost(get_sprite_addr(BRIDGE_BASE, V3_GHOST), SPRITE_LEN, ZOMB2_SPEED);

SpriteCore mouse(get_sprite_addr(BRIDGE_BASE, V1_MOUSE), SPRITE_LEN);

Player player(get_sprite_addr(BRIDGE_BASE, V5_USER5), SPRITE_LEN, &mouse);

OsdCore osd(get_sprite_addr(BRIDGE_BASE, V2_OSD));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));
DdfsCore ddfs(get_slot_addr(BRIDGE_BASE, S12_DDFS));
AdsrCore adsr(get_slot_addr(BRIDGE_BASE, S13_ADSR), &ddfs);
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));
Jukebox jukebox(&ddfs, &adsr);

void waitClick()
{
	int rbtn, xmov, ymov, start = 0;
	while (start == 0)
	{
		jukebox.playSong(true);
		if (ps2.get_mouse_activity(&start, &rbtn, &xmov, &ymov));
	}
}


void wait(int time) //mouse friendly
{
	int lbtn, rbtn, xmov, ymov;
	int cool = now_ms();
	while (now_ms()- cool < time) {
		ps2.get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov);
		jukebox.playSong();
	}
}

void display_txt_var(OsdCore *osd_p, char message[10], int size, int start, int var)
{
	osd_p->write(start, 0, message);
	osd_p->write(start+size, 0, var);
}

void update_OSD(OsdCore *osd_p, int score, int lives, int ammo, int color = 0x00f)
{
	osd_p->set_color(color, 0x000); // dark gray/green
	osd_p->bypass(0);
	osd_p->clr_screen();
	display_txt_var(osd_p, "ZOMBOID: ", 9, 3, score);
	display_txt_var(osd_p, "AMMO: ", 6, 35, ammo);
	display_txt_var(osd_p, "LIVES: ", 7, 60, lives);
}

void start_screen(OsdCore *osd_p)
{
	osd_p->clr_screen();
	osd_p->bypass(0);
	osd_p->set_color(0xf00, 0x000); // dark gray/green

	jukebox.changeSong(1);

	char lines[3][30] = {"NIGHT OF THE", "LIVING","UNDEAD"};

	for (int i = 0; i < 3;i++)
		osd_p->write(25, 8+i, lines[i]);

	osd_p->write(25, 20,  "CLICK TO START");

	osd_p->write(25, 24,  "A KYLER & JASON PRODUCTION");

	waitClick();

	osd_p->bypass(1);
	osd_p->clr_screen();
}

bool story_screen(OsdCore *osd_p)
{
	int refTime = now_ms();
	bool lastStand = false;
	osd_p->bypass(0);
	osd_p->set_color(0xf00, 0x000); // dark gray/green

	char lines[5][30] = { "ON A DARK DAY THE END CAME.", "ZOMBIES ROAMED THE LAND.",
			"MOST OF HUMANITY PERISHED.", "ARMED WITH HIS SHOTGUN, ", "HE TAKES HIS LAST STAND..."};

	for (int i = 0; i <5; i++)
	{
		osd_p->write(5, 5+i, lines[i]);
		wait(50);
	}

	osd_p->write(5, 20, "CLICK TO BEGIN");

	waitClick();

	jukebox.stopMusic(true, false);

	osd_p->clr_screen();

	if (now_ms()-refTime >= 30000) {
		lastStand = true;
		osd_p->write(25, 20,  "NO ONE WILL COME FOR YOU");
	}
	wait(1000);
	osd_p->clr_screen();
	osd_p->bypass(1);
	return lastStand;
}

void game_win(OsdCore *osd_p, int score)
{
	int digits;
	jukebox.changeSong(WIN_THEME);

	osd_p->bypass(0);
	osd_p->set_color(0x0f0, 0x000); // dark gray/green

	char lines[7][35] = { "DESPITE HIS BEST EFFORTS", "THE MAN COULD NOT KILL THEM ALL", "RUNNING LOW ON AMMO.",
			"HE SPARED HIMSELF AND THE WORLD.", "DEATH PREVENTED THE VIRUS FROM", "ADDING A NEW MEMBER TO THE HORDE.",
			"HE SAVED THE WORLD FROM "};

	for (int i = 0; i < 7; i++)
		osd_p->write(5, 5+i, lines[i]);

	digits = osd_p->write(5, 13, score);
	osd_p->write(5+digits+1, 13, "ZOMBIES");
	osd_p->write(5, 20, "GAME OVER: GOOD ENDING");

	waitClick();

	jukebox.stopMusic(true, false);
	osd_p->clr_screen();
}

void last_stand_win(OsdCore *osd_p, int score)
{
	int digits;
	jukebox.changeSong(WIN_THEME);

	osd_p->bypass(0);
	osd_p->set_color(0xf0f, 0x000); // dark gray/green

	char lines[5][35] = { "DESPITE HIS BEST EFFORTS", "THE MAN COULD NOT KILL THEM ALL",
			"WITH NO END IN SIGHT", "HE MADE HIS LAST STAND.", "HE KILLED"};


	for (int i = 0; i < 5; i++)
		osd_p->write(5, 5+i, lines[i]);

	digits = osd_p->write(15, 9, score);
	osd_p->write(15+digits+1, 9, "ZOMBIES BEFORE HE FELL.");
	osd_p->write(5, 20, "GAME OVER: LAST STAND ENDING");

	waitClick();

	jukebox.stopMusic(true, false);
	osd_p->clr_screen();
}

void true_win(OsdCore *osd_p, int score)
{
	int digits;
	jukebox.changeSong(WIN_THEME);

	osd_p->bypass(0);
	osd_p->set_color(0x0ff, 0x000); // dark gray/green

	char lines[6][40] = { "HE KILLED UNTIL IT WAS DONE.", "NO UNDEAD COULD STOP HIM.", "EMPTY SHELLS LITTERED THE GROUND.",
			"NONE WERE PREPARED TO FACE", "THE MIGHT OF THE SLAYER.", "HE REIGNS SUPREME."};

	for (int i = 0; i < 6; i++)
		osd_p->write(5, 5+i, lines[i]);

	osd_p->write(5, 12, "HE SLAUGHTERED");
	digits = osd_p->write(20, 12, score);
	osd_p->write(20+digits+1, 12, "ZOMBIES.");
	osd_p->write(5, 20, "TRUE ENDING");

	waitClick();

	jukebox.stopMusic(true, false);
	osd_p->clr_screen();
}

void game_over(OsdCore *osd_p, int score)
{
	int digits;
	jukebox.changeSong(2);

	osd_p->bypass(0);
	osd_p->set_color(0xf00, 0x000); // dark gray/green

	char lines[3][30] = {"THE MAN DIED ON THE FIELD.", "OVERUN BY ZOMBIES.", "TO BECOME ONE HIMSELF."};

	for (int i = 0; i < 3; i++)
		osd_p->write(5, 5+i, lines[i]);

	osd_p->write(5, 9, "HE SAVED THE WORLD FROM ");
	digits = osd_p->write(5, 10, score);
	osd_p->write(5+digits+1, 10, "ZOMBIES.");

	osd_p->write(5, 20,  "GAME OVER: BAD ENDING");

	waitClick();
	jukebox.stopMusic(true, false);
	osd_p->clr_screen();
}

int main() {
	int lbtn, rbtn, xmov, ymov;
	int score = 0;
	player.bypass(1);
	ghost2.hide();
	ghost.hide();
	mouse.bypass(1);

	Zomboid zombs[] = {ghost, ghost2};

	int id = ps2.init();
	int cool = 0, bullet_cooldown = 0, reload_cooldown= 0, anim_cool = 0;
	int difficulty = 50;
	int lives, ammo, song = 0, hit_frame = FLESH;
	bool reloading = false, win = false, sfx = false, playerhit = false;
	bool osdUpdate = true, hit = false, lastStand = false, trueEnding = false;
	int osdColor = 0x00f;

	while (1)
	{
		lives = LIVES;
		score = 0;
		ammo = AMMO;
		frame.clr_screen(0);
		start_screen(&osd);
		wait(1000);
		lastStand = story_screen(&osd);
		mouse.bypass(0);
		mouse.gotoXY(320,240);
		frame.drawBackground();
		player.spawn();
		trueEnding = false;

		for (int i = 0; i < MAX_ZOMB; i++)
			zombs[i].spawn();

		update_OSD(&osd, score, lives, ammo);
		bullet_cooldown = now_ms();
		reload_cooldown= now_ms();
		reloading = false;
		win = false;
		osdUpdate = true;
		osdColor = (lastStand)? 0x0ff: 0x0f3;

		while (lives > 0 && !trueEnding){
			if (osdUpdate) {
				update_OSD(&osd, score, lives, ammo, osdColor);
				osdUpdate = false;
			}

			jukebox.playSong(false);

			if (ps2.get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov))
			   {
					mouse.moveXY(xmov, -ymov);
					player.rotateSprite();
					if (lbtn && now_ms()-bullet_cooldown > 300)
					{
						if ( ammo > 0 && !reloading)
						{
							ammo--;
							bullet_cooldown = now_ms();
							hit = false;
							for (int i = 0; i < MAX_ZOMB; i++)
							{
								if (zombs[i].mouseCollide(mouse.x, mouse.y))
								{
									hit = true;
									score++;
									if (score == WIN_SCORE) win = true;
									else if (score == MAX_SCORE && lives >= 3 && !lastStand) trueEnding = true;
								}
								osdUpdate = true;
							}
							jukebox.changeSong((hit)?HIT_SFX: MISS_SFX);
						}
						else if (ammo == 0)
							jukebox.changeSong(AMMO_OUT);
					}
					if (rbtn)
					{
						reloading = true;
						jukebox.changeSong(RELOAD_SFX);
					}
			   }



			if (now_ms()-reload_cooldown > 500 && reloading)
			{
				ammo = AMMO;
				reloading = false;
				reload_cooldown = now_ms();
				osdUpdate = true;
			}

			if ((now_ms()-cool) > (difficulty-score/5))
			{
				cool = now_ms();
				if (lives > 0)
				{
					for (int i = 0; i < MAX_ZOMB; i++)
					{
						if (zombs[i].centerCollide())
						{
							lives--;
							playerhit = true;
							hit_frame = BRED;
							player.changeColor(hit_frame);
							anim_cool = now_ms();
							if (lives == 1) osdColor = 0xf22; // danger zone
							osdUpdate = true;
							zombs[i].spawn();
						}
						else if (zombs[i].dead) zombs[i].spawn();
						else zombs[i].moveZomb();
					}
				}
			}

			if(playerhit){
				jukebox.changeSong(DMG_SFX);
			}

			if(now_ms()-anim_cool > 100 && playerhit){
				hit_frame = FLESH;
				player.changeColor(hit_frame);
				anim_cool = now_ms();
				playerhit = false;
			}


	   } // while

		jukebox.stopMusic(true, false);
		for (int i = 0; i < MAX_ZOMB; i++) zombs[i].hide();
		osd.clr_screen();
		player.bypass(1);
		frame.clr_screen(0);

		while (now_ms()- cool < 1000) ps2.get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov);
		mouse.bypass(1);

		if (trueEnding)
			true_win(&osd, score);
		else if (win && !lastStand)
			game_win(&osd, score);
		else if (win && lastStand)
			last_stand_win(&osd, score);
		else
			game_over(&osd,score);
		cool = now_ms();
		while (now_ms()- cool < 1000) ps2.get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov);
	}
} //main
