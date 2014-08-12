#pragma once

#include "OpenAL.h"
#include "../Platform.h"

namespace sys
{
namespace audio
{

class AudioManager : public sys::Singleton<AudioManager>
{
public:

	~AudioManager()
	{
		if(_context)
			alcDestroyContext(_context);
	}

	s32 Init(const char* deviceName = NULL, const s32* attributes = NULL)
	{
		this->_context = alcCreateContext(alcOpenDevice(deviceName), attributes);

		if(alGetError()) return 1;
		
		alcMakeContextCurrent(_context);

		if(alGetError()) return 1;

		return 0;
	}

private:
	ALCcontext* _context;
};

}
}