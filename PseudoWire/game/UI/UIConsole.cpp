#include "UIConsole.h"
#include <sys/sys.h>
#include "UI_DATA.h"
#include "../GameData.h"
#include <vector>

#include "../Renderer.h"
#include "../Fonts.h"

#include <SFML/Graphics.hpp>

using namespace game::ui;

float UIConsole::height = 20.0f;

sf::Image UIConsole::img_normal = sf::Image();
sf::Image UIConsole::img_info = sf::Image();
sf::Image UIConsole::img_warning = sf::Image();
sf::Image UIConsole::img_error = sf::Image();
sf::Image UIConsole::img_heart = sf::Image();
sf::Image UIConsole::img_arrow = sf::Image();

UIConsole::UIConsole(sys::State* s, float width, float _height)
: sys::Component(s)
{
	UserData = (void*)(new ui::UI_DATATYPE(ui::UI_CONSOLE));

	bgwidth = width;
	bgy = _height - height;

	console_opened = false;
	console_size = 10;

	Position.X = 0;
	Position.Y = bgy;
	Size.X = width;
	Size.Y = height;

	Resize(width, _height);

	txt = sf::Text("", sf::Font::GetDefaultFont(), 11);
	txt.SetColor(sf::Color(240, 240, 240));
	icon.SetImage(img_info);

	arrow.SetImage(img_arrow);
	arrow.SetColor( sf::Color(100, 100, 100) );
	arrow.SetOrigin( 8, 8 );
	arrow.SetRotation(180.0f);

	msgtype = MSG_TNORMAL;
	icon_notification = false;
	icon_notification_y = 0.0f;
	icon_notification_alpha = 255.0f;
}
UIConsole::~UIConsole()
{
	delete UserData;
}

void UIConsole::StaticInit()
{
	img_normal.LoadFromFile(GD_RES_COMMENT);
	img_info.LoadFromFile(GD_RES_INFORMATION);
	img_warning.LoadFromFile(GD_RES_ERROR);
	img_error.LoadFromFile(GD_RES_EXCLAMATION);
	img_heart.LoadFromFile(GD_RES_LESSTHANTHREE);
	img_arrow.LoadFromFile(GD_RES_ARROW);
}

void UIConsole::Resize(int w, int h)
{
	bgwidth = w;
	bgy = h - height;
	Position.Y = bgy;
	Size = sys::Point(w, h);

	bg = sf::Shape::Rectangle(0.0f, bgy, static_cast<float>(bgwidth), height, sf::Color(48, 48, 48), 1.0f, sf::Color(36, 36, 36));
	arrow.SetPosition( w - 16.0f + 8, bgy+2 + 8 );
	txt.SetPosition(20, bgy+3);
	icon.SetPosition(1, bgy+2);
}


void UIConsole::Tick(float dt)
{
}

void UIConsole::Draw(const sys::Point& screenpos)
{
#define rw Renderer::Ref()

	// Draw the console prompt (if open)

	if (console_opened)
	{
		float conheight = (console_size*17.0f);
		rw->Draw(sf::Shape::Rectangle(sf::FloatRect(0, bgy-conheight - 1, bgwidth, conheight + 1), sf::Color(0, 0, 0, 164), 1.0f, sf::Color(0, 0, 0, 196)));

		sf::Text t;
		sf::Sprite s;

		t = sf::Text("", sf::Font::GetDefaultFont(), 11);
		t.SetColor(sf::Color(240, 240, 240));

		for (int i = msgs.size()-1; i >= 0; --i)
		{
			UIConsole_Msg& m = msgs[i];

			t.SetString( m.msg );

			switch (m.type)
			{
				case MSG_TNORMAL: s.SetImage(img_normal); break;
				case MSG_TINFO: s.SetImage(img_info); break;
				case MSG_TINFO_NOCONSOLE: s.SetImage(img_info); break;
				case MSG_TWARNING: s.SetImage(img_warning); break;
				case MSG_TERROR: s.SetImage(img_error); break;
				case MSG_TWELCOME: s.SetImage(img_heart); break;
			}

			float yoffset = (i * 17.0f);

			s.SetPosition(1.0f, (bgy-conheight)+yoffset-1);
			t.SetPosition(20.0f, (bgy-conheight)+yoffset);

			rw->Draw(s);
			rw->Draw(t);
		}
	}

	// Draw the message prompt

	rw->Draw( bg );
	rw->Draw( arrow );
	rw->Draw( txt );

	rw->Draw( icon );

	if (icon_notification)
	{
		// icon notification animation

		icon_notification_y += 1.0f;
		icon_notification_alpha -= 12.5f;

		if (icon_notification_alpha <= 0.0f)
		{
			icon_notification = false;
		}
		else
		{
			icon.SetColor(sf::Color(255,255,255,static_cast<int>(icon_notification_alpha)));
			icon.Move(0.0f, -icon_notification_y);

			rw->Draw( icon );

			icon.Move(0.0f, icon_notification_y);
			icon.SetColor(sf::Color::White);
		}
	}
#undef rw
}


