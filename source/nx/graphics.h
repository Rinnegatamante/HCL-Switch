//VITA graphics.h
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <switch.h>

typedef struct sw_texture{
	uint8_t* data;
	uint16_t w;
	uint16_t h;
} sw_texture;

#define PHL_Surface sw_texture*

#define PHL_RGB uint32_t

typedef struct {
	int tileX[16][12];
	int tileY[16][12];
} PHL_Background;

void PHL_GraphicsInit();
void PHL_GraphicsExit();

void PHL_StartDrawing();
void PHL_EndDrawing();

void PHL_ForceScreenUpdate();

void PHL_SetDrawbuffer(sw_texture* surf);
void PHL_ResetDrawbuffer();

uint32_t PHL_NewRGB(uint8_t r, uint8_t g, uint8_t b);
void PHL_SetColorKey(sw_texture* surf, uint8_t r, uint8_t g, uint8_t b);

sw_texture* PHL_NewSurface(uint32_t w, uint32_t h);
void PHL_FreeSurface(sw_texture* surf);

sw_texture* PHL_LoadBMP(int index);

void PHL_DrawRect(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t col);

void PHL_DrawSurface(int32_t x, int32_t y, sw_texture* surf);
void PHL_DrawSurfacePart(int32_t x, int32_t y, int32_t cropx, int32_t cropy, int32_t cropw, int32_t croph, sw_texture*);

void PHL_DrawBackground(PHL_Background back, PHL_Background fore);
void PHL_UpdateBackground(PHL_Background back, PHL_Background fore);

void PHL_SwapBorder();

#endif