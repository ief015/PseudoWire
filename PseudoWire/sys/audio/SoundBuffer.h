#pragma once

#include <stdio.h>
#include <sstream>
#include "../Platform.h"

#include "OpenAL.h"

#define _SYS_AUDIO_SOUNDBUFFER__ENABLE_SNDFILE_		//Common formats
//#define _SYS_AUDIO_SOUNDBUFFER__ENABLE_MODPLUG_	//MOD-Likes
//#define _SYS_AUDIO_SOUNDBUFFER__ENABLE_DUMB_		//MOD-Likes
#define _SYS_AUDIO_SOUNDBUFFER__ENABLE_DECMPA_		//MP3 (potentially a legal threat D:)

#ifdef _SYS_AUDIO_SOUNDBUFFER__ENABLE_SNDFILE_
#include "sndfile.h"
#endif

#ifdef _SYS_AUDIO_SOUNDBUFFER__ENABLE_MODPLUG_
#include "modplug.h"
#endif

#ifdef _SYS_AUDIO_SOUNDBUFFER__ENABLE_DUMB_
#include "dumb.h"
#endif

#ifdef _SYS_AUDIO_SOUNDBUFFER__ENABLE_DECMPA_
#include "decmpa.h"
#endif


namespace sys
{
namespace audio
{

class SoundBuffer
{
public:
	SoundBuffer()
		: _buffer(NULL)
		, _data(NULL)
	{
	}

	SoundBuffer(const char* filename)
		: _buffer(NULL)
		, _data(NULL)
	{
		SetSrc(filename);
	}

	SoundBuffer(short* data, u64 szData, s32 freq = 44100)
		: _buffer(NULL)
		, _data(NULL)
	{
		SetSrc(data, szData, freq);
	}

	virtual ~SoundBuffer()
	{
		if(_buffer)
			alDeleteBuffers(1, &_buffer);
		_buffer = NULL;

		if(_data)
			delete[] _data;
		_data = NULL;
	}

	//OpenAL Buffer ID (bid)
	inline operator s32()
	{
		return _buffer;
	}
	

