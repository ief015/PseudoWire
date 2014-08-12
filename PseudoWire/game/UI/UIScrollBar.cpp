#include "UIScrollBar.h"

using namespace game;
using namespace game::ui;

#include <sys/sys.h>
#include "../Renderer.h"

#include <SFML/Graphics.hpp>

const int UIScrollBar::BOX_LENGTH_HALF = 8;

UIScrollBar::UIScrollBar(sys::State* s)
: sys::Component(s)
{
	this->min = 0;
	this->max = 100;
	this->val = 0;

	this->orientation = ORIENT_VERT;

	this->changed = false;
	this->clicked = false;

	this->scrAmt = 1;

	this->Size.X = BOX_LENGTH_HALF*2;
	this->Size.Y = 100;
}

UIScrollBar::~UIScrollBar()
{

}

void UIScrollBar::Tick(float dt)
{

}

#define rw game::Renderer::Ref()
void UIScrollBar::Draw(const sys::Point& screenPos)
{
	sf::Shape scrollbg;
	sf::Shape scrollbox;

	//float offset = this->getLocalBarO() * ( (float)this->GetPxLength() / (this->max - this->min) );
	float offset = this->getLocalBarO();

	if (this->orientation == ORIENT_VERT)
	{
		scrollbg = sf::Shape::Rectangle( sf::FloatRect(screenPos.X, screenPos.Y, this->GetPxWidth(), this->GetPxLength()), sf::Color(220, 220, 220) );
		scrollbg.SetPointColor(0, sf::Color(204, 204, 204));
		scrollbg.SetPointColor(3, sf::Color(204, 204, 204));
		
		scrollbox = sf::Shape::Rectangle( sf::FloatRect(screenPos.X, screenPos.Y, this->GetPxWidth(), BOX_LENGTH_HALF*2.0f), sf::Color(250, 250, 250), 1.0f, clicked ? sf::Color(216, 108, 27) : sf::Color(123, 150, 185) );

		sf::Color colfill = this->clicked ? sf::Color(180, 180, 180) : sf::Color(204, 204, 204);

		if (this->clicked)
		{
			scrollbox.SetPointColor(0, colfill);
			scrollbox.SetPointColor(1, colfill);
		}
		else
		{
			scrollbox.SetPointColor(2, colfill);
			scrollbox.SetPointColor(3, colfill);
		}

		scrollbox.Move(0.0f, offset);
	}
	else
	{
		scrollbg = sf::Shape::Rectangle( sf::FloatRect(screenPos.X, screenPos.Y, this->GetPxLength(), this->GetPxWidth()), sf::Color(220, 220, 220) );
		scrollbg.SetPointColor(1, sf::Color(204, 204, 204));
		scrollbg.SetPointColor(2, sf::Color(204, 204, 204));
		
		scrollbox = sf::Shape::Rectangle( sf::FloatRect(screenPos.X, screenPos.Y, BOX_LENGTH_HALF*2.0f, this->GetPxWidth()), sf::Color(250, 250, 250), 1.0f, clicked ? sf::Color(216, 108, 27) : sf::Color(123, 150, 185) );
	
		sf::Color colfill = this->clicked ? sf::Color(180, 180, 180) : sf::Color(204, 204, 204);

		if (this->clicked)
		{
			scrollbox.SetPointColor(0, colfill);
			scrollbox.SetPointColor(3, colfill);
		}
		else
		{
			scrollbox.SetPointColor(1, colfill);
			scrollbox.SetPointColor(2, colfill);
		}

		scrollbox.Move(offset, 0.0f);
	}

	rw->Draw( scrollbg );
	rw->Draw( scrollbox );
}
#undef rw

void UIScrollBar::OnMouseClick(int localx, int localy, unsigned int button)
{
	if (button != 0)
		return;

	int localo = (this->orientation == ORIENT_VERT ? localy : localx);

	if (!(localo >= this->getLocalBarO() && localo <= this->getLocalBarO() + (BOX_LENGTH_HALF*2)))
	{
		// Jump to mouse click
		this->val = localOToVal( localo );
		this->changed = true;
	}

	this->clicked = true;
}

void UIScrollBar::OnMouseUp(int localx, int localy, unsigned int button)
{
	if (button != 0)
		return;

	this->clicked = false;
}

void UIScrollBar::OnMouseMoved(int localx, int localy, int dx, int dy)
{
	if (clicked)
	{
		int localo = (this->orientation == ORIENT_VERT ? localy : localx);
		this->val = localOToVal( localo );

		this->changed = true;
	}
}

void UIScrollBar::OnMouseWheel(int localx, int localy, int d)
{
	if (this->IsFocused())
	{
		if (d < 0)
			this->ScrollUp();
		else if (d > 0)
			this->ScrollDown();
	}
}

void UIScrollBar::ScrollUp()
{
	this->SetValue( this->val + this->scrAmt );
}

void UIScrollBar::ScrollDown()
{
	this->SetValue( this->val - this->scrAmt );
}

void UIScrollBar::SetMin(int n)
{
	if (n > this->max)
		n = this->max;

	this->min = n;

	if (this->val > this->max)
	{
		this->val = this->max;
		this->changed = true;
	}
}

void UIScrollBar::SetMax(int n)
{
	if (n < this->min)
		n = this->min;

	this->max = n;

	if (this->val < this->min)
	{
		this->val = this->min;
		this->changed = true;
	}
}

void UIScrollBar::SetMinMax(int minimum, int maximum)
{
	this->min = minimum;
	this->max = maximum;

	if (this->max < this->min)
		this->max = this->min;
	else if (this->min > this->max) // sanity check
		this->min = this->max;
}

void UIScrollBar::SetValue(int v)
{
	if (v > this->max)
		v = this->max;
	else if(v < this->min)
		v = this->min;

	this->val = v;

	this->changed = true;
}

void UIScrollBar::SetOrientation(ORIENT orient)
{
	if (this->orientation == orient)
		return;
	
	this->orientation = orient;

	std::swap(this->Size.X, this->Size.Y);
}

int UIScrollBar::GetPxLength() const
{
	if (this->orientation == ORIENT_VERT)
		return this->Size.Y;
	return this->Size.X;
}

int UIScrollBar::GetPxWidth() const
{
	if (this->orientation == ORIENT_VERT)
		return this->Size.X;
	return this->Size.Y;
}

void UIScrollBar::SetPxLength(int length)
{
	if (this->orientation == ORIENT_VERT)
		this->Size.Y = length;
	else
		this->Size.X = length;
}

void UIScrollBar::SetPxWidth(int width)
{
	if (this->orientation == ORIENT_VERT)
		this->Size.X = width;
	else
		this->Size.Y = width;
}

bool UIScrollBar::PollChanged()
{
	if (this->changed)
	{
		this->changed = false;
		return true;
	}
	return false;
}

bool UIScrollBar::IsChanged() const
{
	return this->changed;
}

float UIScrollBar::getLocalBarO() const
{
	float p = static_cast<float>(this->val) - this->min;
	p /= (this->max - this->min);
	return sys::Math::Clamp<float>( (this->GetPxLength()-(BOX_LENGTH_HALF*2)) * sys::Math::Validate(p), 0.0f, static_cast<float>(this->GetPxLength()) );
}

int UIScrollBar::localOToVal(int o) const
{
	float p = static_cast<float>(o) / (this->GetPxLength()-(BOX_LENGTH_HALF*2));
	return sys::Math::Clamp<int>( static_cast<int>(p*(max-min)+min), this->min, this->max );
}
