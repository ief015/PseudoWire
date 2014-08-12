#pragma once

#include "Math.h"
#include "Platform.h"

namespace sys
{
	template <class T>
	class Vector
	{
	public:
		T X, Y;

		Vector()
		: X(0)
		, Y(0)
		{
		}
		Vector(T x, T y)
		: X(x)
		, Y(y)
		{
		}
		Vector(const Vector<T> & vec)
		: X(vec.X)
		, Y(vec.Y)
		{
		}
		~Vector()
		{
		}

		inline void Absolute()
		{
			X = sys::Math::Abs(X);
			Y = sys::Math::Abs(Y);
		}

		inline Vector<T> GetAbsolute() const
		{
			return Vector<T>(sys::Math::Abs(X), sys::Math::Abs(Y));
		}

		inline T AngleR() const
		{
			return static_cast<T>( atan2(Y, X) );
		}

		inline T AngleD() const
		{
			return static_cast<T>( atan2(Y, X) / sys::Math::PI * 180.0 );
		}

		void Clamp(T left, T top, T right, T bottom)
		{
			if(X < left)
				X = left;
			else if(X > right)
				X = right;
			
			if(Y < top)
				Y = top;
			else if(Y > bottom)
				Y = bottom;
		}
		
		Vector<T> GetClamp(T left, T top, T right, T bottom) const
		{
			Vector<T> v(X, Y);
			
			if(v.X < left)
				v.X = left;
			else if(v.X > right)
				v.X = right;
				
			if(v.Y < top)
				v.Y = top;
			else if(v.Y > bottom)
				v.Y = bottom;
			
			return v;
		}

		void Normalize()
		{
			f32 mag = MagnitudeF();
			if(mag != 0)
			{
				X /= mag;
				Y /= mag;
			}
			else
			{
				X = 0;
				Y = 0;
			}
		}

		Vector<T> GetNormalize(f32* out_magnitude = NULL) const
		{
			f32 mag = MagnitudeF();

			if(out_magnitude)
				*out_magnitude = mag;

			if(mag != 0)
				return Vector<T>(static_cast<T>(X/mag), static_cast<T>(Y/mag));
			else
				return sys::Vector<T>();
		}

		inline f32 MagnitudeF() const
		{
			return sqrt(X*X + Y*Y);
		}

		inline f64 Magnitude() const
		{
			return sqrt(X*X + Y*Y);
		}

		inline T DotProduct(const Vector<T> & v) const
		{
			return (X * v.X + Y * v.Y);
		}

		T DistanceTo(const Vector<T> & v) const
		{
			T _x = X - v.X, _y = Y - v.Y;
			return static_cast<T>(sqrt(_x*_x + _y*_y));
		}

		inline void Set(T x, T y)
		{
			X = x;
			Y = y;
		}

		inline void Set(const Vector<T> & vec)
		{
			X = vec.X;
			Y = vec.Y;
		}

		inline void Move(T x, T y)
		{
			X += x;
			Y += y;
		}

		inline void Move(const Vector<T> & vec)
		{
			X += vec.X;
			Y += vec.Y;
		}

		inline void Zero()
		{
			X = 0;
			Y = 0;
		}

		inline Vector<T> operator -() const
		{
			return Vector(-X, -Y);
		}

		inline Vector<T> operator +(const Vector<T> & v) const
		{
			return Vector(X + v.X, Y + v.Y);
		}
		inline Vector<T> operator -(const Vector<T> & v) const
		{
			return Vector(X - v.X, Y - v.Y);
		}
		inline Vector<T> operator *(const Vector<T> & v) const
		{
			return Vector(X * v.X, Y * v.Y);
		}
		inline Vector<T> operator /(const Vector<T> & v) const
		{
			return Vector(X / v.X, Y / v.Y);
		}

		inline Vector<T> operator *(T v) const
		{
			return Vector(X * v, Y * v);
		}
		inline Vector<T> operator /(T v) const
		{
			return Vector(X / v, Y / v);
		}

		inline void operator *=(T v)
		{
			X *= v;
			Y *= v;
		}
		inline void operator /=(T v)
		{
			X /= v;
			Y /= v;
		}

		inline void operator +=(const Vector<T> & v)
		{
			X += v.X;
			Y += v.Y;
		}
		inline void operator -=(const Vector<T> & v)
		{
			X -= v.X;
			Y -= v.Y;
		}
		inline void operator *=(const Vector<T> & v)
		{
			X *= v.X;
			Y *= v.Y;
		}
		inline void operator /=(const Vector<T> & v)
		{
			X /= v.X;
			Y /= v.Y;
		}

		inline bool operator ==(const Vector<T> & v) const
		{
			return X==v.X && Y==v.Y;
		}
		inline bool operator !=(const Vector<T> & v) const
		{
			return X!=v.X || Y!=v.Y;
		}
	};

	typedef sys::Vector<s32>	Vectori;
	typedef sys::Vector<f32>	Vectorf;
	typedef sys::Vector<f64>	Vectord;
}
