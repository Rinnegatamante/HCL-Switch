#include "input.h"

void updateKey(Button* btn, uint64_t state);

int enterButton = 0;

void PHL_ScanInput()
{	
	hidScanInput();
	uint64_t kDown = hidKeysHeld(CONTROLLER_P1_AUTO);
		
	updateKey(&btnUp, kDown & KEY_DUP);
	updateKey(&btnDown, kDown & KEY_DDOWN);
	updateKey(&btnLeft, kDown & KEY_DLEFT);
	updateKey(&btnRight, kDown & KEY_DRIGHT);
	
	updateKey(&btnStart, kDown & KEY_PLUS);
	updateKey(&btnSelect, kDown & KEY_MINUS);
	
	updateKey(&btnFaceRight, kDown & KEY_B);
	updateKey(&btnFaceDown, kDown & KEY_A);
	updateKey(&btnFaceLeft, kDown & KEY_Y);
	
	updateKey(&btnL, kDown & KEY_L);
	updateKey(&btnR, kDown & KEY_R);
	
	updateKey(&btnAccept, kDown & KEY_A);
	updateKey(&btnDecline, kDown & KEY_B);
	
	updateKey(&btnSwap, kDown & KEY_X);
	
}

void updateKey(Button* btn, uint64_t state)
{
	if (state) {
		if (btn->held == 1) {
			btn->pressed = 0;
		}else{
			btn->pressed = 1;
		}
		btn->held = 1;
		btn->released = 0;
	}else{
		if (btn->held == 1) {
			btn->released = 1;
		}else{
			btn->released = 0;
		}
		btn->held = 0;
		btn->pressed = 0;
	}
}