extern "C"{
	#include "audio.h"
}

#include "audio_generic.h"
#include <vector>
#include <cstdlib>
#include <malloc.h>

#define ALIGN_TO(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

std::unique_ptr<GenericAudio> instance;
Thread audio_thread;
Mutex audio_mutex;

void GenericAudio::LockMutex() const {
	mutexLock((Mutex*)&audio_mutex);
}

void GenericAudio::UnlockMutex() const {
	mutexUnlock((Mutex*)&audio_mutex);
}

const int bytes_per_sample = 4;
const int samples_per_buf = 4096;
const int buf_size = samples_per_buf * bytes_per_sample;
bool termStream = false;

void switch_audio_thread(void*) {
	uint8_t *buffer1 = (uint8_t*)memalign(0x1000, ALIGN_TO(buf_size, 0x1000));
	uint8_t *buffer2 = (uint8_t*)memalign(0x1000, ALIGN_TO(buf_size, 0x1000));
	uint32_t released_count;
	
	AudioOutBuffer source_buffers[2], *released_buffer;
	
	// Init audio buffers
	source_buffers[0].buffer = buffer1;
	source_buffers[1].buffer = buffer2;
	
	for (int i = 0; i < 2; i++){
		source_buffers[i].next = NULL;
		source_buffers[i].buffer_size = buf_size;
		source_buffers[i].data_size = buf_size;
		source_buffers[i].data_offset = 0;
		instance->LockMutex();
		instance->Decode((uint8_t*)source_buffers[i].buffer, buf_size);
		instance->UnlockMutex();
		audoutAppendAudioOutBuffer(&source_buffers[i]);
	}
	
	for(;;) {
		// A pretty bad way to close thread
		if (termStream) {
			termStream = false;
			free(buffer1);
			free(buffer2);
			return;
		}

		audoutWaitPlayFinish(&released_buffer, &released_count, U64_MAX);
		instance->LockMutex();
		instance->Decode((uint8_t*)released_buffer->buffer, buf_size);
		instance->UnlockMutex();
		audoutAppendAudioOutBuffer(released_buffer);

	}
}

void PHL_AudioInit()
{
	instance.reset(new GenericAudio());
	audoutInitialize();
	audoutStartAudioOut();
	mutexInit(&audio_mutex);
	threadCreate(&audio_thread, switch_audio_thread, NULL, 0x10000, 0x2B, -2);
	
	instance->SetFormat(48000, AudioDecoder::Format::S16, 2);
	
	threadStart(&audio_thread);
	
}

void PHL_AudioClose()
{
	// Closing streaming thread
	termStream = true;
	threadWaitForExit(&audio_thread);
	
	// Deleting thread
	threadClose(&audio_thread);
	
	// Terminating audio API
	audoutStopAudioOut();
	audoutExit();
}

//Each system can use a custom music file format
PHL_Music PHL_LoadMusic(char* fname, int loop)
{	
	PHL_Music mus;
	sprintf(mus.filepath, "%s.mp3", fname);
	mus.loop = loop;
	return mus;
}

PHL_Sound PHL_LoadSound(char* fname)
{
	PHL_Sound snd;
	sprintf(snd.filepath, fname);
	snd.loop = 0;
	return snd;
}

void PHL_PlayMusic(PHL_Music snd)
{
	instance->BGM_Play(snd.filepath, 100, 100, 0, snd.loop);
}

void PHL_PlaySound(PHL_Sound snd, int channel)
{
	snd.chn = instance->SE_Play(snd.filepath, 100, 100);
}

void PHL_StopMusic()
{
	instance->BGM_Stop();
}

void PHL_StopSound(PHL_Sound snd, int channel)
{
	if (snd.chn > 0) instance->SE_Stop(snd.chn);
}

void PHL_FreeMusic(PHL_Music snd)
{
	
}

void PHL_FreeSound(PHL_Sound snd)
{
	
}