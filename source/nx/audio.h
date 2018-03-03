#ifndef AUDIO_H
#define AUDIO_H

#include <switch.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Music block struct
typedef struct DecodedMusic
{
	int loop;
	char filepath[256];
	int chn;
} DecodedMusic;

typedef DecodedMusic PHL_Sound;
typedef DecodedMusic PHL_Music;

void PHL_AudioInit();
void PHL_AudioClose();

PHL_Music PHL_LoadMusic(char* fname, int loop); //Same as PHL_LoadSound, but expects a file name without extension
PHL_Sound PHL_LoadSound(char* fname);

void PHL_PlayMusic(PHL_Music snd);
void PHL_PlaySound(PHL_Sound snd, int channel);

void PHL_StopMusic();
void PHL_StopSound(PHL_Sound snd, int channel);

void PHL_FreeMusic(PHL_Music snd);
void PHL_FreeSound(PHL_Sound snd);

#endif