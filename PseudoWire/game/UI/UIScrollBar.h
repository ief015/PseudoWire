#pragma once

#include <sys/sys.h>
#include <SFML/Graphics.hpp>

namespace game
{
namespace ui
{

class UIScrollBar : public sys::Component
{
public:
	enum ORIENT
	{
		ORIENT_VERT = 0,
		ORIENT_HORIZ
	};

private:
	int min, max, val, scrAmt;
	static const int BOX_LENGTH_HALF;

	bool clicked;
	bool changed;

	float getLocalBarO() const;
	int localOToVal(int o) const;

	ORIENT orientation;

public:
	UIScrollBar(sys::State* s);
	~UIScrollBar();

	void Tick(float dt);
	void Draw(const sys::Point& screenPos);

	//void OnMouseHover(int localx, int localy);
	void OnMouseMoved(int localx, int localy, int dx, int dy);
	//void OnMouseMovedInside(int localx, int localy, int dx, int dy);
	//void OnMouseEnter(int localx, int localy);
	//void OnMouseLeave(int localx, int localy);
	//void OnMouseDown(int x, int y, unsigned int button);
	void OnMouseUp(int x, int y, unsigned int button);
	void OnMouseClick(int localx, int localy, unsigned int button);
	//void OnMouseUnclick(int localx, int localy, unsigned int button);
	void OnMouseWheel(int localx, int localy, int d);
	//void OnMouseWheelInside(int localx, int localy, int d);
	//void OnKeyPress(int key, bool shift, bool ctrl, bool alt);
	//void OnKeyRelease(int key, bool shift, bool ctrl, bool alt);
	//void OnKeyDown(int key, bool shift, bool ctrl, bool alt);
	//void OnKeyUp(int key, bool shift, bool ctrl, bool alt);
	//void OnText(int key);

	bool PollChanged();
	bool IsChanged() const;

	void ScrollUp();
	void ScrollDown();

	void SetMin(int n);
	void SetMax(int n);
	void SetMinMax(int minimum, int maximum);
	void SetValue(int v);
	inline void SetScrollAmount(int amt) { this->scrAmt = amt; }
	void SetPxLength(int length);
	void SetPxWidth(int width);
	void SetOrientation(ORIENT orient);
	
	inline int GetMin() const { return this->min; }
	inline int GetMax() const { return this->max; }
	inline int GetValue() const { return this->val; }
	inline int GetScrollAmount() const { return this->scrAmt; }
	int GetPxLength() const;
	int GetPxWidth() const;
	inline ORIENT GetOrientation() const { return this->orientation; }
};

}
}