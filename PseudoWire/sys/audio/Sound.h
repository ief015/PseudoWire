#pragma once

#include "AudioManager.h"
#include "SoundBuffer.h"
#include "OpenAL.h"
#include "../Platform.h"

namespace sys
{
namespace audio
{

class Sound
{
public:
	Sound();
	Sound(SoundBuffer* buffer);
	~Sound();

	//Change the buffer. Returns OpenAL Error, or 0 if no error occurs.
	s32 SetBuffer(SoundBuffer* buf);

	//Retrieve the buffer
	SoundBuffer* GetBuffer();

	//Play the sound
	void Play();

	//Stop the sound
	void Stop();

	//Pause the sound
	void Pause();

	//Get the current playing position in seconds
	f32 PlayingOffsetSec() const;

	//Get the current playing position by samples
	s32 PlayingOffset() const;

	//Set the current playing position in seconds
	void PlayingOffsetSec(f32 pos);

	//Set the current playing position by samples
	void PlayingOffset(s32 pos);

	//Is the sound currently playing?
	bool IsPlaying() const;

	//Set volume (0.0 - 1.0)
	void Volume(f32 value);

	//Get volume (0.0 - 1.0)
	f32 Volume() const;

	//Set the pitch (1.0 is default)
	void Pitch(f32 value);

	//Get the pitch (1.0 is default)
	f32 Pitch() const;

	//Set the sound to loop
	void Loop(bool loop);

	//Get the sound to loop
	bool Loop() const;

private:
	ALuint _source;
	SoundBuffer* _buffer;
};

}
}