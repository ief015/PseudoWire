#pragma once

#include <sys/sys.h>
#include <SFML/Graphics.hpp>

namespace game
{
namespace ui
{

enum WINDOW_RESULT
{
	RESULT_ACCEPT = 0,
	RESULT_CANCEL,

	NUM_RESULTS
};

class UIWindow_Static
{
public:
	static const int TITLEBAR_HEIGHT;

	static sf::Image img_chkbox_off;
	static sf::Image img_chkbox_on;

	static sf::Image img_radio_off;
	static sf::Image img_radio_on;

	static sf::Image img_accept;
	static sf::Image img_decline;

	static void StaticInit();
};

template<typename CallbackT>
class UIWindow : public sys::XComponent
{
public:
	UIWindow(sys::State* s) : sys::XComponent(s)
	{
		UserData = (void*)(new ui::UI_DATATYPE(ui::UI_WINDOW));
		opened = false;
		dragging_window = false;
		singleresult = false;
		selectedClose = 0;

		titlebar_text = sf::Text("", sf::Font::GetDefaultFont(), 12);
		titlebar_text.SetColor( sf::Color::Black );

		spr_accept.SetImage( UIWindow_Static::img_accept );
		spr_decline.SetImage( UIWindow_Static::img_decline );

		spr_accept.SetColor( sf::Color(196,196,196, 128) );
		spr_decline.SetColor( sf::Color(196,196,196, 128) );

		Size.X = 250;
		Size.Y = 32;
	}

	~UIWindow()
	{
		delete UserData;
	}

	void Move(int x, int y)
	{
		Position.X += x;
		Position.Y += y;

		bg.Move(x, y);
		titlebar.Move(x, y);
		titlebar_text.Move(x, y);
		spr_decline.Move(x, y);
		spr_accept.Move(x, y);
	}

protected:
	bool opened;
	bool dragging_window;
	bool singleresult;

	sf::Shape bg;
	sf::Shape titlebar;
	sf::Text titlebar_text;
	sf::Sprite spr_accept;
	sf::Sprite spr_decline;

	void XTick(float dt) {}

#define rw game::Renderer::Ref()
	void XDraw(const sys::Point& screenPos)
	{
		rw->Draw(bg);
		rw->Draw(titlebar);
		rw->Draw(titlebar_text);

		rw->Draw(spr_accept);
		if(!singleresult) rw->Draw(spr_decline);
	}
#undef rw

	int selectedClose;
	
	void XOnMouseHover(int localx, int localy) {}
	void XOnMouseMoved(int localx, int localy, int dx, int dy)
	{
		if(dragging_window)
		{
			Position.X += dx;
			Position.Y += dy;

			Show(singleresult); // refresh
		}
	}
	void XOnMouseMovedInside(int localx, int localy, int dx, int dy)
	{
		int x = localx + Position.X;
		spr_accept.SetColor(sf::Color(196,196,196, 128));
		spr_decline.SetColor(sf::Color(196,196,196, 128));
		selectedClose = 0;

		if(localy < UIWindow_Static::TITLEBAR_HEIGHT)
		{
			//in titlebar

			if(singleresult)
			{
				if(x >= spr_accept.GetPosition().x && x < spr_accept.GetPosition().x + UIWindow_Static::img_accept.GetWidth())
				{
					//accept
					spr_accept.SetColor(sf::Color(255,255,255, 255));
					selectedClose = 1;
				}
				else selectedClose = -1;
			}
			else
			{
				if(x >= spr_accept.GetPosition().x && x < spr_accept.GetPosition().x + UIWindow_Static::img_accept.GetWidth())
				{
					//accept
					spr_accept.SetColor(sf::Color(255,255,255, 255));
					selectedClose = 1;
				}
				else if(x >= spr_decline.GetPosition().x && x < spr_decline.GetPosition().x + UIWindow_Static::img_decline.GetWidth())
				{
					//decline
					spr_decline.SetColor(sf::Color(255,255,255, 255));
					selectedClose = 2;
				}
				else selectedClose = -1;
			}
		}
	}
	void XOnMouseEnter(int localx, int localy) {}
	void XOnMouseLeave(int localx, int localy)
	{
		spr_accept.SetColor(sf::Color(196,196,196, 128));
		spr_decline.SetColor(sf::Color(196,196,196, 128));
		selectedClose = 0;
	}
	void XOnMouseDown(int x, int y, unsigned int button) {}
	void XOnMouseUp(int x, int y, unsigned int button)
	{
		dragging_window = false;
	}
	void XOnMouseClick(int localx, int localy, unsigned int button)
	{
		if(button == 0)
		{
			bool close = true;
			if(selectedClose == 1)
			{
				//accept
				(OnResult_Obj->*OnResult)(RESULT_ACCEPT); //damn templates won't let me .cpp
			}
			else if(selectedClose == 2)
			{
				//decline
				(OnResult_Obj->*OnResult)(RESULT_CANCEL); //screw you, too
			}
			else if(selectedClose == -1)
			{
				//title bar
				dragging_window = true;
			}
			else close = false;

			if(close)
				Hide();
		}
	}
	void XOnMouseUnclick(int localx, int localy, unsigned int button) {}
	void XOnMouseWheel(int localx, int localy, int d) {}
	void XOnMouseWheelInside(int localx, int localy, int d) {}
	void XOnKeyPress(int key, bool shift, bool ctrl, bool alt) {}
	void XOnKeyRelease(int key, bool shift, bool ctrl, bool alt) {}
	void XOnKeyDown(int key, bool shift, bool ctrl, bool alt) {}
	void XOnKeyUp(int key, bool shift, bool ctrl, bool alt) {}
	
public:
	typedef void (CallbackT::*UIWindow_Callback)(WINDOW_RESULT res);
	UIWindow_Callback OnResult;
	CallbackT* OnResult_Obj;

