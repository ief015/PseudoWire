
#include "Gradient.h"

#include "Color.h"
#include "Platform.h"
#include <vector>

using namespace sys;

GradientElement::GradientElement(f32 Point, Color4 Color)
: point(Point)
, color(Color)
{
}

Gradient::Gradient(Color4 startColor, Color4 endColor)
{
	elements_.push_back(GradientElement(0.0f, startColor));
	elements_.push_back(GradientElement(1.0f, endColor));
}

Gradient::Gradient(Color4 startColor, Color4 endColor, GradientElement* elements, s32 elements_cnt)
{
	elements_.push_back(GradientElement(0.0f, startColor));
	elements_.push_back(GradientElement(1.0f, endColor));

	for(s32 i = 0; i < elements_cnt; i++)
	{
		elements_.push_back(elements[i]);
	}
}

Gradient::~Gradient(void)
{
}

bool Gradient::Add(const GradientElement& newElem)
{
	if(0.0f >= newElem.point || newElem.point >= 1.0f)
		return false;

	for(u32 i = 0; i < elements_.size(); i++)
	{
		if(newElem.point < elements_[i].point)
		{
			elements_.insert(elements_.begin() + i, newElem);
			break;
		}
	}
	return true;
}

bool Gradient::Remove(u32 elemIndex)
{
	if(elemIndex < 0 || elemIndex >= elements_.size())
		return false;
	elements_.erase(elements_.begin()+elemIndex);
	return true;
}

Color4 Gradient::Solve(f32 point)
{
	//bounds check
	if(point <= 0.0f)
		return elements_[0].color;
	else if (point >= 1.0f)
		return elements_[elements_.size() - 1].color;
	
	//find color
	GradientElement leftelem, rightelem;
	for(u32 i = 0; i < elements_.size(); i++)
	{
		if(elements_[i].point >= point)
		{
			leftelem.color = elements_[i-1].color;
			leftelem.point = elements_[i-1].point;
			rightelem.color = elements_[i].color;
			rightelem.point = elements_[i].point;
			break;
		}
	}
	f32 pcnt = (point - leftelem.point) / (rightelem.point - leftelem.point);
	Color4 c;
	c.R = (leftelem.color.R * (1.0f - pcnt)) + (rightelem.color.R * pcnt);
	c.G = (leftelem.color.G * (1.0f - pcnt)) + (rightelem.color.G * pcnt);
	c.B = (leftelem.color.B * (1.0f - pcnt)) + (rightelem.color.B * pcnt);
	c.A = (leftelem.color.A * (1.0f - pcnt)) + (rightelem.color.A * pcnt);

	return c;
}