void UIConsole::Print(MESSAGE_TYPE type, const char* format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	
	char buf[0xFF];
	vsprintf(buf, format, argptr);

#ifdef _DEBUG
	if (type != MSG_TINFO_NOCONSOLE)
#else
	if (type != MSG_TNORMAL && type != MSG_TINFO_NOCONSOLE)
#endif
		sys::Dbg::Printf("%s\n", buf);

	// Print message
	if (type != MSG_TCONSOLE)
	{
		txt.SetString(buf);
		msgtype = type;

		switch (type)
		{
			case MSG_TNORMAL: icon.SetImage(img_normal); break;
			case MSG_TINFO: icon.SetImage(img_info); break;
			case MSG_TINFO_NOCONSOLE: icon.SetImage(img_info); break;
			case MSG_TWARNING: icon.SetImage(img_warning); break;
			case MSG_TERROR: icon.SetImage(img_error); break;
			case MSG_TWELCOME: icon.SetImage(img_heart); break;
		}

		if (type != MSG_TNORMAL && !console_opened)
		{
			icon_notification = true;
			icon_notification_y = 0.0f;
			icon_notification_alpha = 255;
		}

		if (type != MSG_TINFO_NOCONSOLE)
			msgs.push_back( UIConsole_Msg(type, buf) );
	}

	// If message count now surpasses limit, remove oldest message
	if (msgs.size() > console_size)
	{
		msgs.erase( msgs.begin() );
	}
}

void UIConsole::ToggleConsole()
{
	console_opened = !console_opened;

	if (console_opened)
		arrow.SetRotation(0.0f);
	else
		arrow.SetRotation(180.0f);
}

bool UIConsole::ConsoleOpened()
{
	return console_opened;
}


void UIConsole::OnMouseHover(int localx, int localy)
{
}
void UIConsole::OnMouseMoved(int localx, int localy, int dx, int dy)
{
}
void UIConsole::OnMouseMovedInside(int localx, int localy, int dx, int dy)
{
}
void UIConsole::OnMouseEnter(int localx, int localy)
{
}
void UIConsole::OnMouseLeave(int localx, int localy)
{
}
void UIConsole::OnMouseDown(int x, int y, unsigned int button)
{
}
void UIConsole::OnMouseUp(int x, int y, unsigned int button)
{
}
void UIConsole::OnMouseClick(int localx, int localy, unsigned int button)
{
	if (localx >= bgwidth-18)
		this->ToggleConsole();
}
void UIConsole::OnMouseUnclick(int localx, int localy, unsigned int button)
{
}
void UIConsole::OnMouseWheel(int localx, int localy, int d)
{
}
void UIConsole::OnMouseWheelInside(int localx, int localy, int d)
{
}
void UIConsole::OnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
}
void UIConsole::OnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}
void UIConsole::OnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
}
void UIConsole::OnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
}
