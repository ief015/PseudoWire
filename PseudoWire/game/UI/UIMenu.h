#pragma once

#include <sys/sys.h>
#include <vector>

#include "../GameState.h"

namespace game
{
namespace ui
{

class UIMenu : public sys::Component
{
protected:
	GameState* game;
	std::vector<sf::Sprite*> icons;
	std::vector<sf::Text*> texts;
	std::vector<int> widths;
	float bgwidth;
	sf::Shape bg;
	
	static float height;

	int selected;
	
public:
	void AddItem(const char* text, const char* iconfile);
	void EditItem(int idx, const char* text, const char* iconfile);
	void Update();
	void Resize(int w, int h);
	
	UIMenu(sys::State* s, float width);
	~UIMenu();
	
	void Tick(float dt);
	void Draw(const sys::Point& screenpos);
	
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
};

}
}
