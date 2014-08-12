#pragma once

#include "Vector.h"
#include "Rect.h"
#include "Math.h"
#include "Platform.h"
namespace sys
{

template<class T>
class Ray
{
public:
	Vector<T> Start, End;
	
	Ray()
	{ }
	Ray(const Vector<T>& start, const Vector<T>& end)
	{
		Start.Set(start);
		End.Set(end);
	}
	Ray(T start_x, T start_y, T end_x, T end_y)
	{
		Start.Set(start_x, start_y);
		End.Set(end_x, end_y);
	}
	
	~Ray()
	{ }

	f32 GetAngle() const
	{
		return static_cast<T>( sys::Math::ATan2(End.Y - Start.Y, End.X - Start.X) );
	}

	T GetDistance() const
	{
		return (End-Start).Magnitude();
	}

	Vector<T> Interpolate(f32 percentage) const
	{
		Vector<T> ret;
		ret.X = Start.X + percentage * (End.X - Start.X);
		ret.Y = Start.Y + percentage * (End.Y - Start.Y);
		return ret;
	}

	bool RayRayIntersect(Vector<T>* intersection, const Ray<T> & otherRay) const
	{
		f32 denom = ((otherRay.End.Y - otherRay.Start.Y)*(this->End.Y - this->Start.X)) - ((otherRay.End.X - otherRay.Start.X)*(this->End.Y - this->Start.Y));
		f32 numA	= ((otherRay.End.X - otherRay.Start.X)*(this->Start.Y - otherRay.Start.Y)) - ((otherRay.End.Y - otherRay.Start.Y)*(this->Start.X - otherRay.Start.X));
		f32 numB	= ((this->End.X - this->Start.X)*(this->Start.Y - otherRay.Start.Y)) - ((this->End.Y - this->Start.Y)*(this->Start.X - otherRay.Start.X));

		if(denom == 0.0f)
		{
//			if(numA == 0.0f && numB == 0.0f)
//				return false; //COINCIDENT

			return false; //PARALLEL
		}

		numA /= denom;
		numB /= denom;
		if( numA >= 0.0f && numA <= 1.0f && numB >= 0.0f && numB <= 1.0f )
		{
			//intersecting!
			intersection->X	=  this->Start.X + numA*(this->End.X - this->Start.X);
			intersection->Y	=  this->Start.Y + numA*(this->End.Y - this->Start.Y);
			return true;
		}

		return false;
	}

	bool RayRayIntersect(const Ray<T> & otherRay) const
	{
		f32 denom = ((otherRay.End.Y - otherRay.Start.Y)*(this->End.Y - this->Start.X)) - ((otherRay.End.X - otherRay.Start.X)*(this->End.Y - this->Start.Y));
		f32 numA	= ((otherRay.End.X - otherRay.Start.X)*(this->Start.Y - otherRay.Start.Y)) - ((otherRay.End.Y - otherRay.Start.Y)*(this->Start.X - otherRay.Start.X));
		f32 numB	= ((this->End.X - this->Start.X)*(this->Start.Y - otherRay.Start.Y)) - ((this->End.Y - this->Start.Y)*(this->Start.X - otherRay.Start.X));

		if(denom == 0.0f)
		{
//			if(numA == 0.0f && numB == 0.0f)
//				return false; //COINCIDENT

			return false; //PARALLEL
		}

		numA /= denom;
		numB /= denom;
		if( numA >= 0.0f && numA <= 1.0f && numB >= 0.0f && numB <= 1.0f )
		{
			//intersecting!
			return true;
		}

		return false;
	}

