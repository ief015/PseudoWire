#pragma once

#include <sys/sys.h>
#include "../LuaComponent.h"
#include <SFML/Graphics.hpp>

namespace game
{
namespace ui
{
class UIEdit : public sys::XComponent
{
public:
	UIEdit(sys::State* s);
	~UIEdit();

	static const int SPACE_BETWEEN;
	static const int TITLEBAR_HEIGHT;
	static const int WINDOW_WIDTH;

	static sf::Image img_chkbox_off;
	static sf::Image img_chkbox_on;

	static sf::Image img_radio_off;
	static sf::Image img_radio_on;

	static sf::Image img_accept;
	static sf::Image img_decline;

	static void StaticInit();

	void Move(int x, int y);

protected:
	bool opened;
	bool dragging_window;

	sf::Shape bg;
	sf::Shape titlebar;
	sf::Text titlebar_text;
	sf::Sprite spr_accept;
	sf::Sprite spr_decline;
	void XTick(float dt);
	void XDraw(const sys::Point& screenPos);


	int selectedClose;
	
	void XOnMouseHover(int localx, int localy);
	void XOnMouseMoved(int localx, int localy, int dx, int dy);
	void XOnMouseMovedInside(int localx, int localy, int dx, int dy);
	void XOnMouseEnter(int localx, int localy);
	void XOnMouseLeave(int localx, int localy);
	void XOnMouseDown(int x, int y, unsigned int button);
	void XOnMouseUp(int x, int y, unsigned int button);
	void XOnMouseClick(int localx, int localy, unsigned int button);
	void XOnMouseUnclick(int localx, int localy, unsigned int button);
	void XOnMouseWheel(int localx, int localy, int d);
	void XOnMouseWheelInside(int localx, int localy, int d);
	void XOnKeyPress(int key, bool shift, bool ctrl, bool alt);
	void XOnKeyRelease(int key, bool shift, bool ctrl, bool alt);
	void XOnKeyDown(int key, bool shift, bool ctrl, bool alt);
	void XOnKeyUp(int key, bool shift, bool ctrl, bool alt);
	
public:
	LuaComponent* c;
	bool Show(LuaComponent* comp);
	void Close(bool cancel = false);
	inline bool IsOpen() const { return opened; }
};

}
}
