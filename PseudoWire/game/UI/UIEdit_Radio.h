#pragma once

#include <sys/sys.h>
#include <SFML/Graphics.hpp>

namespace game
{
namespace ui
{

class UIEdit_Radio : public sys::Component
{
private:
	bool checked;

public:
	UIEdit_Radio(sys::State* s, const char* group_id, UIEdit_Radio* prev_radio = NULL);
	~UIEdit_Radio();

	void Tick(float dt);
	void Draw(const sys::Point& screenPos);

	void OnMouseHover(int localx, int localy);
	void OnMouseMoved(int localx, int localy, int dx, int dy);
	void OnMouseMovedInside(int localx, int localy, int dx, int dy);
	void OnMouseEnter(int localx, int localy);
	void OnMouseLeave(int localx, int localy);
	void OnMouseDown(int x, int y, unsigned int button);
	void OnMouseUp(int x, int y, unsigned int button);
	void OnMouseClick(int localx, int localy, unsigned int button);
	void OnMouseUnclick(int localx, int localy, unsigned int button);
	void OnMouseWheel(int localx, int localy, int d);
	void OnMouseWheelInside(int localx, int localy, int d);
	void OnKeyPress(int key, bool shift, bool ctrl, bool alt);
	void OnKeyRelease(int key, bool shift, bool ctrl, bool alt);
	void OnKeyDown(int key, bool shift, bool ctrl, bool alt);
	void OnKeyUp(int key, bool shift, bool ctrl, bool alt);

	void SetCheck(bool val);
	inline bool Checked() const { return checked; }
	void SetText(const char* str);

	std::string Group;
	UIEdit_Radio* Next;
	UIEdit_Radio* Previous;

	sf::Text text;
	sf::Sprite spr_chk;
};

}
}
