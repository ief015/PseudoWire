#pragma once

#include <sys/sys.h>
#include <vector>

#include <SFML/Graphics.hpp>

namespace game
{
namespace ui
{

class UIConsole : public sys::Component
{
public:
	enum MESSAGE_TYPE
	{
		MSG_TNORMAL = 0,
		MSG_TINFO,
		MSG_TINFO_NOCONSOLE,
		MSG_TWARNING,
		MSG_TERROR,
		MSG_TWELCOME,
		MSG_TCONSOLE,

		NUM_MSG_TYPES
	};

	struct UIConsole_Msg
	{
		UIConsole_Msg(MESSAGE_TYPE t, const char* m)
		{
			type = t;
			msg = m;
		}
		UIConsole_Msg(MESSAGE_TYPE t, const std::string& m)
		{
			type = t;
			msg = m;
		}

		MESSAGE_TYPE type;
		std::string msg;
	};

	void Print(MESSAGE_TYPE type, const char* format, ... );
	void ToggleConsole();
	bool ConsoleOpened();

	void Resize(int w, int h);

protected:
	float bgwidth;
	float bgy;
	sf::Shape bg;
	sf::Text txt;

	sf::Sprite arrow;

	sf::Sprite icon;
	MESSAGE_TYPE msgtype;

	bool icon_notification;
	float icon_notification_y;
	float icon_notification_alpha;

	std::vector<UIConsole_Msg> msgs;
	bool console_opened;
	int console_size;
	
	static float height;
	static sf::Image img_normal;
	static sf::Image img_info;
	static sf::Image img_warning;
	static sf::Image img_error;
	static sf::Image img_heart;
	static sf::Image img_arrow;
	
public:
	UIConsole(sys::State* s, float width, float height);
	~UIConsole();

	static void StaticInit();
	
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
