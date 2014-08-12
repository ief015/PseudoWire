#pragma once

#include <sys/sys.h>
#include <SFML/Graphics.hpp>

namespace game
{
namespace ui
{

class UIEdit_StringML : public sys::Component
{
private:
	int textbox_linesshown;
	int textbox_height;

public:
	UIEdit_StringML(sys::State* s);
	~UIEdit_StringML();

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
	void OnText(int key);

	void SetText(const char* str);
	void SetString(const char* str);
	void SetHeight(int lines);
	void AddLine();
	void InsertLine();
	sys::String GetString();
	void UpdateTextboxColour();

	static const float TEXTBOX_WIDTH;

	sf::Text text;
	std::vector<sf::Text*> textbox_text;
	std::vector<sys::String> textbox_string;
	sf::Shape textbox_shp;
	int cursor;
	int cursor_line;
	int firstline_shown;

	bool allowtext;
};

}
}
