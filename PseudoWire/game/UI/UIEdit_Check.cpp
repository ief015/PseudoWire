#include "UIEdit_Check.h"

#include <sys/sys.h>
#include "../Renderer.h"
#include "../Fonts.h"
#include "UIEdit.h"
#include <SFML/Graphics.hpp>

using namespace game;
using namespace game::ui;


UIEdit_Check::UIEdit_Check(sys::State* s) : sys::Component(s)
{
	text = sf::Text("", sf::Font::GetDefaultFont(), 12);
	text.SetPosition(20.0f, 0.0f);
	text.SetColor(sf::Color(238, 238, 238));
	spr_chk.SetPosition( 2.0f, 0.0f );

	Size.X = 22 + text.GetRect().Width;
	Size.Y = 16;

	checked = false;
	spr_chk.SetImage(UIEdit::img_chkbox_off);
}
UIEdit_Check::~UIEdit_Check()
{
}


void UIEdit_Check::SetText(const char* str)
{
	text.SetString(str);

	Size.X = 22 + text.GetRect().Width;
	Size.Y = 16;
}

void UIEdit_Check::SetCheck(bool val)
{
	checked = val;
	if(checked)
		spr_chk.SetImage( UIEdit::img_chkbox_on );
	else
		spr_chk.SetImage( UIEdit::img_chkbox_off );
}


void UIEdit_Check::Tick(float dt)
{
}

#define rw game::Renderer::Ref()
void UIEdit_Check::Draw(const sys::Point& screenPos)
{
	spr_chk.Move(screenPos.X, screenPos.Y);
	rw->Draw( spr_chk );
	spr_chk.Move(-screenPos.X, -screenPos.Y);

	text.Move(screenPos.X, screenPos.Y);
	rw->Draw(text);
	text.Move(-screenPos.X, -screenPos.Y);
}
#undef rw

	
void UIEdit_Check::OnMouseHover(int localx, int localy)
{
}
void UIEdit_Check::OnMouseMoved(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Check::OnMouseMovedInside(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Check::OnMouseEnter(int localx, int localy)
{
}
void UIEdit_Check::OnMouseLeave(int localx, int localy)
{
}
void UIEdit_Check::OnMouseDown(int x, int y, unsigned int button)
{
}
void UIEdit_Check::OnMouseUp(int x, int y, unsigned int button)
{
}
void UIEdit_Check::OnMouseClick(int localx, int localy, unsigned int button)
{
	//toggle checkbox
	this->SetCheck( ! this->Checked() );
}
void UIEdit_Check::OnMouseUnclick(int localx, int localy, unsigned int button)
{
}
void UIEdit_Check::OnMouseWheel(int localx, int localy, int d)
{
}
void UIEdit_Check::OnMouseWheelInside(int localx, int localy, int d)
{
}
void UIEdit_Check::OnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Check::OnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Check::OnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Check::OnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
}