	//Set or change the source of the sound
	bool SetSrc(const char* filename)
	{
		sys::Dbg::PrintfDBG("\nLoading sound: %s", filename);
		{
			//does the file exist?
			FILE* f = std::fopen(filename, "r");
			if(!f)
			{
				sys::Dbg::Printf("\nSound file could not be loaded, does not exist", filename);
				return false;
			}
			else std::fclose(f);
		}

		s32 alerr = 0;

		if(alerr = alGetError())
		{
			sys::Dbg::Printf("\nOpenAL - Error %i - Could not begin loading audio", alerr);
			return false;
		}

		if(!_buffer)
		{
			alGenBuffers(1, &_buffer);
			if(alerr = alGetError())
			{
				sys::Dbg::Printf(false, "\nOpenAL - Error %i - Could not generate audio buffer", alerr);
				if(_buffer)
					alDeleteBuffers(1, &_buffer);
				_buffer = NULL;
				return false;
			}
		}

		// get data ------------------
		bool gotData = false;
		
#ifdef _SYS_AUDIO_SOUNDBUFFER__ENABLE_SNDFILE_	 
		//SNDFILE -- Common audio files... WAV, OGG, FLAC, RAW, AIFF, SND, SGI, AIFC, Sun, DEC, AU, PAF, IFF, SVX, SF, VOC, W64, MAT4, MAT5, PVF, XI, HTK, CAF, SD2
		//        -- It does not support MP3!! That's what DECMPA is for. Supports almost everything that isn't MOD-like or MP3
		while(!gotData)
		{
			sys::Dbg::PrintfDBG("\nSNDFILE - Loading sound");

			SNDFILE* file = NULL;
			SF_INFO fileInfo;
			s32 err = 0;

			file = sf_open(filename, SFM_READ, &fileInfo);

			if(!file || sf_error(file))
			{
				sys::Dbg::PrintfDBG("\nSNDFILE Could not load this file");
				if(_data)
					delete[] _data;
				if(file)
					sf_close(file);
				break;
			}

			_channels = fileInfo.channels;
			_len = fileInfo.frames * _channels;
			_freq = fileInfo.samplerate;

			if(_data)
				delete[] _data;
			_data = new s16[_len];

			sf_read_short(file, _data, _len);
			if(err = sf_error(file))
			{
				sys::Dbg::Printf("\nSNDFILE - Error %i - Could not get audio data", err);
				if(_data)
					delete[] _data;
				sf_close(file);
				break;
			}

			sf_close(file);

			//success
			gotData = true;
		}
#endif
		
#ifdef _SYS_AUDIO_SOUNDBUFFER__ENABLE_MODPLUG_
		// BROKEN
		//libmodplug -- for MOD-like formats... MOD, S3M, XM, IT, 669, AMF (both of them), AMS, DBM, DMF, DSM, FAR, MDL, MED, MTM, OKT, PTM, STM, ULT, UMX, MT2, PSM
		while(!gotData)
		{
			sys::Dbg::PrintfDBG("\nMODPLUG - Loading sound");

			//libmodplug doesn't do file IO... we have to do this ourselves.         assholes.
			void* f_buf = NULL;
			s64 f_buf_sz = 0;
			{
				FILE* f = std::fopen( filename, "r" );

				if(!f)
				{
					sys::Dbg::Printf("\nMODPLUG - Sound file could not be loaded, does not exist", filename);
					break;
				}

				std::fseek(f , 0 , SEEK_END);
				f_buf_sz = std::ftell(f);
				std::rewind(f);

				f_buf = malloc(f_buf_sz);
				std::fread(f_buf, 1, f->_bufsiz, f);
				std::fclose(f);
			}

			ModPlugFile* file = ModPlug_Load(f_buf, f_buf_sz);

			if(!file)
			{
				sys::Dbg::PrintfDBG("\nMODPLUG Could not load this file");
				if(f_buf)
					free(f_buf);
				if(_data)
					delete[] _data;
				break;
			}

			//hacky test if it is a legit mod...
			if(0){
				char _name[0xff]; ModPlug_SampleName(file, 0xff, _name);
				s32 _numsamples = ModPlug_NumSamples(file);
				s32 _numchannels = ModPlug_NumChannels(file);
				s32 _type = ModPlug_GetModuleType(file);

				if( (_numsamples % _numchannels != 0) /* || _name == "" */ || _type == 0 )
				{
					sys::Dbg::PrintfDBG("\nMODPLUG Could not load this file");
					if(f_buf)
						free(f_buf);
					if(_data)
						delete[] _data;
					break;
				}
			}

			_len = ModPlug_NumSamples(file);
			
			ModPlug_Settings* file_settings = (ModPlug_Settings*)file;
			ModPlug_GetSettings(file_settings);

			if(!file_settings)
			{
				sys::Dbg::Printf("\nMODPLUG - Could not get file settings");
				if(f_buf)
					free(f_buf);
				if(_data)
					delete[] _data;
				break;
			}

			_channels = file_settings->mChannels;
			_freq = file_settings->mFrequency;

			if(_data)
				delete[] _data;
			_data = new s16[_len];

			s64 offset = 0;
			s32 bytesRead = -1;
			while(bytesRead)
			{
				bytesRead = ModPlug_Read(file, &(_data[offset/sizeof(s16)]), 56);
				offset += bytesRead;
			}
			
			free(f_buf);
			ModPlug_Unload(file);

			//success
			gotData = true;
		}
#endif

#ifdef _SYS_AUDIO_SOUNDBUFFER__ENABLE_DUMB_
		//BROKEN
		//dumb -- for MOD-like formats... MOD, S3M, XM, IT
		while(!gotData)
		{
			//manually perform IO	(hot)
			void* f_buf = NULL;
			s64 f_buf_sz = 0;
			{
				FILE* f = std::fopen( filename, "r" );

				if(!f)
				{
					sys::Dbg::Printf("\nDUMB - Sound file could not be loaded, does not exist", filename);
					break;
				}

				std::fseek(f , 0 , SEEK_END);
				f_buf_sz = std::ftell(f);
				std::rewind(f);

				f_buf = malloc(f_buf_sz);
				std::fread(f_buf, 1, f->_bufsiz, f);
				std::fclose(f);
			}

			DUMBFILE* file = dumbfile_open_memory((char*)f_buf, f_buf_sz);
			
			if(!file)
				break;

			DUH* duh = NULL;

			dumbfile_skip(file, 0);
			if(!duh)
				duh = dumb_read_mod(file);

			dumbfile_skip(file, 0);
			if(!duh)
				duh = dumb_read_xm(file);

			dumbfile_skip(file, 0);
			if(!duh)
				duh = dumb_read_s3m(file);

			dumbfile_skip(file, 0);
			if(!duh)
				duh = dumb_read_it(file);

			if(!duh)
			{
				dumbfile_close(file);
				if(f_buf)
					delete f_buf;
				break;
			}

			DUH_SIGRENDERER* sigdata = duh_start_sigrenderer(duh, 0, 2, 0);

			_channels = duh_sigrenderer_get_n_channels(sigdata);
			_len = duh_get_length(duh);
			_freq = 44100;

			//get samples
			if(_data)
				delete[] _data;
			_data = new s16[_len];

			duh_render(sigdata, 16, 0, 1.0f, 65536.0f / _freq, _len/sizeof(s16), _data);

			duh_end_sigrenderer(sigdata);

			unload_duh(duh);
			dumbfile_close(file);
			delete f_buf;

			//success
			gotData = true;
		}
#endif

#ifdef _SYS_AUDIO_SOUNDBUFFER__ENABLE_DECMPA_ 
		//DECMPA -- for mp3 only.
		while(!gotData) //use while so we can break out of this scope early
		{
			sys::Dbg::PrintfDBG("\nDECMPA - Loading sound");

			s32 err = 0;
			void* decObj = NULL;

			//create buffer
			err = DecMPA_CreateUsingFile(&decObj, filename, DECMPA_VERSION);
			
			if(!decObj || err < 0)
			{
				sys::Dbg::Printf("\nDECMPA - Error %i - Could not load this file", err);
				if(_data)
					delete _data;
				break;
			}

			//get mpeg info
			s64 bytesDecoded = 0;
			_len = 0;
			while(err == DECMPA_OK)
			{
				err = DecMPA_DecodeNoData(decObj, &bytesDecoded);
				_len += bytesDecoded;
			}
			_len /= sizeof(s16);

			if(err < 0)
			{
				sys::Dbg::PrintfDBG("\nDECMPA - Error %i - Could not decode", err);
				if(_data)
					delete _data;
				DecMPA_Destroy(decObj);
				break;
			}

			s64 samplesPerFrame = 1125;
			DecMPA_MPEGHeader header;
			err = DecMPA_GetMPEGHeader(decObj, &header);
			if(err < 0)
			{
				sys::Dbg::Printf("\nDECMPA - Error %i - WARNING: Could not read audio header", err);
			}
			else samplesPerFrame = header.nDecodedSamplesPerFrame;

			DecMPA_OutputFormat format;
			err = DecMPA_GetOutputFormat(decObj, &format);
			if(err < 0)
			{
				sys::Dbg::Printf("\nDECMPA - Error %i - Could not read audio output format", err);
				if(_data)
					delete _data;
				DecMPA_Destroy(decObj);
				break;
			}

			_channels = format.nChannels;
			_freq = format.nFrequency;

			//get samples
			if(_data)
				delete[] _data;
			_data = new s16[_len];

			s64 offset = 0;
			
			DecMPA_SeekToTime(decObj, 0);
			while(err == DECMPA_OK)
			{
				err = DecMPA_Decode(decObj, &(_data[offset/sizeof(s16)]), samplesPerFrame, &bytesDecoded);
				offset += bytesDecoded;
			}

			if(err < 0)
			{
				sys::Dbg::Printf("\nDECMPA - Error %i - Could not get audio data", err);
				if(_data)
					delete _data;
				DecMPA_Destroy(decObj);
				break;
			}

			//done
			DecMPA_Destroy(decObj);

			//success
			gotData = true;
		}
#endif
		
		// still didn't get data...
		if(!gotData)
		{
			sys::Dbg::Printf("\nCould not load this file");
			return false;
		}

		//---------------------------- Send data to buffer

		alBufferData(_buffer, (_channels>1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16), _data, _len*sizeof(s16), _freq);
		
		if(alerr = alGetError())
		{
			sys::Dbg::Printf(false, "\nOpenAL - Error %i - Could not send audio data to audio buffer.", alerr);
			return false;
		}

		return true;
	}

