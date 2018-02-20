#include "system.h"
#include <switch.h>
#include "../PHL.h"
#include <stdlib.h>
#include <stdio.h>

uint32_t white;
int quitGame = 0;

int PHL_MainLoop(){	
	if (quitGame == 1) return 0;
	return 1;
}

void PHL_ConsoleInit(){
	white = RGBA8(0xFF, 0xFF, 0xFF, 0xFF);
}

void PHL_GameQuit(){
	quitGame = 1;
}

void PHL_ErrorScreen(char* message){	
	PHL_GameQuit();
}