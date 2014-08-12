#pragma once

#include "Platform.h"

namespace sys
{

template<class T>
class Rect
{

public:

	T X;
	T Y;
	T Width;
	T Height;

	Rect()
	{
		X = 0;
		Y = 0;
		Width = 0;
		Height = 0;
	}

	Rect(T width, T height, bool center)
	{
		Width = width;
		Height = height;

		if(center)
		{
			X = -width/2;
			Y = -height/2;
		}
	}

	Rect(T x, T y, T squarelen)
	{
		X = x;
		Y = y;
		Width = squarelen;
		Height = squarelen;
	}

	Rect(T x, T y, T width, T height)
	{
		X = x;
		Y = y;
		Width = width;
		Height = height;
	}

	//Get the Y position of the top side of the rectangle.
	inline T Top() const
	{
		return Y;
	}

	//Get the X position of the left side of the rectangle.
	inline T Left() const
	{
		return X;
	}

	//Get the Y position of the bottom side of the rectangle.
	inline T Bottom() const
	{
		return Y + Height;
	}

	//Get the X position of the right side of the rectangle.
	inline T Right() const
	{
		return X + Width;
	}

	//Move the rectangle.
	inline void Move(T offsetx, T offsety)
	{
		X += offsetx;
		Y += offsety;
	}

	//Grow/Shrink the size of the rectangle.
	inline void Grow(T offsetwidth, T offsetheight)
	{
		Width += offsetwidth;
		Height += offsetheight;
	}

	//Set the rectangle to a new position.
	inline void SetPos(T& x, T& y)
	{
		X = x;
		Y = y;
	}

	//Resize the rectangle.
	inline void Resize(T width, T height)
	{
		Width = width;
		Height = height;
	}

	//Scale the rectangle to another size. Center will keep the center of the rectangle in the same spot.
	inline void Scale(f32 scale, bool center = false)
	{
		Scale(scale, scale, center);
	}
	//Scale the rectangle to another size. Center will keep the center of the rectangle in the same spot.
	inline void Scale(f64 scale, bool center = false)
	{
		Scale(scale, scale, center);
	}
	//Scale the rectangle to another size. Center will keep the center of the rectangle in the same spot.
	inline void Scale(f32 scalex, f32 scaley, bool center = false)
	{
		if(center)
		{
			X += Width/2;
			Y += Height/2;
		}
		Width *= scalex;
		Height *= scaley;
		if(center)
		{
			X -= Width/2;
			Y -= Height/2;
		}
	}
	//Scale the rectangle to another size. Center will keep the center of the rectangle in the same spot.
	inline void Scale(f64 scalex, f64 scaley, bool center = false)
	{
		if(center)
		{
			X += Width/2;
			Y += Height/2;
		}
		Width *= scalex;
		Height *= scaley;
		if(center)
		{
			X -= Width/2;
			Y -= Height/2;
		}
	}

	//Find the center point of the rectangle.
	inline void GetCenter(T& x, T& y)
	{
		x = Width / 2;
		y = Height / 2;
		x += X;
		y += Y;
	}

	//Get the area of the rectangle.
	inline T Area()
	{
		return Width*Height;
	}

	//Get surface area of the rectangle.
	inline T SurfaceArea()
	{
		return Width*2 + Height*2;
	}

	inline Rect<T> operator +(const sys::Vector<T> & vec)
	{
		return Rect<T>(this->X + vec.X, this->Y + vec.Y, this->Width, this->Height);
	}

	inline Rect<T> operator -(const sys::Vector<T> & vec)
	{
		return Rect<T>(this->X - vec.X, this->Y - vec.Y, this->Width, this->Height);
	}

	//Is this rectangle and another rectangle intersecting? Inclusive, when true, allows touching sides.
	bool Intersects(const Rect<T> & rect, bool inclusive = false)
	{
		if(inclusive)
		{
			return
			!(
				rect.Left() > this->Right() ||
				rect.Right() < this->Left() ||
				rect.Top() > this->Bottom() ||
				rect.Bottom() < this->Top()
			);
		}
		else
		{
			return
			!(
				rect.Left() >= this->Right() ||
				rect.Right() <= this->Left() ||
				rect.Top() >= this->Bottom() ||
				rect.Bottom() <= this->Top()
			);
		}
	}

	//Is the vector2 given inside of the rectangle?
	bool Intersects(const Vector<T> & vec, bool inclusive = false)
	{
		if(inclusive)
		{
			return
			(
				vec.X >= this->Left() &&
				vec.X <= this->Right() &&
				vec.Y <= this->Bottom() &&
				vec.Y >= this->Top()
			);
		}
		else
		{
			return
			(
				vec.X > this->Left() &&
				vec.X < this->Right() &&
				vec.Y < this->Bottom() &&
				vec.Y > this->Top()
			);
		}
	}
};

typedef sys::Rect<s32>	Recti;
typedef sys::Rect<f32>	Rectf;
typedef sys::Rect<f64>	Rectd;

}