#pragma once
#include "Platform.h"

namespace sys
{
class Color4;

class Color3
{
public:
	f32 R;
	f32 G;
	f32 B;

	Color3();
	Color3(const Color3 & copy);
	Color3(f32 r, f32 g, f32 b);
	Color3(s32 r, s32 g, s32 b);

	//Return a clamped (0.0f - 1.0f) copy of this colour.
	inline Color3 GetClamp();

	//Clamp (0.0f - 1.0f) this colour.
	inline void Clamp();

	//Alpha blend this colour with another colour.
	Color3 AlphaBlend(const sys::Color4 & src);
	
	operator Color4();

	inline bool operator == (Color3 v)
	{
		return R == v.R && G == v.G && B == v.B;
	}
	inline bool operator != (Color3 v)
	{
		return R != v.R || G != v.G || B != v.B;
	}

	inline Color3 operator + (Color3 v)
	{
		return Color3(R + v.R, G + v.G, B + v.B);
	}
	inline Color3 operator - (Color3 v)
	{
		return Color3(R - v.R, G - v.G, B - v.B);
	}
	inline Color3 operator * (Color3 v)
	{
		return Color3(R * v.R, G * v.G, B * v.B);
	}
	inline Color3 operator / (Color3 v)
	{
		return Color3(R / v.R, G / v.G, B / v.B);
	}
	
	inline Color3 operator * (f32 v)
	{
		return Color3(R * v, G * v, B * v);
	}
	inline Color3 operator / (f32 v)
	{
		return Color3(R / v, G / v, B / v);
	}

};

class Color4
{
public:
	f32 R;
	f32 G;
	f32 B;
	f32 A;

	Color4();
	Color4(const Color4 & copy);
	Color4(f32 r, f32 g, f32 b, f32 a = 1.0f);
	Color4(s32 r, s32 g, s32 b, s32 a = 255);

	//Return a clamped (0.0f - 1.0f) copy of this colour.
	inline Color4 GetClamp();

	//Clamp (0.0f - 1.0f) this colour.
	inline void Clamp();
	
	operator Color3();

	//Alpha blend this colour with another colour.
	Color4 AlphaBlend(const sys::Color4 & src);

	inline bool operator == (Color4 v)
	{
		return R == v.R && G == v.G && B == v.B && A == v.A;
	}
	inline bool operator != (Color4 v)
	{
		return R != v.R || G != v.G || B != v.B || A != v.A;
	}

	inline Color4 operator + (Color4 v)
	{
		return Color4(R + v.R, G + v.G, B + v.B, A + v.A);
	}
	inline Color4 operator - (Color4 v)
	{
		return Color4(R - v.R, G - v.G, B - v.B, A - v.A);
	}
	inline Color4 operator * (Color4 v)
	{
		return Color4(R * v.R, G * v.G, B * v.B, A * v.A);
	}
	inline Color4 operator / (Color4 v)
	{
		return Color4(R / v.R, G / v.G, B / v.B, A / v.A);
	}
	
	inline Color4 operator * (f32 v)
	{
		return Color4(R * v, G * v, B * v, A * v);
	}
	inline Color4 operator / (f32 v)
	{
		return Color4(R / v, G / v, B / v, A / v);
	}

};

}
