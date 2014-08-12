#pragma once

#include <sys/sys.h>
#include <SFML/Graphics.hpp>

namespace game
{
namespace ui
{

class UIEdit_Sprite : public sys::Component
{
private:
	sf::Sprite spr;

public:
	UIEdit_Sprite(sys::State* s);
	~UIEdit_Sprite();

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

	void Load(const sf::Image& img);
};

}
}