	void UIWindow::Show(bool single_result = false)
	{
		singleresult = single_result;

		bg = sf::Shape::Rectangle( 
			sf::FloatRect(static_cast<float>(Position.X), static_cast<float>(Position.Y), static_cast<float>(Size.X), static_cast<float>(Size.Y)),
			sf::Color(52, 52, 52),
			2.0f,
			//sf::Color::Black );
			//sf::Color(80, 80, 80) );
			sf::Color(42, 42, 42) );

		titlebar = sf::Shape::Rectangle( 
			sf::FloatRect(static_cast<float>(Position.X), static_cast<float>(Position.Y), static_cast<float>(Size.X), static_cast<float>(UIWindow_Static::TITLEBAR_HEIGHT)),
			sf::Color(205, 220, 255),
			1.0f,
			//sf::Color::Black );
			//sf::Color(225, 245, 255) );
			//sf::Color(105, 175, 195) );
			sf::Color(198, 205, 210) );

		titlebar.SetPointColor(2, sf::Color(160, 190, 210));
		titlebar.SetPointColor(3, sf::Color(160, 190, 210));

		titlebar_text.SetPosition(static_cast<float>(Position.X + 4.0f), static_cast<float>(Position.Y + 1.0f));
		
		if(!singleresult)
		{
			spr_decline.SetPosition( static_cast<float>(Size.X - UIWindow_Static::img_decline.GetWidth()) + Position.X, 1.0f + Position.Y);
			spr_accept.SetPosition( static_cast<float>(Size.X - UIWindow_Static::img_decline.GetWidth() - 2.0f - UIWindow_Static::img_accept.GetWidth()) + Position.X, 1.0f + Position.Y);
		}
		else
		{
			spr_accept.SetPosition( static_cast<float>(Size.X - UIWindow_Static::img_accept.GetWidth() - 2.0f) + Position.X, 1.0f + Position.Y);
		}

		spr_accept.SetColor(sf::Color(196,196,196, 128));
		spr_decline.SetColor(sf::Color(196,196,196, 128));
		selectedClose = 0;

		opened = true;
	}

	void Hide()
	{
		opened = false;
	}

	bool IsOpened()
	{
		return opened;
	}

	WINDOW_RESULT UIWindow::GetResult()
	{
		switch(selectedClose)
		{
		case 1: return RESULT_ACCEPT;
		case 2: default: return RESULT_CANCEL;
		}
	}

	void SetTitle(const std::string& str)
	{
		titlebar_text.SetString(str);
	}
};

}
}
