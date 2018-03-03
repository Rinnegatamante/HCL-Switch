/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EP_AUDIO_GENERIC_H
#define EP_AUDIO_GENERIC_H

#include "audio.h"
#include "audio_decoder.h"
#include "audio_secache.h"

/**
 * A software implementation for handling EasyRPG Audio utilizing the
 * AudioDecoder for BGM and AudioSeCache for fast SE playback.
 *
 * Inheriting implementations have to:
 * 1. Init the audio system in the constructor (and deinit in destructor)
 * 2. Start a thread (or a callback) which invokes the Decode function to
 *    fill the output buffer and controls access to the audio api of the
 *    target platform.
 * 3. Initialize the "output_format" (must match the format of the hardware)
 * 4. Implement LockMutex and UnlockMutex. Locking and Unlocking when
 *    calling Decode must be done manually.
 * 5. Implement update function (optional)
 */
struct GenericAudio{
public:
	GenericAudio();
	virtual ~GenericAudio();

	void BGM_Play(std::string const& file, int volume, int pitch, int fadein, bool loop);
	void BGM_Pause();
	void BGM_Resume();
	void BGM_Stop();
	bool BGM_PlayedOnce() const;
	bool BGM_IsPlaying() const;
	unsigned BGM_GetTicks() const;
	void BGM_Fade(int fade);
	void BGM_Volume(int volume);
	void BGM_Pitch(int pitch);
	int SE_Play(std::string const& file, int volume, int pitch);
	void SE_Stop(int chn);
	virtual void Update();

	void SetFormat(int frequency, AudioDecoder::Format format, int channels);

	virtual void LockMutex() const;
	virtual void UnlockMutex() const;

	void Decode(uint8_t* output_buffer, int buffer_length);

private:
	struct BgmChannel {
		std::unique_ptr<AudioDecoder> decoder;
		bool paused;
		bool stopped;
	};
	struct SeChannel {
		AudioSeRef se;
		size_t buffer_pos;
		int volume;
		bool paused;
		bool stopped;
		bool finished;
	};
	struct Format {
		int frequency;
		AudioDecoder::Format format;
		int channels;
	};
	Format output_format = {0};

	bool PlayOnChannel(BgmChannel& chan,std::string const& file, int volume, int pitch, int fadein, bool loop);
	bool PlayOnChannel(SeChannel& chan,std::string const& file, int volume, int pitch);

	static const unsigned nr_of_se_channels=31;
	static const unsigned nr_of_bgm_channels=2;

	static BgmChannel BGM_Channels[nr_of_bgm_channels];
	static SeChannel SE_Channels[nr_of_se_channels];
	static bool BGM_PlayedOnceIndicator;
	static bool Muted;

	static std::vector<int16_t> sample_buffer;
	static std::vector<uint8_t> scrap_buffer;
	static unsigned scrap_buffer_size;
	static std::vector<float> mixer_buffer;
};

#endif //EASYRPG_AUDIOGENERIC_H_
