#pragma once

#include <vector>
#include "Color.h"
#include "Platform.h"

namespace sys
{
	struct GradientElement
	{
		f32 point;
		Color4 color;
		
		GradientElement() {}
		GradientElement(f32 Point, Color4 Color);
	};

	struct Gradient
	{
	private:
		std::vector<GradientElement> elements_;
		
	public:
		Gradient(Color4 startColor, Color4 endColor);
		Gradient(Color4 startColor, Color4 endColor, GradientElement* elements, s32 cnt_elements);
		~Gradient();

		bool Add(const GradientElement& newElem);
		bool Remove(u32 elemIndex);
		inline s32 GetElemCount() const { return elements_.size(); }
		inline GradientElement& GetElement(u32 elemIndex) { return elements_[elemIndex]; }

		Color4 Solve(f32 point);
	};

}
