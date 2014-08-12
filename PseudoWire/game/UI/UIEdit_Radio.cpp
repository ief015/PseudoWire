#include "UIEdit_Radio.h"

#include <sys/sys.h>
#include "../Renderer.h"
#include "../Fonts.h"
#include "UIEdit.h"
#include <SFML/Graphics.hpp>

using namespace game;
using namespace game::ui;

UIEdit_Radio::UIEdit_Radio(sys::State* s, const char* group_id, UIEdit_Radio* prev_radio) : sys::Component(s)
{
	Group = group_id;
	Previous = prev_radio;
	if(prev_radio)
		prev_radio->Next = this;
	Next = NULL;

	text = sf::Text("", sf::Font::GetDefaultFont(), 12);
	text.SetPosition(20.0f, 0.0f);
	text.SetColor(sf::Color(238, 238, 238));
	spr_chk.SetPosition( 2.0f, 0.0f );

	Size.X = 22 + text.GetRect().Width;
	Size.Y = 16;

	checked = false;
	spr_chk.SetImage(UIEdit::img_radio_off);
}
UIEdit_Radio::~UIEdit_Radio()
{
}


void UIEdit_Radio::SetText(const char* str)
{
	text.SetString(str);

	Size.X = 22 + text.GetRect().Width;
	Size.Y = 16;
}

void UIEdit_Radio::SetCheck(bool val)
{
	checked = val;
	if(checked)
		spr_chk.SetImage( UIEdit::img_radio_on );
	else
		spr_chk.SetImage( UIEdit::img_radio_off );
}


void UIEdit_Radio::Tick(float dt)
{
}

#define rw game::Renderer::Ref()
void UIEdit_Radio::Draw(const sys::Point& screenPos)
{
	spr_chk.Move(screenPos.X, screenPos.Y);
	rw->Draw( spr_chk );
	spr_chk.Move(-screenPos.X, -screenPos.Y);

	text.Move(screenPos.X, screenPos.Y);
	rw->Draw(text);
	text.Move(-screenPos.X, -screenPos.Y);
}
#undef rw

	
void UIEdit_Radio::OnMouseHover(int localx, int localy)
{
}
void UIEdit_Radio::OnMouseMoved(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Radio::OnMouseMovedInside(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Radio::OnMouseEnter(int localx, int localy)
{
}
void UIEdit_Radio::OnMouseLeave(int localx, int localy)
{
}
void UIEdit_Radio::OnMouseDown(int x, int y, unsigned int button)
{
}
void UIEdit_Radio::OnMouseUp(int x, int y, unsigned int button)
{
}
void UIEdit_Radio::OnMouseClick(int localx, int localy, unsigned int button)
{
	//find first radiobutton in group
	UIEdit_Radio* cur = this;
	while(cur->Previous != NULL)
		cur = cur->Previous;

	//uncheck all radios
	while(cur != NULL)
	{
		cur->SetCheck(false);
		cur = cur->Next;
	}

	//check ourself
	this->SetCheck(true);
}
void UIEdit_Radio::OnMouseUnclick(int localx, int localy, unsigned int button)
{
}
void UIEdit_Radio::OnMouseWheel(int localx, int localy, int d)
{
}
void UIEdit_Radio::OnMouseWheelInside(int localx, int localy, int d)
{
}
void UIEdit_Radio::OnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Radio::OnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Radio::OnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Radio::OnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
}
