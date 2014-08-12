#include "Fixture.h"

#include "../Vector.h"
#include <vector>
#include "../Math.h"

using namespace sys;
using namespace sys::physics;

Fixture::Fixture(const std::vector< sys::Vector<float> > & Points)
{
	for(unsigned int i = 0; i < Points.size(); i++)
	{
		points.push_back(sys::Vector<float>(Points[i].X, Points[i].Y));
	}
	UpdateEdges();
}

Fixture::~Fixture()
{
}

void Fixture::UpdateEdges()
{
	Vector<float> p1(0.0f, 0.0f);
	Vector<float> p2(0.0f, 0.0f);
	edges.clear();
	for(unsigned i = 0; i < points.size(); i++)
	{
		p1.Set(points[i]);
		if(i+1 >= points.size())
			p2.Set(points[0]);
		else
			p2.Set(points[i+1]);
		edges.push_back(Vector<float>(p2 - p1));
	}
}

Vector<float> Fixture::GetAABB()
{
	Vector<float> r(0.0f, 0.0f);
	for(std::vector< Vector<float> >::const_iterator itr = points.begin(); itr != points.end(); itr++)
	{
		if(itr->X > r.X) r.X = itr->X;
		if(itr->Y > r.Y) r.Y = itr->Y;
	}
	r -= this->Position();
	return r;
}

Vector<float> Fixture::FindCenter()
{
	Vector<float> center(0.0f, 0.0f);
	for(std::vector< Vector<float> >::const_iterator itr = points.begin(); itr != points.end(); itr++)
	{
		center += (*itr);
	}
	center /= static_cast<float>(points.size());
	return center;
}

void Fixture::SetPosition(const Vector<float> & newPos)
{
	for(std::vector< Vector<float> >::iterator itr = points.begin(); itr != points.end(); itr++)
	{
		itr->Move( newPos.X - pos.X, newPos.Y - pos.Y );
	}
	pos.Set(newPos);
}

void Fixture::Move(const Vector<float> & offset)
{
	for(std::vector< Vector<float> >::iterator itr = points.begin(); itr != points.end(); itr++)
	{
		itr->Move( offset.X, offset.Y );
	}
	pos.Move(offset);
}

float Fixture::IntervalDistance(float minA, float maxA, float minB, float maxB)
{
	if(minA < minB)
		return minB - maxA;
	else
		return minA - maxB;
}

void Fixture::ProjectPolygon(const Vector<float> &axis, float &min, float &max)
{
	float d = points[0].DotProduct(axis);
	min = d;
	max = d;

	for(std::vector< Vector<float> >::iterator itr = points.begin(); itr != points.end(); itr++)
	{
		d = itr->DotProduct(axis);
		if(d < min)
			min = d;
		else if(d > max)
			max = d;
	}
}

bool Fixture::Colliding(Fixture* OtherFixture)
{
	bool IsTouching = true;

	Vector<float> edge(0.0f, 0.0f);

	for(unsigned edgeI = 0; edgeI < edges.size() + OtherFixture->GetEdgeCount(); edgeI++)
	{
		if(edgeI < edges.size())
			edge.Set(edges[edgeI]);
		else
			edge.Set(OtherFixture->GetEdge(edgeI - edges.size()));

		Vector<float> axis(-edge.Y, edge.X);
		axis.Normalize();

		float minA = 0.0f, maxA = 0.0f, minB = 0.0f, maxB = 0.0f;
		this->ProjectPolygon(axis, minA, maxA);
		OtherFixture->ProjectPolygon(axis, minB, maxB);

		if(IntervalDistance(minA, maxA, minB, maxB) > 0.0f) IsTouching = false;

		if(!IsTouching) break;
	}

	return IsTouching;
}

Contact Fixture::Colliding(const Vector<float> & Velocity, Fixture* OtherFixture)
{
	Contact ret = Contact();
	ret.IsTouching = true;
	ret.WillTouch = true;
	ret.TouchTranslation = Vector<float>(0.0f, 0.0f);
	ret.WillTouchTranslation = Vector<float>(0.0f, 0.0f);

	float minIntervalDistance = sys::Math::INFINITY_32;
	Vector<float> translationAxis(0.0f, 0.0f);
	Vector<float> edge(0.0f, 0.0f);

	for(unsigned edgeI = 0; edgeI < edges.size() + OtherFixture->GetEdgeCount(); edgeI++)
	{
		if(edgeI < edges.size())
			edge.Set(edges[edgeI]);
		else
			edge.Set(OtherFixture->GetEdge(edgeI - edges.size()));

		Vector<float> axis(-edge.Y, edge.X);
		axis.Normalize();

		float minA = 0.0f, maxA = 0.0f, minB = 0.0f, maxB = 0.0f;
		this->ProjectPolygon(axis, minA, maxA);
		OtherFixture->ProjectPolygon(axis, minB, maxB);

		if(IntervalDistance(minA, maxA, minB, maxB) > 0.0f) ret.IsTouching = false;
		
		float velocityProj = axis.DotProduct(Velocity);
		if(velocityProj < 0)
			minA += velocityProj;
		else
			maxA += velocityProj;

		float intvDistance = IntervalDistance(minA, maxA, minB, maxB);
		if(intvDistance > 0) ret.WillTouch = false;

		if(!ret.IsTouching && !ret.WillTouch) break;

		intvDistance = sys::Math::Abs(intvDistance);
		if(intvDistance < minIntervalDistance)
		{
			minIntervalDistance = intvDistance;
			translationAxis = axis;

			Vector<float> d = this->FindCenter() - OtherFixture->FindCenter();
			if(d.DotProduct(translationAxis) < 0.0f) translationAxis *= -1;
		}
	}

	if(ret.WillTouch)
	{
		ret.WillTouchTranslation = translationAxis * minIntervalDistance; //not accurate
	}

	if(ret.IsTouching)
	{
		ret.TouchTranslation = translationAxis * minIntervalDistance;
	}
	else
	{

	}

	return ret;
}