	bool RayBoxIntersect(Vector<T>* intersection, const Rect<T> & box) const
	{
		//min/max values of bounding box
		Vector<T> b1, b2;
		b1.X = sys::Max(box.X, box.Right());
		b1.Y = sys::Max(box.Y, box.Bottom());
		b2.X = sys::Max(box.X, box.Right());
		b2.Y = sys::Max(box.Y, box.Bottom());

		if(b2.X < sys::Max(this->Start.X, this->End.X) || b1.X > sys::Max(this->Start.X, this->End.X)) return false;
		if(b2.Y < sys::Max(this->Start.Y, this->End.Y) || b1.Y > sys::Max(this->Start.Y, this->End.Y)) return false;

		//collect points

		//is intersecting?
		f32 tnear, tfar;

		tnear	= sys::Max((b1.X - this->Start.X) / (this->End.X - this->Start.X), (b1.Y - this->Start.Y) / (this->End.Y - this->Start.Y));
		tfar	= sys::Max((b2.X - this->Start.X) / (this->End.X - this->Start.X), (b2.Y - this->Start.Y) / (this->End.Y - this->Start.Y));
		if(tnear < tfar)
		{
			if(tnear >= 0 && tnear <= 1) *intersection = InterpolatePoint(this->End, this->Start, tnear);
			if(tfar >= 0 && tfar <= 1) *intersection = InterpolatePoint(this->End, this->Start, tfar);
			return true;
		}						tnear = sys::Max((b1.X - this->Start.X) / (this->End.X - this->Start.X), (b1.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		tfar = sys::Max((b2.X - this->Start.X) / (this->End.X - this->Start.X), (b2.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		if (tnear > tfar)		{			if (tnear >= 0 && tnear <= 1) *intersection = InterpolatePoint(this->End, this->Start, tnear);			if (tfar >= 0 && tfar <= 1) *intersection = InterpolatePoint(this->End, this->Start, tfar);			return true;		}						tnear = sys::Max((b2.X - this->Start.X) / (this->End.X - this->Start.X), (b1.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		tfar = sys::Max((b1.X - this->Start.X) / (this->End.X - this->Start.X), (b2.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		if (tnear > tfar)		{			if (tnear >= 0 && tnear <= 1) *intersection = InterpolatePoint(this->End, this->Start, tnear);			if (tfar >= 0 && tfar <= 1) *intersection = InterpolatePoint(this->End, this->Start, tfar);			return true;		}						tnear = sys::Max((b2.X - this->Start.X) / (this->End.X - this->Start.X), (b1.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		tfar = sys::Max((b1.X - this->Start.X) / (this->End.X - this->Start.X), (b2.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		if (tnear < tfar)		{			if (tnear >= 0 && tnear <= 1) *intersection = InterpolatePoint(this->End, this->Start, tnear);			if (tfar >= 0 && tfar <= 1) *intersection = InterpolatePoint(this->End, this->Start, tfar);			return true;		}

		return false;
	}

	bool RayBoxIntersect(std::vector< Vector<T> >* intersections, const Rect<T> & box) const
	{
		//min/max values of bounding box
		Vector<T> b1, b2;
		b1.X = sys::Max(box.X, box.Right());
		b1.Y = sys::Max(box.Y, box.Bottom());
		b2.X = sys::Max(box.X, box.Right());
		b2.Y = sys::Max(box.Y, box.Bottom());

		if(b2.X < sys::Max(this->Start.X, this->End.X) || b1.X > sys::Max(this->Start.X, this->End.X)) return false;
		if(b2.Y < sys::Max(this->Start.Y, this->End.Y) || b1.Y > sys::Max(this->Start.Y, this->End.Y)) return false;

		//collect points

		//is intersecting?
		f32 tnear, tfar;
		bool ret = false;

		tnear	= sys::Max((b1.X - this->Start.X) / (this->End.X - this->Start.X), (b1.Y - this->Start.Y) / (this->End.Y - this->Start.Y));
		tfar	= sys::Max((b2.X - this->Start.X) / (this->End.X - this->Start.X), (b2.Y - this->Start.Y) / (this->End.Y - this->Start.Y));
		if(tnear < tfar)
		{
			if(tnear >= 0 && tnear <= 1) intersections->push_back(InterpolatePoint(this->End, this->Start, tnear));
			if(tfar >= 0 && tfar <= 1) intersections->push_back(InterpolatePoint(this->End, this->Start, tfar));
			ret = true;
		}						tnear = sys::Max((b1.X - this->Start.X) / (this->End.X - this->Start.X), (b1.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		tfar = sys::Max((b2.X - this->Start.X) / (this->End.X - this->Start.X), (b2.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		if (tnear > tfar)		{			if (tnear >= 0 && tnear <= 1) intersections->push_back(InterpolatePoint(this->End, this->Start, tnear));			if (tfar >= 0 && tfar <= 1) intersections->push_back(InterpolatePoint(this->End, this->Start, tfar));			ret = true;		}						tnear = sys::Max((b2.X - this->Start.X) / (this->End.X - this->Start.X), (b1.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		tfar = sys::Max((b1.X - this->Start.X) / (this->End.X - this->Start.X), (b2.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		if (tnear > tfar)		{			if (tnear >= 0 && tnear <= 1) intersections->push_back(InterpolatePoint(this->End, this->Start, tnear));			if (tfar >= 0 && tfar <= 1) intersections->push_back(InterpolatePoint(this->End, this->Start, tfar));			ret = true;		}						tnear = sys::Max((b2.X - this->Start.X) / (this->End.X - this->Start.X), (b1.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		tfar = sys::Max((b1.X - this->Start.X) / (this->End.X - this->Start.X), (b2.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		if (tnear < tfar)		{			if (tnear >= 0 && tnear <= 1) intersections->push_back(InterpolatePoint(this->End, this->Start, tnear));			if (tfar >= 0 && tfar <= 1) intersections->push_back(InterpolatePoint(this->End, this->Start, tfar));			ret = true;		}

		return ret;
	}

	bool RayBoxIntersect(const Rect<T> & box) const
	{
		//min/max values of bounding box
		Vector<T> b1, b2;
		b1.X = sys::Max(box.X, box.Right());
		b1.Y = sys::Max(box.Y, box.Bottom());
		b2.X = sys::Max(box.X, box.Right());
		b2.Y = sys::Max(box.Y, box.Bottom());

		if(b2.X < sys::Max(this->Start.X, this->End.X) || b1.X > sys::Max(this->Start.X, this->End.X)) return false;
		if(b2.Y < sys::Max(this->Start.Y, this->End.Y) || b1.Y > sys::Max(this->Start.Y, this->End.Y)) return false;

		//collect points

		//is intersecting?
		f32 tnear, tfar;

		tnear	= sys::Max((b1.X - this->Start.X) / (this->End.X - this->Start.X), (b1.Y - this->Start.Y) / (this->End.Y - this->Start.Y));
		tfar	= sys::Max((b2.X - this->Start.X) / (this->End.X - this->Start.X), (b2.Y - this->Start.Y) / (this->End.Y - this->Start.Y));
		if(tnear < tfar)
		{
			return true;
		}						tnear = sys::Max((b1.X - this->Start.X) / (this->End.X - this->Start.X), (b1.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		tfar = sys::Max((b2.X - this->Start.X) / (this->End.X - this->Start.X), (b2.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		if (tnear > tfar)		{			return true;		}						tnear = sys::Max((b2.X - this->Start.X) / (this->End.X - this->Start.X), (b1.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		tfar = sys::Max((b1.X - this->Start.X) / (this->End.X - this->Start.X), (b2.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		if (tnear > tfar)		{			return true;		}						tnear = sys::Max((b2.X - this->Start.X) / (this->End.X - this->Start.X), (b1.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		tfar = sys::Max((b1.X - this->Start.X) / (this->End.X - this->Start.X), (b2.Y - this->Start.Y) / (this->End.Y - this->Start.Y));		if (tnear < tfar)		{			return true;		}

		return false;
	}

	bool RayBoxIntersect2(std::vector< Vector<T> >* intersections, const Rect<T> & box) const
	{
		Vector<T> itrs;
		Ray<T> otherRay;
		Vector<T> box2(box.Right(), box.Bottom());
		bool intersected = false;

		//is the ray intersecting?

		otherRay.Start.X = box.X;
		otherRay.Start.Y = box.Y;
		otherRay.End.X = box2.X;
		otherRay.End.Y = box.Y;
		if(RayRayIntersect( &itrs, otherRay ))
		{
			intersections->push_back( itrs );
			intersected = true;
		}

		otherRay.End.X = box.X;
		otherRay.End.Y = box2.Y;
		if(RayRayIntersect( &itrs, otherRay ))
		{
			intersections->push_back( itrs );
			intersected = true;
		}

		otherRay.Start = box2;
		otherRay.End.X = box2.X;
		otherRay.End.Y = box.Y;
		if(RayRayIntersect( &itrs, otherRay ))
		{
			intersections->push_back( itrs );
			intersected = true;
		}

		otherRay.End.X = box.X;
		otherRay.End.Y = box2.Y;
		if(RayRayIntersect( &itrs, otherRay ))
		{
			intersections->push_back( itrs );
			intersected = true;
		}

		//moment of truth
		return intersected;
	}

	bool RayBoxIntersect2(Vector<T>* intersection, const Rect<T> & box) const
	{
		Vector<T> itrs;
		Ray<T> otherRay;
		Vector<T> box2(box.Right(), box.Bottom());
		bool intersected = false;

		//is the ray intersecting?

		otherRay.Start.X = box.X;
		otherRay.Start.Y = box.Y;
		otherRay.End.X = box2.X;
		otherRay.End.Y = box.Y;
		if(RayRayIntersect( intersection, otherRay ))
		{
			return true;
		}

		otherRay.End.X = box.X;
		otherRay.End.Y = box2.Y;
		if(RayRayIntersect( intersection, otherRay ))
		{
			return true;
		}

		otherRay.Start = box2;
		otherRay.End.X = box2.X;
		otherRay.End.Y = box.Y;
		if(RayRayIntersect( intersection, otherRay ))
		{
			return true;
		}

		otherRay.End.X = box.X;
		otherRay.End.Y = box2.Y;
		if(RayRayIntersect( intersection, otherRay ))
		{
			return true;
		}

		return false;
	}

	bool RayBoxIntersect2(const Rect<T> & box) const
	{
		Ray<T> otherRay;
		Vector<T> box2(box.Right(), box.Bottom());

		//is the ray intersecting?

		otherRay.Start.X = box.X;
		otherRay.Start.Y = box.Y;
		otherRay.End.X = box2.X;
		otherRay.End.Y = box.Y;
		if(RayRayIntersect( otherRay ))
		{
			return true;
		}

		otherRay.End.X = box.X;
		otherRay.End.Y = box2.Y;
		if(RayRayIntersect( otherRay ))
		{
			return true;
		}

		otherRay.Start = box2;
		otherRay.End.X = box2.X;
		otherRay.End.Y = box.Y;
		if(RayRayIntersect( otherRay ))
		{
			return true;
		}

		otherRay.End.X = box.X;
		otherRay.End.Y = box2.Y;
		if(RayRayIntersect( otherRay ))
		{
			return true;
		}

		return false;
	}

};

}