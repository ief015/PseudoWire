#include "UIButton.h"

#include <sys/sys.h>
#include <SFML/Graphics.hpp>

#include "../Renderer.h"
#include "../Fonts.h"

using namespace game;
using namespace game::ui;

const char UIButton::FLAG_DOWN = 0x01;
const char UIButton::FLAG_HOVER = 0x02;
const char UIButton::FLAG_POLL = 0x04;

UIButton::UIButton(sys::State* s)
: sys::Component(s)
, flags(0)
, txt("", sf::Font::GetDefaultFont(), 12)
{
	txt.SetColor(sf::Color::Black);
}

UIButton::~UIButton()
{

}

void UIButton::Tick(float dt)
{

}

#define rw game::Renderer::Ref()
void UIButton::Draw(const sys::Point& scrpos)
{
	sf::Color coloutline, colfill;

	colfill = this->IsDown() ? sf::Color(225, 225, 225) : sf::Color(250, 250, 250);
	coloutline = this->IsMouseHovering() ? sf::Color(216, 108, 27) : sf::Color(123, 150, 185);

	sf::Shape box = sf::Shape::Rectangle(
		sf::FloatRect(scrpos.X, scrpos.Y, this->Size.X, this->Size.Y),
		colfill,
		1.0,
		coloutline );

	colfill = this->IsDown() ? sf::Color(180, 180, 180) : sf::Color(204, 204, 204);

	if (this->IsDown())
	{
		box.SetPointColor(0, colfill);
		box.SetPointColor(1, colfill);
	}
	else
	{
		box.SetPointColor(2, colfill);
		box.SetPointColor(3, colfill);
	}

	sf::FloatRect txtRect = txt.GetRect();
	txt.SetOrigin( (int)txtRect.Width / 2, (int)txtRect.Height / 2 );
	txt.SetPosition( scrpos.X + this->Size.X/2, scrpos.Y + this->Size.Y/2 - (this->IsDown()?1:2) );

	rw->Draw(box);
	rw->Draw(txt);
}
#undef rw

void UIButton::OnMouseEnter(int localx, int localy)
{
	this->enableFlag(FLAG_HOVER);
}

void UIButton::OnMouseLeave(int localx, int localy)
{
	this->disableFlag(FLAG_HOVER);
}

void UIButton::OnMouseClick(int localx, int localy, unsigned button)
{
	if (button == sf::Mouse::Left)
	{
		this->enableFlag(FLAG_DOWN);
		this->enableFlag(FLAG_POLL);
	}
}

void UIButton::OnMouseUp(int localx, int localy, unsigned button)
{
	if (button == sf::Mouse::Left)
	{
		this->disableFlag(FLAG_DOWN);
	}
}
