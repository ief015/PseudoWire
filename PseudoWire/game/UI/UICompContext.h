#pragma once

#include <sys/sys.h>
#include "UI_DATA.h"

#include <SFML/Graphics.hpp>

namespace game
{
class LuaComponent;
namespace ui
{

// Component selection list box
class UICompContext : public sys::Component
{
protected:
	static sf::Text myText;

	std::vector<std::string> itemStr;

	sf::Shape shp_selectionbox;
	sf::Shape shp_listbox;

	float listItemSize;
	int numItemsAtOnce;

public:
	UICompContext(sys::State* parent, UICONTEXT_TYPE type);
	virtual ~UICompContext();

	static void StaticInit();

	void Draw(const sys::Point &screenpos);

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

	int ItemClicked;
	UICONTEXT_TYPE Type;

	inline int NumOfItems() const { return numItemsAtOnce; }

	void UpdateList(LuaComponent* comp);
	LuaComponent* comp_ref;
	int selected_item;
	bool mouse_inside;
};

}
}