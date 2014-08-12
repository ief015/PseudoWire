#pragma once

#include <math.h>
#include <climits>
#include "Platform.h"

namespace sys
{

class Math
{
public:

	static const f64 PI;
	static const f64 PI2; //Half PI
	static const f32 PI_32;
	static const f32 PI2_32; //Half PI
	static const f32 E_32;
	static const f64 E;
	static const f32 INFINITY_32;
	static const f64 INFINITY;
	static const f32 INFINITY_NEG32;
	static const f64 INFINITY_NEG;
	static const f32 NAN_32;
	static const f64 NAN;

	static const u8 MAX_BYTE;
	static const s8 MAX_CHAR;
	static const u16 MAX_USHORT;
	static const s16 MAX_SHORT;
	static const u32 MAX_UINT;
	static const s32 MAX_INT;
	static const u64 MAX_ULONG;
	static const s64 MAX_LONG;
	static const f32 MAX_FLOAT;
	static const f64 MAX_DOUBLE;

	//Absolute
	template<typename T>
	static inline T Abs(T n)
	{
		return (n < 0) ? -n : n;
	}

	//Clamping
	template<typename T>
	static inline T Clamp(T n, T min, T max)
	{
		return (n > max) ? max : (n < min) ? min : n;
	}

	//Sign of "n"
	template<typename T>
	static inline s32 Sign(T n)
	{
		return (n > 0) ? 1 : (n < 0) ? -1 : 0;
	}

	//Trig Conversion

	static inline f32 Deg2Rad(f32 deg)
	{
		return static_cast<f32>(deg / 180.0f * PI);
	}
	static inline f64 Deg2Rad(f64 deg)
	{
		return (deg / 180.0 * PI);
	}
	static inline f32 Rad2Deg(f32 rad)
	{
		return static_cast<f32>(rad / PI * 180.0f);
	}
	static inline f64 Rad2Deg(f64 rad)
	{
		return (rad / PI * 180.0);
	}

	//Trig
	static inline f32 Sin(f32 r)
	{
		return sin(r);
	}
	static inline f32 Cos(f32 r)
	{
		return cos(r);
	}
	static inline f32 Tan(f32 r)
	{
		return tan(r);
	}
	static inline f32 ASin(f32 n)
	{
		return asin(n);
	}
	static inline f32 ACos(f32 n)
	{
		return acos(n);
	}
	static inline f32 ATan(f32 n)
	{
		return atan(n);
	}
	static inline f32 ATan2(f32 y, f32 x)
	{
		return atan2(y, x);
	}
	static inline f64 Sin(f64 r)
	{
		return sin(r);
	}
	static inline f64 Cos(f64 r)
	{
		return cos(r);
	}
	static inline f64 Tan(f64 r)
	{
		return tan(r);
	}
	static inline f64 ASin(f64 n)
	{
		return asin(n);
	}
	static inline f64 ACos(f64 n)
	{
		return acos(n);
	}
	static inline f64 ATan(f64 n)
	{
		return atan(n);
	}
	static inline f64 ATan2(f64 y, f64 x)
	{
		return atan2(y, x);
	}

	//Rounding
	static inline f32 Floor(f32 n)
	{
		return floor(n);
	}
	static inline f32 Ceil(f32 n)
	{
		return ceil(n);
	}
	static inline f32 Round(f32 n)
	{
		return floor(n+0.5f);
	}
	static inline f64 Floor(f64 n)
	{
		return floor(n);
	}
	static inline f64 Ceil(f64 n)
	{
		return ceil(n);
	}
	static inline f64 Round(f64 n)
	{
		return floor(n+0.5);
	}

