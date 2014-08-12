#include "UIEdit_Label.h"
#include "../GameData.h"

#include <sys/sys.h>
#include "../Renderer.h"
#include "../Fonts.h"
#include <SFML/Graphics.hpp>

using namespace game;
using namespace game::ui;

const float UIEdit_Label::HEIGHT = 8.0f;

UIEdit_Label::UIEdit_Label(sys::State* s) : sys::Component(s)
{
	Size.Y = HEIGHT;

	txt = sf::Text(GD_NULLSTR, sf::Font::GetDefaultFont(), 12);
}
UIEdit_Label::~UIEdit_Label()
{
}

void UIEdit_Label::SetString(const char* str)
{
	txt.SetString(str);

	Size.X = txt.GetRect().Width;
	Size.Y = HEIGHT + txt.GetRect().Height;
}

void UIEdit_Label::Tick(float dt)
{
}

#define rw game::Renderer::Ref()
void UIEdit_Label::Draw(const sys::Point& screenPos)
{
	txt.Move(screenPos.X, screenPos.Y);
	rw->Draw(txt);
	txt.Move(-screenPos.X, -screenPos.Y);
}
#undef rw

void UIEdit_Label::OnMouseHover(int localx, int localy)
{
}
void UIEdit_Label::OnMouseMoved(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Label::OnMouseMovedInside(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Label::OnMouseEnter(int localx, int localy)
{
}
void UIEdit_Label::OnMouseLeave(int localx, int localy)
{
}
void UIEdit_Label::OnMouseDown(int x, int y, unsigned int button)
{
}
void UIEdit_Label::OnMouseUp(int x, int y, unsigned int button)
{
}
void UIEdit_Label::OnMouseClick(int localx, int localy, unsigned int button)
{
}
void UIEdit_Label::OnMouseUnclick(int localx, int localy, unsigned int button)
{
}
void UIEdit_Label::OnMouseWheel(int localx, int localy, int d)
{
}
void UIEdit_Label::OnMouseWheelInside(int localx, int localy, int d)
{
}
void UIEdit_Label::OnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Label::OnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Label::OnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Label::OnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
}
