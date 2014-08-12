#include "UIMenu.h"
#include <sys/sys.h>
#include "UI_DATA.h"

#include "../GameState.h"
#include "../Renderer.h"
#include "../Fonts.h"

#include <SFML/Graphics.hpp>

using namespace game::ui;

float UIMenu::height = 20.0f;

UIMenu::UIMenu(sys::State* s, float width)
: sys::Component(s)
{
	UserData = (void*)(new ui::UI_DATATYPE(ui::UI_MENU));
	game = (game::GameState*)s;

	Resize(width, height);
}
UIMenu::~UIMenu()
{
	for(int i = 0; i < icons.size(); ++i)
	{
		delete icons[i]->GetImage();
		delete icons[i];
	}
	for(int i = 0; i < texts.size(); ++i)
	{
		delete texts[i];
	}
	delete UserData;
}

void UIMenu::Resize(int w, int h)
{
	bgwidth = w;
	bg = sf::Shape::Rectangle(0.0f, 0.0f, static_cast<float>(bgwidth), height, sf::Color(48, 48, 48), 1.0f, sf::Color(36, 36, 36));
	Size = sys::Point(w, (int)height);
}

void UIMenu::AddItem(const char* text, const char* iconfile)
{
	//new text
	sf::Text* txt = new sf::Text(text, Fonts::font_COMMS, 11);
	texts.push_back(txt);
	
	//new icon
	sf::Image* img = new sf::Image();
	img->LoadFromFile(iconfile);
	sf::Sprite* spr = new sf::Sprite();
	spr->SetImage(*img);
	icons.push_back(spr);
	
	//set positions
	int w = 0;
	for(int i = 0; i < widths.size(); ++i)
		w += widths[i];
	spr->SetPosition(w + 3, 2);
	txt->SetPosition(w + 6 + img->GetWidth(), 2);
	
	//new width
	w = 0;
	w += txt->GetRect().Width;
	w += img->GetWidth();
	w += 6;
	w += 6;
	widths.push_back(w);

	//all vectors should be the same size
	sys::Dbg::Assert(texts.size() == icons.size() && icons.size() == widths.size());
}

void UIMenu::EditItem(int idx, const char* text, const char* iconfile)
{
	//edit text
	sf::Text* txt = new sf::Text(text, Fonts::font_COMMS, 11);
	delete texts[idx];
	texts[idx] = txt;

	//edit icon
	sf::Image* img = new sf::Image();
	img->LoadFromFile(iconfile);
	sf::Sprite* spr = new sf::Sprite();
	spr->SetImage(*img);
	delete icons[idx];
	icons[idx] = spr;

	//set positions
	int w = 0;
	for(int i = 0; i < idx; ++i)
		w += widths[i];
	spr->SetPosition(w + 3, 2);
	txt->SetPosition(w + 6 + img->GetWidth(), 2);

	//edit width
	w = 0;
	w += txt->GetRect().Width;
	w += img->GetWidth();
	w += 6;
	w += 6;
	widths[idx] = w;

	//all vectors should be the same size
	sys::Dbg::Assert(texts.size() == icons.size() && icons.size() == widths.size());
}

void UIMenu::Update()
{
	for(int i = 0, t = 0; i < widths.size(); ++i)
	{
		int w = widths[i];
		
		icons[i]->SetPosition(t + 3, 2);
		texts[i]->SetPosition(t + 6 + icons[i]->GetImage()->GetWidth(), 2);

		t += w;
	}
}


void UIMenu::Tick(float dt)
{
}

void UIMenu::Draw(const sys::Point& screenpos)
{
#define rw Renderer::Ref()
	rw->Draw( bg );
	for(int i = 0, w = 0; i < widths.size(); ++i)
	{
		if(icons[i])
			rw->Draw(*icons[i]);
		rw->Draw(*texts[i]);

		if(i == selected)
		{
			rw->Draw( sf::Shape::Rectangle(static_cast<float>(w), 0, static_cast<float>(widths[i]), height, sf::Color(128, 200, 255, 64)) );
		}
		w += widths[i];
	}
#undef rw
}


void UIMenu::OnMouseHover(int localx, int localy)
{
}
void UIMenu::OnMouseMoved(int localx, int localy, int dx, int dy)
{
}
void UIMenu::OnMouseMovedInside(int localx, int localy, int dx, int dy)
{
	bool found = false;
	for(int i = 0, w = 0; i < widths.size(); ++i)
	{
		if(localx >= w && localx < w + widths[i])
		{
			selected = i;
			found = true;
			break;
		}
		w += widths[i];
	}
	if(!found)
	{
		selected = -1; // none
	}
}
void UIMenu::OnMouseEnter(int localx, int localy)
{
}
void UIMenu::OnMouseLeave(int localx, int localy)
{
	selected = -1; // none
}
void UIMenu::OnMouseDown(int x, int y, unsigned int button)
{
}
void UIMenu::OnMouseUp(int x, int y, unsigned int button)
{
}
void UIMenu::OnMouseClick(int localx, int localy, unsigned int button)
{
	if(button == 0)
		game->MenuButtonSelected(selected);
}
void UIMenu::OnMouseUnclick(int localx, int localy, unsigned int button)
{
}
void UIMenu::OnMouseWheel(int localx, int localy, int d)
{
}
void UIMenu::OnMouseWheelInside(int localx, int localy, int d)
{
}
void UIMenu::OnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
}
void UIMenu::OnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}
void UIMenu::OnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
}
void UIMenu::OnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
}
