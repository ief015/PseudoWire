#pragma once

#include <sys/sys.h>
#include <SFML/Graphics.hpp>

#include "UIScrollBar.h"

namespace game
{
namespace ui
{

class UIEdit_Explorer : public sys::XComponent
{
private:
	sf::Shape shpview;
	sf::Shape shphighlight;
	std::vector<sf::Text*> texts;
	
	int scrollamount;
	int selecteditem;

	UIScrollBar* scrollbar;

protected:
	void XTick(float dt);
	void XDraw(const sys::Point& screenPos);

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
	void XOnText(int key);

public:
	UIEdit_Explorer(sys::State* s);
	~UIEdit_Explorer();

	static const int ITEM_HEIGHT;
	static const int VIEW_HEIGHT_ITEMS;

	void OpenFolder(const char* foldername);

	std::string ItemSelected() const;
};

}
}
