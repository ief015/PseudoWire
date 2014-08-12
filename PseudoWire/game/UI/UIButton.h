#pragma once

#include <sys/sys.h>
#include <SFML/Graphics.hpp>

namespace game
{
namespace ui
{

class UIButton : public sys::Component
{
private:
	char flags;
	static const char FLAG_DOWN;
	static const char FLAG_HOVER;
	static const char FLAG_POLL;

	inline void enableFlag(char s) { this->flags |= s; }
	inline void disableFlag(char s) { this->flags &= ~s; }

	sf::Text txt;

public:
	UIButton(sys::State* s);
	~UIButton();

	void Tick(float dt);
	void Draw(const sys::Point& screenPos);

	//void OnMouseHover(int localx, int localy);
	//void OnMouseMoved(int localx, int localy, int dx, int dy);
	//void OnMouseMovedInside(int localx, int localy, int dx, int dy);
	void OnMouseEnter(int localx, int localy);
	void OnMouseLeave(int localx, int localy);
	//void OnMouseDown(int x, int y, unsigned int button);
	void OnMouseUp(int x, int y, unsigned int button);
	void OnMouseClick(int localx, int localy, unsigned int button);
	//void OnMouseUnclick(int localx, int localy, unsigned int button);
	//void OnMouseWheel(int localx, int localy, int d);
	//void OnMouseWheelInside(int localx, int localy, int d);
	//void OnKeyPress(int key, bool shift, bool ctrl, bool alt);
	//void OnKeyRelease(int key, bool shift, bool ctrl, bool alt);
	//void OnKeyDown(int key, bool shift, bool ctrl, bool alt);
	//void OnKeyUp(int key, bool shift, bool ctrl, bool alt);
	//void OnText(int key);

	inline bool IsMouseHovering() const { return (this->flags & FLAG_HOVER) != 0; }
	inline bool IsDown() const { return (this->flags & FLAG_DOWN) != 0; }

	inline void SetText(const std::string& text) { this->txt.SetString(text); }
	inline std::string GetText() const { return this->txt.GetString().ToAnsiString(); }

	template<typename TCallback>
	void Poll(TCallback* receiver, void (TCallback::*func)())
	{
		if ((this->flags & FLAG_POLL) != 0)
		{
			receiver->*func();
			this->disableFlag(FLAG_POLL);
		}
	}

	bool Poll()
	{
		if ((this->flags & FLAG_POLL) != 0)
		{
			this->disableFlag(FLAG_POLL);
			return true;
		}
		return false;
	}
};

}
}