	static f32 Floor(f32 n, s32 p)
	{
		f32 m = pow(10.0f, p);
		return ceil(n*m) / m;
	}
	static f32 Ceil(f32 n, s32 p)
	{
		f32 m = pow(10.0f, p);
		return ceil(n*m) / m;
	}
	static f32 Round(f32 n, s32 p)
	{
		f32 m = pow(10.0f, p);
		return floor(n*m + 0.5f) / m;
	}
	static f64 Floor(f64 n, s32 p)
	{
		f64 m = pow(10.0, p);
		return ceil(n*m) / m;
	}
	static f64 Ceil(f64 n, s32 p)
	{
		f64 m = pow(10.0, p);
		return ceil(n*m) / m;
	}
	static f64 Round(f64 n, s32 p)
	{
		f64 m = pow(10.0, p);
		return floor(n*m + 0.5) / m;
	}

	//Misc. Math functions

	static inline f32 Log(f32 n)
	{
		return logf(n);
	}
	static inline f64 Log(f64 n)
	{
		return log(n);
	}
	static inline f32 Log10(f32 n)
	{
		return log10(n);
	}
	static inline f64 Log10(f64 n)
	{
		return log10(n);
	}
	static inline f32 Sqrt(f32 n)
	{
		return sqrt(n);
	}
	static inline f64 Sqrt(f64 n)
	{
		return sqrt(n);
	}
	static inline f32 Pow(f32 b, f32 e)
	{
		return pow(b, e);
	}
	static inline f64 Pow(f64 b, f64 e)
	{
		return pow(b, e);
	}
	static inline f32 Pow(f32 b, s32 e)
	{
		return pow(b, e);
	}
	static inline f64 Pow(f64 b, s32 e)
	{
		return pow(b, e);
	}
	static inline f32 Exp(f32 n)
	{
		return expf(n);
	}
	static inline f64 Exp(f64 n)
	{
		return exp(n);
	}
	static inline f32 Mod(f32 n, f32 d)
	{
		return fmod(n, d);
	}
	static inline f64 Mod(f64 n, f64 d)
	{
		return fmod(n, d);
	}

	//Returns the parameter with the lowest value
	template<class T>
	static inline T Min(T a, T b)
	{
		if(a > b)
			return b;
		return a;
	}

	//Returns the parameter with the highest value
	template<class T>
	static inline T Max(T a, T b)
	{
		if(a > b)
			return a;
		return b;
	}
	
	//Get value between left and right from percentage (p, 0.0-1.0)
	static inline f32 Interpolate(f32 left, f32 right, f32 p)
	{
		return (left * (1.0f - p)) + (right * p);
	}

	//Get value between left and right from percentage (p, 0.0-1.0)
	static inline f64 Interpolate(f64 left, f64 right, f64 p)
	{
		return (left * (1.0 - p)) + (right * p);
	}

	//Get value between left and right from percentage (p, 0.0-1.0)
	static inline s32 Interpolate(s32 left, s32 right, f32 p)
	{
		return static_cast<s32>((left * (1.0f - p)) + (right * p));
	}

	//Get value between left and right from percentage (p, 0.0-1.0)
	static inline s32 Interpolate(s32 left, s32 right, f64 p)
	{
		return static_cast<s32>((left * (1.0 - p)) + (right * p));
	}

	// Check if float value is a number (not Infinity or NaN)
	static bool IsValid(f32 v)
	{
		return !(v == INFINITY_32 || v == INFINITY_NEG32 || v == NAN_32);
	}

	// Check if float value is a number (not Infinity or NaN)
	static bool IsValid(f64 v)
	{
		return !(v == INFINITY || v == INFINITY_NEG || v == NAN);
	}

	// Check if float value is a number (not Infinity or NaN), return v if valid, else return 0.
	static f32 Validate(f32 v)
	{
		if (v == INFINITY_32 || v == INFINITY_NEG32 || v == NAN_32)
			return 0.0f;
		return v;
	}

	// Check if float value is a number (not Infinity or NaN)
	static f64 Validate(f64 v)
	{
		if (v == INFINITY || v == INFINITY_NEG || v == NAN)
			return 0.0;
		return v;
	}
};

}
