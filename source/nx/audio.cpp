extern "C"{
	#include "audio.h"
}

#define BUFSIZE        8192    // Max dimension of audio buffer size
#define BUFSIZE_MONO   4096    // Dimension of audio buffer files for mono tracks
#define NSAMPLES       2048    // Number of samples for output
#define SOUND_CHANNELS    7    // PSVITA has 8 available audio channels, one is reserved for musics

void PHL_AudioInit()
{

}

void PHL_AudioClose()
{

}

//Each system can use a custom music file format
PHL_Music PHL_LoadMusic(char* fname, int loop)
{	
	PHL_Music mus;
	sprintf(mus.filepath, fname);
	mus.loop = loop;
	mus.audiobuf = NULL;
	mus.audioThread = 0xFF;
	return mus;
}

PHL_Sound PHL_LoadSound(char* fname)
{
	PHL_Sound snd;
	sprintf(snd.filepath, fname);
	snd.loop = 0;
	snd.audiobuf = NULL;
	snd.audioThread = 0xFF;
	return snd;
}

void PHL_PlayMusic(PHL_Music snd)
{
	
}

void PHL_PlaySound(PHL_Sound snd, int channel)
{
	
}

void PHL_StopMusic()
{
	
}

void PHL_StopSound(PHL_Sound snd, int channel)
{
	
}

void PHL_FreeMusic(PHL_Music snd)
{
	
}

void PHL_FreeSound(PHL_Sound snd)
{
	
}