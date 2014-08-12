#include "UIEdit_Number.h"

#include <sys/sys.h>
#include "../Renderer.h"
#include "../Fonts.h"
#include "UIEdit.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

using namespace game;
using namespace game::ui;

const float UIEdit_Number::TEXTBOX_WIDTH = 196.0f;

UIEdit_Number::UIEdit_Number(sys::State* s) : sys::Component(s)
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
UIEdit_Number::~UIEdit_Number()
{
}


void UIEdit_Number::SetText(const char* str)
{
	text.SetString(str);
}

void UIEdit_Number::SetString(const char* str)
{
	textbox_string = str;
	textbox_text.SetString(textbox_string.Str_C());
	cursor = textbox_string.Length();
}

double UIEdit_Number::GetNumber()
{
	double num = atof(textbox_string.C_Str());
	if(enable_minmax)
	{
		if(num < min)
			num = min;
		else if(num > max)
			num = max;
	}
	return num;
}

void UIEdit_Number::Tick(float dt)
{
}

#define rw game::Renderer::Ref()
void UIEdit_Number::Draw(const sys::Point& screenPos)
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

void UIEdit_Number::UpdateTextboxColour()
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
	
void UIEdit_Number::OnMouseHover(int localx, int localy)
{
}
void UIEdit_Number::OnMouseMoved(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Number::OnMouseMovedInside(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Number::OnMouseEnter(int localx, int localy)
{
}
void UIEdit_Number::OnMouseLeave(int localx, int localy)
{
}
void UIEdit_Number::OnMouseDown(int x, int y, unsigned int button)
{
}
void UIEdit_Number::OnMouseUp(int x, int y, unsigned int button)
{
}
void UIEdit_Number::OnMouseClick(int localx, int localy, unsigned int button)
{
	allowtext = ((localy >= 16) && (localx >= 4));
	UpdateTextboxColour();
}
void UIEdit_Number::OnMouseUnclick(int localx, int localy, unsigned int button)
{
}
void UIEdit_Number::OnMouseWheel(int localx, int localy, int d)
{
}
void UIEdit_Number::OnMouseWheelInside(int localx, int localy, int d)
{
}
void UIEdit_Number::OnKeyPress(int key, bool shift, bool ctrl, bool alt)
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

	case 286: //Del
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
void UIEdit_Number::OnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Number::OnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Number::OnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Number::OnText(int key)
{
	switch(key)
	{
		default:
		{
			if(isprint(key) && (isdigit(key) || key == '.' || key == '-' || key == '+' || key == 'e' || key == 'E'))
			{
				//already contains a '.'
				if(key == '.' && textbox_string.Str().find_first_of('.') != textbox_string.Str().npos)
					return;

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