	//Set or change the source of the sound
	bool SetSrc(s16* rawdata, u64 szData, s32 channels = 2, s32 freq = 44100)
	{
		if(_data)
			delete[] _data;
		_data = rawdata;

		s32 alerr = 0;

		if(!_buffer)
		{
			alGenBuffers(1, &_buffer);
			if(alerr = alGetError())
			{
				sys::Dbg::Printf(false, "\nOpenAL - Error %i - Could not generate audio buffer", alerr);
				if(_buffer)
					alDeleteBuffers(1, &_buffer);
				_buffer = NULL;
				return false;
			}
		}

		_freq = freq;
		_len = szData;
		_channels = channels;

		alBufferData(_buffer, ( channels>1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16 ), _data, _len, _freq);

		if(alerr = alGetError())
		{
			sys::Dbg::Printf(false, "\nOpenAL - Error %i - Could not send data to buffer", alerr);
			return false;
		}

		return true;
	}

	//Get the frequency of the sound in Hz (eg: 44100)
	inline s32 Frequency() const { return _freq; }

	//Get the number of channels of the sound (mono: 1, stereo: 2)
	inline s32 Channels() const { return _channels; }

	//Get the 16-bit audio sample data
	inline const s16* GetSamples() const { return _data; }

	//Get the number of audio samples
	inline u64 GetSampleCount() const { return _len; }

	inline s32 GetError() const
	{
		return alGetError();
	}

private:
	s16* _data;
	s32 _freq;
	u64 _len;
	s32 _channels;

	ALuint _buffer;
};

}
}