#pragma once

#include <time.h>
#include "Math.h"
#include "Platform.h"
#include "Singleton.h"

// #define _SYS_RANDOM_MT19937_64_

namespace sys
{
// Pseudo-Random Number Generator based on the 64-bit variant of the MT19937 algorithm.
class Random : public sys::Singleton<Random>
{
public:
	Random()
	{
		Init( time(NULL) );
	}
	Random(u64 seed)
	{
		Init(seed);
	}
	~Random()
	{
	}


	/***** Bool *****/


	inline bool GetBoolean()
	{
		return (static_cast<u8>(GetData()) < 0x7F);
	}


	/***** Char *****/


	inline s8 GetChar()
	{
		return static_cast<s8>(GetData());
	}
	inline s8 GetChar(s8 min, s8 max)
	{
		u8 r = static_cast<u8>(GetChar()+Math::MAX_CHAR);
		r = r % (max - min) + min;
		return static_cast<s8>(r);
	}


	/***** Byte *****/


	inline u8 GetByte()
	{
		return static_cast<u8>(GetData());
	}
	inline u8 GetByte(u8 min, u8 max)
	{
		return GetByte() % (max - min) + min;
	}


	/***** Int 16 *****/


	inline s16 GetInt16()
	{
		return static_cast<s16>(GetData());
	}
	inline s16 GetInt16(s16 min, s16 max)
	{
		u16 r = static_cast<u16>(GetInt16()+Math::MAX_SHORT);
		r = r % (max - min) + min;
		return static_cast<s16>(r);
	}


	/***** Int 32 *****/


	inline s32 GetInt32()
	{
		return static_cast<s32>(GetData());
	}
	inline s32 GetInt32(s32 min, s32 max)
	{
		u32 r = static_cast<u32>(GetInt32()+Math::MAX_INT);
		r = r % (max - min) + min;
		return static_cast<s32>(r);
	}


	/***** Int 64 *****/


	inline s64 GetInt64()
	{
		return static_cast<s64>(GetData());
	}
	inline s64 GetInt64(s64 min, s64 max)
	{
		u64 r = static_cast<u64>(GetInt32()+Math::MAX_LONG);
		r = r % (max - min) + min;
		return static_cast<s64>(r);
	}


	/***** UInt 16 *****/


	inline u16 GetUInt16()
	{
		return static_cast<u16>(GetData());
	}
	inline u16 GetUInt16(u16 min, u16 max)
	{
		return GetUInt16() % (max - min) + min;
	}


	/***** UInt 32 *****/


	inline u32 GetUInt32()
	{
		return static_cast<u32>(GetData());
	}
	inline u32 GetUInt32(u32 min, u32 max)
	{
		return GetUInt32() % (max - min) + min;
	}


	/***** UInt 64 *****/


	inline u64 GetUInt64() //MAIN RANDOM FUNC
	{
		return GetData();
	}
	inline u64 GetUInt64(u64 min, u64 max)
	{
		return GetUInt64() % (max - min) + min;
	}


	/***** Float 32 *****/


	inline f32 GetFloat32()
	{
		return (static_cast<u32>(GetData()) / (65535.0f * 65537.0f));
	}
	inline f32 GetFloat32(f32 min, f32 max)
	{
		return GetFloat32() * (max - min) + min;
	}


	/***** Float 64 *****/


	inline f64 GetFloat64()
	{
		return (GetData() / (4294967295.0 * 4294967297.0));
	}
	inline f64 GetFloat64(f64 min, f64 max)
	{
		return GetFloat64() * (max - min) + min;
	}


	/*-=========== OPERATORS ===========-*/


	//input

	//Assign a new UInt64 seed.
	inline void operator <<(u64 seed)
	{
		Init(seed);
	}

	//output

	//Output random number of given type.
	inline void operator >>(bool &v)
	{
		v = GetBoolean();
	}

	//Output random number of given type.
	inline void operator >>(s8 &v)
	{
		v = GetChar();
	}

	//Output random number of given type.
	inline void operator >>(u8 &v)
	{
		v = GetByte();
	}

	//Output random number of given type.
	inline void operator >>(s16 &v)
	{
		v = GetInt16();
	}

	//Output random number of given type.
	inline void operator >>(u16 &v)
	{
		v = GetUInt16();
	}

	//Output random number of given type.
	inline void operator >>(s32 &v)
	{
		v = GetInt32();
	}

	//Output random number of given type.
	inline void operator >>(u32 &v)
	{
		v = GetUInt32();
	}

	//Output random number of given type.
	inline void operator >>(s64 &v)
	{
		v = GetInt64();
	}

	//Output random number of given type.
	inline void operator >>(u64 &v)
	{
		v = GetUInt64();
	}

	//Output random number of given type.
	inline void operator >>(f32 &v)
	{
		v = GetFloat32();
	}

	//Output random number of given type.
	inline void operator >>(f64 &v)
	{
		v = GetFloat64();
	}

	//Cast operator for outputting a random boolean.
	inline operator bool()
	{
		return GetBoolean();
	}

	//Cast operator for outputting a random character.
	inline operator s8()
	{
		return GetChar();
	}

	//Cast operator for outputting a random byte.
	inline operator u8()
	{
		return GetByte();
	}

	//Cast operator for outputting a random int 16.
	inline operator s16()
	{
		return GetInt16();
	}

	//Cast operator for outputting a random uint 16.
	inline operator u16()
	{
		return GetUInt16();
	}

	//Cast operator for outputting a random int.
	inline operator s32()
	{
		return GetInt32();
	}

	//Cast operator for outputting a random uint.
	inline operator u32()
	{
		return GetUInt32();
	}

	//Cast operator for outputting a random int 64.
	inline operator s64()
	{
		return GetInt64();
	}

	//Cast operator for outputting a random uint 64.
	inline operator u64()
	{
		return GetUInt64();
	}

	//Cast operator for outputting a random float.
	inline operator f32()
	{
		return GetFloat32();
	}

	//Cast operator for outputting a random double.
	inline operator f64()
	{
		return GetFloat64();
	}

protected:

	u64 _init_seed;
	u64 _iter;
	u64 _seed;

	inline void Init(u64 seed)
	{
		_iter = 0;
		_init_seed = seed;
		_seed = seed;
	}

// #ifdef _SYS_RANDOM_MT19937_64_

	inline void IncSeed()
	{
		_seed = 6364136223846793005uLL * (_seed ^ (_seed>>30)) + _iter;
		++_iter;
	}

	u64 GetData()
	{
		u64 y = _seed;

		y ^= (y>>29) & 0x5555555555555555uLL;
		y ^= (y<<17) & 0x71D67FFFEDA60000uLL;
		y ^= (y<<37) & 0xFFF7EEE000000000uLL;
		y ^= (y>>43);

		IncSeed();

		return y;
	}

/*
#else

	inline u64 IncSeed()
	{
		++_iter;
		//_seed += 12345uL + (_iter<<2);
		_seed += 12345uL + (_iter);
		return _seed;
	}

	inline u64 GetData()
	{
		u64 r = IncSeed();
		r += (r>>2)*((_init_seed+12345uL)>>1)*sizeof(s64)*_iter;
		return r;
	}
#endif
*/

};

}
