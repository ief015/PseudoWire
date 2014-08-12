#include "UIEdit_Separator.h"

#include <sys/sys.h>
#include "../Renderer.h"
#include "../Fonts.h"
#include <SFML/Graphics.hpp>

using namespace game;
using namespace game::ui;

const float UIEdit_Separator::HEIGHT = 12.0f;

UIEdit_Separator::UIEdit_Separator(sys::State* s) : sys::Component(s)
{
	Size.X = 256;
	Size.Y = HEIGHT;

	ln = sf::Shape::Line(16.0f, HEIGHT/2, Size.X - 16.0f, HEIGHT/2, 1.5f, sf::Color(96, 96, 96));
}
UIEdit_Separator::~UIEdit_Separator()
{
}

void UIEdit_Separator::Tick(float dt)
{
}

#define rw game::Renderer::Ref()
void UIEdit_Separator::Draw(const sys::Point& screenPos)
{
	ln.Move(screenPos.X, screenPos.Y);
	rw->Draw(ln);
	ln.Move(-screenPos.X, -screenPos.Y);
}
#undef rw

void UIEdit_Separator::OnMouseHover(int localx, int localy)
{
}
void UIEdit_Separator::OnMouseMoved(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Separator::OnMouseMovedInside(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Separator::OnMouseEnter(int localx, int localy)
{
}
void UIEdit_Separator::OnMouseLeave(int localx, int localy)
{
}
void UIEdit_Separator::OnMouseDown(int x, int y, unsigned int button)
{
}
void UIEdit_Separator::OnMouseUp(int x, int y, unsigned int button)
{
}
void UIEdit_Separator::OnMouseClick(int localx, int localy, unsigned int button)
{
}
void UIEdit_Separator::OnMouseUnclick(int localx, int localy, unsigned int button)
{
}
void UIEdit_Separator::OnMouseWheel(int localx, int localy, int d)
{
}
void UIEdit_Separator::OnMouseWheelInside(int localx, int localy, int d)
{
}
void UIEdit_Separator::OnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Separator::OnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Separator::OnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Separator::OnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
}
