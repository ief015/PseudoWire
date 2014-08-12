#pragma once

#include <sys/sys.h>
#include "../GameLoader.h"

namespace game
{
namespace ui
{

class UITree_Category
{
public:
	UITree_Category()
	: name("")
	, collapsed(false)
	{ }

	bool collapsed;
	std::string name;
	std::vector<AddonItem*> item_refs;
};


// Component selection list box
class UITree : public sys::Component
{
public:
	UITree(sys::State* parent);
	virtual ~UITree();

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

	void PrintSelected(bool force = false);

	bool ReadyToClose;
	sys::Point PlaceComponentPos;
	AddonItem* ItemSelected;

protected:
	static sf::Text UITreeText;
	static sf::Image UITreeImgCollapsed;
	static sf::Image UITreeImgClosed;

	sf::Sprite spr_collapsed;
	sf::Sprite spr_closed;

	sf::Shape shp_selectionbox;
	sf::Shape shp_listbox;

	float listItemSize;
	int numItemsAtOnce;
	int scrollUpCount;
	bool mouse_inside;
	int numItemsOpen;

	int selected_item;
	int last_printed_item;
	std::vector<UITree_Category> categories;
};

}
}