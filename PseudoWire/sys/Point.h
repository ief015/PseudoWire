#pragma once
#include "Platform.h"

namespace sys
{

//Lightweight 2D Int32/Float32 Point struct for UI
struct Point
{
#if IEF_ENABLE_FLOAT_UI
# define IEF_POINT_T f32
#else
# define IEF_POINT_T s32
#endif
	
	IEF_POINT_T X;
	IEF_POINT_T Y;

	Point()
	: X(0)
	, Y(0)
	{ }

	Point(IEF_POINT_T x, IEF_POINT_T y)
	: X(x)
	, Y(y)
	{ }

	inline Point operator - () const
	{
		return Point(-X, -Y);
	}

	inline Point operator + (const Point& v) const
	{
		return Point(X + v.X, Y + v.Y);
	}

	inline Point operator - (const Point& v) const
	{
		return Point(X - v.X, Y - v.Y);
	}

	inline Point operator * (const Point& v) const
	{
		return Point(X * v.X, Y * v.Y);
	}

	inline Point operator * (s32 v) const
	{
		return Point(X * static_cast<IEF_POINT_T>(v), Y * static_cast<IEF_POINT_T>(v));
	}

	inline Point operator * (f32 v) const
	{
		return Point(X * static_cast<IEF_POINT_T>(v), Y * static_cast<IEF_POINT_T>(v));
	}

	inline Point operator / (const Point& v) const
	{
		return Point(X / v.X, Y / v.Y);
	}

	inline Point operator / (s32 v) const
	{
		return Point(X / static_cast<IEF_POINT_T>(v), Y / static_cast<IEF_POINT_T>(v));
	}

	inline Point operator / (f32 v) const
	{
		return Point(X / static_cast<IEF_POINT_T>(v), Y / static_cast<IEF_POINT_T>(v));
	}

	inline void operator += (const Point& v)
	{
		X += v.X;
		Y += v.Y;
	}

	inline void operator -= (const Point& v)
	{
		X -= v.X;
		Y -= v.Y;
	}

	inline void operator *= (const Point& v)
	{
		X *= v.X;
		Y *= v.Y;
	}

	inline void operator *= (s32 v)
	{
		X *= static_cast<IEF_POINT_T>(v);
		Y *= static_cast<IEF_POINT_T>(v);
	}

	inline void operator *= (f32 v)
	{
		X *= static_cast<IEF_POINT_T>(v);
		Y *= static_cast<IEF_POINT_T>(v);
	}

	inline void operator /= (const Point& v)
	{
		X /= v.X;
		Y /= v.Y;
	}

	inline void operator /= (s32 v)
	{
		X /= static_cast<IEF_POINT_T>(v);
		Y /= static_cast<IEF_POINT_T>(v);
	}

	inline void operator /= (f32 v)
	{
		X /= static_cast<IEF_POINT_T>(v);
		Y /= static_cast<IEF_POINT_T>(v);
	}

	inline bool operator == (const Point& v) const
	{
		return (X == v.X && Y == v.Y);
	}

	inline bool operator != (const Point& v) const
	{
		return (X != v.X || Y != v.Y);
	}

	inline void operator = (const Point& v)
	{
		X = v.X;
		Y = v.Y;
	}

};

}
