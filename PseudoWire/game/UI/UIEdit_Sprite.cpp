#include "UIEdit_Sprite.h"
#include "../GameData.h"

#include <sys/sys.h>
#include "../Renderer.h"
#include "../Fonts.h"
#include "../GameLoader.h"
#include <SFML/Graphics.hpp>

using namespace game;
using namespace game::ui;

UIEdit_Sprite::UIEdit_Sprite(sys::State* s) : sys::Component(s)
{
	spr = sf::Sprite();
}
UIEdit_Sprite::~UIEdit_Sprite()
{
}

void UIEdit_Sprite::Load(const sf::Image& img)
{
	spr.SetImage( img );
}

void UIEdit_Sprite::Tick(float dt)
{
}

#define rw game::Renderer::Ref()
void UIEdit_Sprite::Draw(const sys::Point& screenPos)
{
	spr.Move(screenPos.X, screenPos.Y);
	rw->Draw(spr);
	spr.Move(-screenPos.X, -screenPos.Y);
}
#undef rw

void UIEdit_Sprite::OnMouseHover(int localx, int localy)
{
}
void UIEdit_Sprite::OnMouseMoved(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Sprite::OnMouseMovedInside(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Sprite::OnMouseEnter(int localx, int localy)
{
}
void UIEdit_Sprite::OnMouseLeave(int localx, int localy)
{
}
void UIEdit_Sprite::OnMouseDown(int x, int y, unsigned int button)
{
}
void UIEdit_Sprite::OnMouseUp(int x, int y, unsigned int button)
{
}
void UIEdit_Sprite::OnMouseClick(int localx, int localy, unsigned int button)
{
}
void UIEdit_Sprite::OnMouseUnclick(int localx, int localy, unsigned int button)
{
}
void UIEdit_Sprite::OnMouseWheel(int localx, int localy, int d)
{
}
void UIEdit_Sprite::OnMouseWheelInside(int localx, int localy, int d)
{
}
void UIEdit_Sprite::OnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Sprite::OnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Sprite::OnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Sprite::OnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
}
