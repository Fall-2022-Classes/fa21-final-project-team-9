/*****************************************************************//**
 * @file timer_core.cpp
 *
 * @brief implementation of various video core classes
 *
 * @author p chu
 * @version v1.0: initial release
 ********************************************************************/

#include "vga_core.h"

/**********************************************************************
 * General purpose video core methods
 *********************************************************************/
GpvCore::GpvCore(uint32_t core_base_addr) {
   base_addr = core_base_addr;
}
GpvCore::~GpvCore() {
}

void GpvCore::wr_mem(int addr, uint32_t data) {
   io_write(base_addr, addr, data);
}

void GpvCore::bypass(int by) {
   io_write(base_addr, BYPASS_REG, (uint32_t ) by);
}

/**********************************************************************
 * Sprite core methods
 *********************************************************************/
SpriteCore::SpriteCore(uint32_t core_base_addr, int sprite_size) {
   base_addr = core_base_addr;
   size = sprite_size;
   speedx = 0;
   speedy = 0;
}
SpriteCore::~SpriteCore() {
}

void SpriteCore::wr_mem(int addr, uint32_t color) {
   io_write(base_addr, addr, color);
}

void SpriteCore::bypass(int by) {
   io_write(base_addr, BYPASS_REG, (uint32_t ) by);
}

void SpriteCore::gotoXY(int xpos, int ypos) {
	x = xpos;
	y = ypos;
	io_write(base_addr, X_REG, x);
	io_write(base_addr, Y_REG, y);
	return;
}

void SpriteCore::moveXY(int xpos, int ypos, int exitScreen) {
	x += xpos;
	y += ypos;
	if (exitScreen == 0)
	{
		 if (x < 0) x = 0;
		 if (y < 0) y = 0;
		 if (x > VideoW-size) x = VideoW-size;
		 if (y > VideoH-size) y = VideoH-size;
	}

	io_write(base_addr, X_REG, x);
	io_write(base_addr, Y_REG, y);
   return;
}

void SpriteCore::moveDir() {
	x += speedx;
	y += speedy;
	io_write(base_addr, X_REG, x);
	io_write(base_addr, Y_REG, y);
   return;
}

void SpriteCore::stop() {
	speedx=0;
	speedy=0;
   return;
}

void SpriteCore::get_start()
{
	int side = randint(0, 3);
	if (side == 0)
		gotoXY(5 * randint(0, 128), 0);
	else if (side == 2)
		gotoXY(5 * randint(0, 128), VideoH-size);
	else if (side == 1)
		gotoXY(VideoW-size, 5 * randint(0, 96));
	else
		gotoXY(0, 5 * randint(0, 96));

	speedx =(x < 320)? 5: -5;
	speedy =(y < 240)? 5: -5;
}

bool SpriteCore::collide(int xpos, int ypos) {
   return (x-size <= xpos) && (x + size >= xpos)&&(y-size <= ypos) && (y + size >= ypos);
}

void SpriteCore::wr_ctrl(int32_t cmd) {
	ctrl = cmd;
   io_write(base_addr, SPRITE_CTRL_REG, cmd);
}

void SpriteCore::changeColor(int32_t color) {
	ctrl &= 0xFFE7;
	ctrl |= color << 3;
	io_write(base_addr, SPRITE_CTRL_REG, ctrl);
}

void SpriteCore::changeSprite(int32_t sprite)	{
	ctrl &= 0xFFFC;
	ctrl |= sprite;
	io_write(base_addr, SPRITE_CTRL_REG, ctrl);
}


/**********************************************************************
 * OSD core methods
 *********************************************************************/
OsdCore::OsdCore(uint32_t core_base_addr) {
   base_addr = core_base_addr;
   set_color(0x0f0, CHROMA_KEY_COLOR);  // green on black
}
OsdCore::~OsdCore() {
}
// not used

void OsdCore::set_color(uint32_t fg_color, uint32_t bg_color) {
   io_write(base_addr, FG_CLR_REG, fg_color);
   io_write(base_addr, BG_CLR_REG, bg_color);
}

void OsdCore::wr_char(uint8_t x, uint8_t y, char ch, int reverse) {
   uint32_t ch_offset;
   uint32_t data;

   ch_offset = (y << 7) + (x & 0x07f);   // offset is concatenation of y and x
   if (reverse == 1)
      data = (uint32_t)(ch | 0x80);
   else
      data = (uint32_t) ch;
   io_write(base_addr, ch_offset, data);
   return;
}

void OsdCore::write(uint8_t x, uint8_t y, char ch[], int reverse) {
	int i=0;
	while (ch[i] != '\0')
	{
		wr_char(x+i, y, ch[i], reverse);
		i++;
	}
   return;
}

