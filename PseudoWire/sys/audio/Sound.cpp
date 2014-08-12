#include "Sound.h"

#include "AudioManager.h"
#include "SoundBuffer.h"
#include "OpenAL.h"
#include "../Platform.h"

using namespace sys::audio;

Sound::Sound()
: _source(NULL)
{
}

Sound::Sound(SoundBuffer* buffer)
: _source(NULL)
{
	SetBuffer(buffer);
}

Sound::~Sound()
{
	if(_source)
		alDeleteSources(1, &_source);
}

s32 Sound::SetBuffer(SoundBuffer* buf)
{
	s32 alerr = 0;

	_buffer = buf;

	if(!_source)
	{
		alGenSources(1, &_source);
		if(alerr = alGetError())
		{
			sys::Dbg::Printf("\nOpenAL - Error %i - Could not generate source.", alerr);
			return alerr;
		}
	}

	alSourcei(_source, AL_BUFFER, *_buffer);

	if(alerr = alGetError())
	{
		sys::Dbg::Printf("\nOpenAL - Error %i - Could not send buffer to source.", alerr);
		return alerr;
	}

	return alerr;
}

SoundBuffer* Sound::GetBuffer()
{
	return _buffer;
}

void Sound::Play()
{
	if(_source)
		alSourcePlay(_source);
}

void Sound::Stop()
{
	if(_source)
		alSourceStop(_source);
}

void Sound::Pause()
{
	if(_source)
		alSourcePause(_source);
}

f32 Sound::PlayingOffsetSec() const
{
	f32 ret = 0.0f;
	if(_source)
		alGetSourcef(_source, AL_SEC_OFFSET, &ret);
	return ret;
}

s32 Sound::PlayingOffset() const
{
	s32 ret = 0;
	if(_source)
		alGetSourcei(_source, AL_SAMPLE_OFFSET, &ret);
	return ret;
}

void Sound::PlayingOffsetSec(f32 pos)
{
	if(_source)
		alSourcef(_source, AL_SEC_OFFSET, pos);
}

void Sound::PlayingOffset(s32 pos)
{
	if(_source)
		alSourcei(_source, AL_SAMPLE_OFFSET, pos);
}

bool Sound::IsPlaying() const
{
	s32 v = 0;
	alGetSourcei(_source, AL_PLAYING, &v);
	return (v==0?false:true);
}

void Sound::Volume(f32 value)
{
	if(_source)
		alSourcef(_source, AL_GAIN, value);
}

f32 Sound::Volume() const
{
	f32 v = 0.0f;
	if(_source)
		alGetSourcef(_source, AL_GAIN, &v);
	return v;
}

void Sound::Pitch(f32 value)
{
	if(_source)
		alSourcef(_source, AL_PITCH, value);
}

f32 Sound::Pitch() const
{
	f32 v = 0.0f;
	if(_source)
		alGetSourcef(_source, AL_PITCH, &v);
	return v;
}

void Sound::Loop(bool loop)
{
	if(_source)
		alSourcei(_source, AL_LOOPING, (loop?1:0));
}

bool Sound::Loop() const
{
	s32 v = 0;
	if(_source)
		alGetSourcei(_source, AL_LOOPING, &v);
	return (v==0?false:true);
}
