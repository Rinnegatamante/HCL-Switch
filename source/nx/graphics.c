//NX graphics.c
#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>
#include "../game.h"
#include "../qda.h"
#include "../hero.h"

uint32_t *framebuf = NULL;
uint32_t fb_w, fb_h;
uint8_t drawing_phase = 0;
extern int in_game;

void set_pixel(sw_texture* texture, int x, int y, unsigned int color){
	if ((x < 0) || (y < 0) || (x >= texture->w) || (y >= texture->h)) return;
	uint32_t *data = (uint32_t*)texture->data;
	data[x+y*texture->w] = color;
}

void draw_rectangle(int x, int y, uint32_t w, uint32_t h, unsigned int color){
	uint32_t *orig = framebuf;
	if (x + w > fb_w) w = fb_w - x;
	if (y + h > fb_h) h = fb_h - y;
	if (x < 0){
		w += x;
		x = 0;
	}
	if (y < 0){
		h += y;
		y = 0;
	}
	int i, j;
	for (i=0;i<h;i++){
		framebuf = orig + x + (y + i) * fb_w;
		for (j=0;j<w;j++){
			framebuf[j] = color;
		}
	}
}

void draw_texture_part_scale_3x(const sw_texture *texture, int x, int y, int tex_x, int tex_y, int tex_w, int tex_h){
	uint32_t *tex_data = (uint32_t*)texture->data;
	uint32_t *orig = framebuf;
	uint32_t w = tex_w - tex_x;
	uint32_t h = tex_h - tex_y;
	if ((x + w) > fb_w) w = fb_w - x;
	if ((y + h) > fb_h) h = fb_h - y;
	if (x < 0){
		w += x;
		tex_x -= x;
		x = 0;
	}
	if (y < 0){
		h += y;
		tex_y -= y;
		y = 0;
	}
	int i, j;
	for (i=0;i<h;i++){
		framebuf = orig + x + (y + i) * fb_w;
		for (j=0;j<w;j++){
			if (&framebuf[j] > &orig[fb_w*fb_h]) break;
			if (&framebuf[j] < orig) continue;
			uint32_t tex_offs = tex_x + j + (tex_y + i) * tex_w;
			if (tex_offs > tex_w * tex_h) continue;
			framebuf[j] = tex_data[tex_offs];
		}
	}
}

void draw_texture_scale_3x(const sw_texture *texture, int x, int y){
	draw_texture_part_scale_3x(texture, x, y, 0, 0, texture->w, texture->h);
}

//One time graphics setup
void PHL_GraphicsInit()
{
	gfxInitDefault();
}

void PHL_SwapBorder()
{

}

void PHL_StartDrawing()
{
	if (drawing_phase) return;
	framebuf = (uint32_t*)gfxGetFramebuffer(&fb_w, &fb_h);
	drawing_phase = 1;
}

void PHL_EndDrawing()
{	
	if (!drawing_phase) return;
	gfxFlushBuffers();
	gfxSwapBuffers();
	gfxWaitForVsync();
	drawing_phase = 0;
}

void PHL_GraphicsExit()
{
	if (drawing_phase) PHL_EndDrawing();
	gfxExit();
}

void PHL_ForceScreenUpdate()
{	
	
}

void PHL_SetDrawbuffer(sw_texture* surf)
{

}

void PHL_ResetDrawbuffer()
{

}

uint32_t PHL_NewRGB(uint8_t r, uint8_t g, uint8_t b)
{
	return (r | (g << 8) | (b << 16) | (0xFF << 24));
}

void PHL_SetColorKey(sw_texture* surf, uint8_t r, uint8_t g, uint8_t b)
{

}

sw_texture *PHL_NewSurface(uint16_t w, uint16_t h)
{	
	sw_texture *r = (sw_texture*)malloc(sizeof(sw_texture));
	r->data = (uint8_t*)malloc(w*h*4);
	r->w = w;
	r->h = h;
	return r;
}

void PHL_FreeSurface(sw_texture *surf)
{
	if (surf != NULL) {
		free(surf->data);
		free(surf);
		surf = NULL;
	}
}

