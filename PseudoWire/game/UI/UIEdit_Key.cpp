#include "UIEdit_Key.h"

#include <sys/sys.h>
#include "../Renderer.h"
#include "../Fonts.h"
#include "UIEdit.h"
#include "../LuaAPI.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

using namespace game;
using namespace game::ui;

const float UIEdit_Key::TEXTBOX_WIDTH = 128.0f;

UIEdit_Key::UIEdit_Key(sys::State* s) : sys::Component(s)
{
	text = sf::Text("", sf::Font::GetDefaultFont(), 12);
	text.SetPosition(4.0f, 0.0f);
	text.SetColor(sf::Color(238, 238, 238));

	textbox_text = sf::Text("", sf::Font::GetDefaultFont(), 12);
	textbox_text.SetPosition(4.0f, 16.0f);
	textbox_text.SetColor(sf::Color::Black);

	textbox_string = "";

	textbox_shp = sf::Shape::Rectangle(
		sf::FloatRect(4.0f, 16.0f, TEXTBOX_WIDTH, 16.0f),
		sf::Color(250, 250, 250),
		1.0,
		sf::Color(123, 150, 185)
	);
	textbox_shp.SetPointColor(0, sf::Color(204, 204, 204));
	textbox_shp.SetPointColor(1, sf::Color(204, 204, 204));

	Size.X = 4.0f + TEXTBOX_WIDTH;
	Size.Y = 32;

	allowtext = false;
	mykey = 0;
}
UIEdit_Key::~UIEdit_Key()
{
}


void UIEdit_Key::SetText(const char* str)
{
	text.SetString(str);
}


const char* UIEdit_Key::GetString()
{
	return textbox_string.C_Str();
}

void UIEdit_Key::Tick(float dt)
{
}

#define rw game::Renderer::Ref()
void UIEdit_Key::Draw(const sys::Point& screenPos)
{
	if(allowtext)
	{
		if(!this->IsFocused())
		{
			allowtext = false;
			this->UpdateTextboxColour();
		}
	}

	textbox_shp.Move(screenPos.X, screenPos.Y);
	rw->Draw(textbox_shp);
	textbox_shp.Move(-screenPos.X, -screenPos.Y);

	text.Move(screenPos.X, screenPos.Y);
	rw->Draw(text);
	text.Move(-screenPos.X, -screenPos.Y);

	textbox_text.Move(screenPos.X, screenPos.Y);
	rw->Draw(textbox_text);
	textbox_text.Move(-screenPos.X, -screenPos.Y);
}
#undef rw

void UIEdit_Key::UpdateTextboxColour()
{
	if(allowtext)
	{
		for(int i = 0; i < textbox_shp.GetPointsCount(); ++i)
			textbox_shp.SetPointOutlineColor(i, sf::Color(216, 108, 27));
	}
	else
	{
		for(int i = 0; i < textbox_shp.GetPointsCount(); ++i)
			textbox_shp.SetPointOutlineColor(i, sf::Color(123, 150, 185));
	}
}
	
void UIEdit_Key::OnMouseHover(int localx, int localy)
{
}
void UIEdit_Key::OnMouseMoved(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Key::OnMouseMovedInside(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Key::OnMouseEnter(int localx, int localy)
{
}
void UIEdit_Key::OnMouseLeave(int localx, int localy)
{
}
void UIEdit_Key::OnMouseDown(int x, int y, unsigned int button)
{
}
void UIEdit_Key::OnMouseUp(int x, int y, unsigned int button)
{
}
void UIEdit_Key::OnMouseClick(int localx, int localy, unsigned int button)
{
	allowtext = ((localy >= 16) && (localx >= 4));
	UpdateTextboxColour();
}
void UIEdit_Key::OnMouseUnclick(int localx, int localy, unsigned int button)
{
}
void UIEdit_Key::OnMouseWheel(int localx, int localy, int d)
{
}
void UIEdit_Key::OnMouseWheelInside(int localx, int localy, int d)
{
}
void UIEdit_Key::OnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
	this->mykey = key;
	textbox_text.SetString(KeycodeToString_C(key));
}
void UIEdit_Key::OnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Key::OnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Key::OnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Key::OnText(int key)
{
}
