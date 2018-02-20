/*
PHL stands for Portable Homebrew Library
*/
#ifndef PHL_H
#define PHL_H

#ifdef _3DS
	#include "3ds/system.h"
	#include "3ds/graphics.h"
	#include "3ds/input.h"
	#include "3ds/audio.h"
#endif

#ifdef _WII
	#include "wii/system.h"
	#include "wii/graphics.h"
	#include "wii/input.h"
	#include "wii/audio.h"
#endif

#ifdef _PSP
	#include "psp/system.h"
	#include "psp/graphics.h"
	#include "psp/input.h"
	#include "psp/audio.h"
#endif

#ifdef _PSP2
	#include "vita/system.h"
	#include "vita/graphics.h"
	#include "vita/input.h"
	#include "vita/audio.h"
#endif

#ifdef SWITCH
	#include "nx/system.h"
	#include "nx/graphics.h"
	#include "nx/input.h"
	#include "nx/audio.h"
#endif

typedef struct {
	int x, y, w, h;
} PHL_Rect;

void PHL_Init();
void PHL_Deinit();

int WHITE,
	RED,
	YELLOW;

PHL_Surface PHL_LoadQDA(char* fname);
void PHL_DrawTextBold(char* txt, int dx, int dy, int col);
void PHL_DrawTextBoldCentered(char* txt, int dx, int dy, int col);

#endif