int OsdCore::write(uint8_t x, uint8_t y, int num, int reverse) {
   int digits[10] = {0,0,0,0,0,0,0,0,0,0};
   int numDigits = 0, temp = num;

   if (num == 0)
   {
	   wr_char(x, y, 48, reverse);
	   return 1;
   }

   while (temp > 0)
   {
      digits[numDigits] = temp%10;
      temp = temp/10;
      numDigits++;
   }


   char ch;
   int pos = 0;
   for (int i = numDigits-1; i > -1; i--, pos++)
   {
      ch = digits[i] + 48;
      wr_char(x+pos, y, ch, reverse);
   }
   return numDigits;
}

void OsdCore::clr_screen() {
   int x, y;

   for (x = 0; x < CHAR_X_MAX; x++)
      for (y = 0; y < CHAR_Y_MAX; y++) {
         wr_char(x, y, NULL_CHAR);
      }
   return;
}

void OsdCore::bypass(int by) {
   io_write(base_addr, BYPASS_REG, (uint32_t ) by);
}

/**********************************************************************
 * FrameCore core methods
 *********************************************************************/
FrameCore::FrameCore(uint32_t frame_base_addr) {
   base_addr = frame_base_addr;
}
FrameCore::~FrameCore() {
}
// not used

void FrameCore::wr_pix(int x, int y, int color) {
   uint32_t pix_offset;

   pix_offset = HMAX * y + x;
   io_write(base_addr, pix_offset, color);
   return;
}

void FrameCore::clr_screen(int color) {
   int x, y;

   for (x = 0; x < HMAX; x++)
      for (y = 0; y < VMAX; y++) {
         wr_pix(x, y, color);
      }
   return;
}

void FrameCore::bypass(int by) {
   io_write(base_addr, BYPASS_REG, (uint32_t ) by);
}

void FrameCore::writeArray(int colors[], int x, int y, int size)
{
    for (int i= 0; i < size; i++)
    {
        wr_pix(x+i, y, colors[i]);
    }
}

void FrameCore:: drawTombstone(int xpos, int ypos)
{
	int colors[4][15] = {
			{81,81,81,81,81,365,365,365,365,365,81,81,81,81,81},
			{81,81,81,81,365,365,365,365,365,365,365,81,81,81,81},
			{81,81,81,365,365,365,365,365,365,365,365,365,81,81,81},
			{81,81,365,365,365,365,365,365,365,365,365,365,365,81,81}
	};

	int words = 0;

	for (int i = 0; i < 4; i++)
		writeArray(colors[i],xpos, ypos+i, 15);

	for (int i = 0; i < 15; i++)
		for (int j = 4; j < 15; j++)
		{
			if (randint(0,10) < 5 && words < 20 && i > 3 && i < 13 && j > 6 && j < 13 && j%2)
			{
				words++;
				wr_pix(i+xpos,j+ypos, 0);
			}
			else
				wr_pix(i+xpos,j+ypos, 0b101101101);
		}


}

void FrameCore::drawBackground()
{
	clr_screen(0b001010001);

	int xs[32] = {195, 315, 520, 257, 80, 550, 460, 330, 65, 205, 475, 315, 105, 169, 620, 400, 80, 35, 530, 133, 285,
		  585, 442, 403, 260, 525, 115, 430, 378, 300, 575, 115};
	int ys[32] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315,
			330, 345, 360, 375, 390, 405, 402, 435, 450, 465};

	for (int i = 0; i < 32; i++)
		drawTombstone(xs[i],ys[i]);
}

// from AdaFruit
void FrameCore::plot_line(int x0, int y0, int x1, int y1, int color) {
   int dx, dy;
   int err, ystep, steep;

   if (x0 > x1) {
      swap(x0, x1);
      swap(y0, y1);
   }
   // slope is high
   steep = (abs(y1 - y0) > abs(x1 - x0)) ? 1 : 0;
   if (steep) {
      swap(x0, y0);
      swap(x1, y1);
   }
   dx = x1 - x0;
   dy = abs(y1 - y0);
   err = dx / 2;
   if (y0 < y1) {
      ystep = 1;
   } else {
      ystep = -1;
   }
   for (; x0 <= x1; x0++) {
      if (steep) {
         wr_pix(y0, x0, color);
      } else {
         wr_pix(x0, y0, color);
      }
      err = err - dy;
      if (err < 0) {
         y0 = y0 + ystep;
         err = err + dx;
      }
   }
}

void FrameCore::swap(int &a, int &b) {
   int tmp;

   tmp = a;
   a = b;
   b = tmp;
}

