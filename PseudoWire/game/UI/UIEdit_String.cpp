#include "UIEdit_String.h"

#include <sys/sys.h>
#include "../Renderer.h"
#include "../Fonts.h"
#include "UIEdit.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

using namespace game;
using namespace game::ui;

const float UIEdit_String::TEXTBOX_WIDTH = 220.0f;

UIEdit_String::UIEdit_String(sys::State* s) : sys::Component(s)
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

	cursor = 0;
	allowtext = false;
}
UIEdit_String::~UIEdit_String()
{
}


void UIEdit_String::SetText(const char* str)
{
	text.SetString(str);
}

void UIEdit_String::SetString(const char* str)
{
	std::string s_str = sys::StringHelper::Replace(str, '\n', ' ');
	textbox_string = s_str;
	textbox_text.SetString(s_str);
	cursor = textbox_string.Length();
}

const char* UIEdit_String::GetString()
{
	return textbox_string.C_Str();
}

void UIEdit_String::Tick(float dt)
{
}

#define rw game::Renderer::Ref()
void UIEdit_String::Draw(const sys::Point& screenPos)
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
	if(allowtext)
	{
		//cursor/caret
		sf::Vector2f cursorvec = textbox_text.GetCharacterPos(cursor) + textbox_text.GetPosition();
		rw->Draw( sf::Shape::Line(cursorvec.x + 0.5f, cursorvec.y + 2.0f, cursorvec.x + 0.5f, cursorvec.y + 12.0f + 2.0f, 1.0f, sf::Color::Black) );
	}
	textbox_text.Move(-screenPos.X, -screenPos.Y);
}
#undef rw

void UIEdit_String::UpdateTextboxColour()
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
	
void UIEdit_String::OnMouseHover(int localx, int localy)
{
}
void UIEdit_String::OnMouseMoved(int localx, int localy, int dx, int dy)
{
}
void UIEdit_String::OnMouseMovedInside(int localx, int localy, int dx, int dy)
{
}
void UIEdit_String::OnMouseEnter(int localx, int localy)
{
}
void UIEdit_String::OnMouseLeave(int localx, int localy)
{
}
void UIEdit_String::OnMouseDown(int x, int y, unsigned int button)
{
}
void UIEdit_String::OnMouseUp(int x, int y, unsigned int button)
{
}
void UIEdit_String::OnMouseClick(int localx, int localy, unsigned int button)
{
	allowtext = ((localy >= 16) && (localx >= 4));
	UpdateTextboxColour();
}
void UIEdit_String::OnMouseUnclick(int localx, int localy, unsigned int button)
{
}
void UIEdit_String::OnMouseWheel(int localx, int localy, int d)
{
}
void UIEdit_String::OnMouseWheelInside(int localx, int localy, int d)
{
}
void UIEdit_String::OnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
	bool update_string = false;

	switch(key)
	{
	case 291: //left <-
		--cursor;
		if(cursor < 0) cursor = 0;
		break;
	case 292: //right ->
		++cursor;
		if(cursor > textbox_string.Length()) cursor = textbox_string.Length();
		break;
	case 293: //up ^
		break;
	case 294: //down v
		break;

	case 284: //Home
		cursor = 0;
		break;
	case 283: //End
		cursor = textbox_string.Length();
		break;

	case 286: //DEL
		if(cursor >= 0 && cursor < textbox_string.Length())
		{
			textbox_string.Erase(cursor, 1);
			update_string = true;
		}
		break;
	}
	
	if(update_string)
		textbox_text.SetString(textbox_string.Str_C());
}
void UIEdit_String::OnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_String::OnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_String::OnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_String::OnText(int key)
{
	switch(key)
	{
		default:
		{
			if(key == '\n')
			{
				return;
			}

			if(isprint(key))
			{
				char fakestring[2];
				sprintf(fakestring, "%c", static_cast<char>(key));
				textbox_string.Insert(fakestring, cursor);

				++cursor;
			}
		} break;

		case '\b': //BACKSPACE
		{
			if(cursor > 0 && cursor <= textbox_string.Length())
			{
				textbox_string.Erase(cursor-1, 1);
				--cursor;
			}
		} break;
	}

	textbox_text.SetString(textbox_string.Str_C());
}