sw_texture* PHL_LoadBMP(int index)
{	
	sw_texture* result = NULL;
	unsigned char* QDAFile = (unsigned char*)malloc(headers[index].size);
	
	FILE* f = NULL;
	f = fopen("./bmp.qda", "rb");
	if (f != NULL){
		
		//Load QDA file data
		fseek(f, headers[index].offset, SEEK_SET);
		fread(QDAFile, headers[index].size, 1, f);
		fclose(f);
		
		//Read data from header
		unsigned short w, h;

		memcpy(&w, &QDAFile[18], 2);
		memcpy(&h, &QDAFile[22], 2);

		result = PHL_NewSurface(w, h);
		
		//Load Palette
		uint32_t palette[20][18];
		int dx, dy;
		int count = 0;
		for (dx = 0; dx < 20; dx++) {
			for (dy = 0; dy < 16; dy++) {
				palette[dx][dy] = PHL_NewRGB(QDAFile[54 + count + 2], QDAFile[54 + count + 1], QDAFile[54 + count]);
				count += 4;
			}
		}
		uint32_t alphaKey = palette[0][0];
		
		//Darkness special case
		if (index == 27) alphaKey = PHL_NewRGB(0, 0, 0);
		
		//Edit surface pixels
		for (dx = w; dx > 0; dx--) {
			for (dy = h; dy >= 0; dy--) {		
				int pix = w - dx + w * dy;

				int px = QDAFile[1078 + pix] / 16;
				int py = QDAFile[1078 + pix] % 16;
				
				if (palette[px][py] == alphaKey) {
					set_pixel(result, w - dx, h - dy - 1, PHL_NewRGB(0, 0, 0));
				}else{
					set_pixel(result, w - dx, h - dy - 1, palette[px][py]);
				}			
			}
		}
		
	}
	
	free(QDAFile);
	
	return result;
}

void PHL_DrawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t color)
{	
	if (!drawing_phase) return;	
	draw_rectangle(120 + x, y, w, h, RGBA8((color) & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF, (color >> 24) & 0xFF));
}

void PHL_DrawSurface(int16_t x, int16_t y, sw_texture* surf)
{
	if (!drawing_phase) return;
	
	//Quake Shake
	if (quakeTimer > 0) {
		int val = quakeTimer % 4;
		if (val == 0) {
			y -= 2;
		}else if (val == 2) {
			y += 2;
		}
	}
	
	draw_texture_scale_3x(surf, x, y);
}

void PHL_DrawSurfacePart(int16_t x, int16_t y, int16_t cropx, int16_t cropy, int16_t cropw, int16_t croph, sw_texture* surf)
{	
	if (!drawing_phase) return;
	if (surf != NULL)
	{
		//Quake Shake
		if (quakeTimer > 0) {
			int val = quakeTimer % 4;
			if (val == 0) {
				y -= 2;
			}else if (val == 2) {
				y += 2;
			}
		}
		
		draw_texture_part_scale_3x(surf, 120+x, 32+y, cropx/2, cropy/2, cropw/2, croph/2);

	}
}

void PHL_DrawBackground(PHL_Background back, PHL_Background fore)
{
	int xx, yy;
	
	for (yy = 0; yy < 12; yy++)
	{
		for (xx = 0; xx < 16; xx++)
		{
			//Draw Background tiles
			PHL_DrawSurfacePart(xx * 40, yy * 40, back.tileX[xx][yy] * 40, back.tileY[xx][yy] * 40, 40, 40, images[imgTiles]);
			
			//Only draw foreground tile if not a blank tile
			if (fore.tileX[xx][yy] != 0 || fore.tileY[xx][yy] != 0) {
				PHL_DrawSurfacePart(xx * 40, yy * 40, fore.tileX[xx][yy] * 40, fore.tileY[xx][yy] * 40, 40, 40, images[imgTiles]);
			}
		}
	}
}

void PHL_UpdateBackground(PHL_Background back, PHL_Background fore)
{
	
}