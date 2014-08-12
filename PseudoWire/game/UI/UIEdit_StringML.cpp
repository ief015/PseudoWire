#include "UIEdit_StringML.h"

#include <sys/sys.h>
#include "../Renderer.h"
#include "../Fonts.h"
#include "UIEdit.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

using namespace game;
using namespace game::ui;

const float UIEdit_StringML::TEXTBOX_WIDTH = 220.0f;

UIEdit_StringML::UIEdit_StringML(sys::State* s) : sys::Component(s)
{
	text = sf::Text("", sf::Font::GetDefaultFont(), 12);
	text.SetPosition(4.0f, 0.0f);
	text.SetColor(sf::Color(238, 238, 238));

	AddLine();

	Size.X = 4.0f + TEXTBOX_WIDTH;
	Size.Y = 24;

	cursor = 0;
	cursor_line = 0;
	allowtext = false;
	firstline_shown = 0;

	SetHeight(8);
}
UIEdit_StringML::~UIEdit_StringML()
{
	for(int i = 0; i < textbox_text.size(); ++i)
		delete textbox_text[i];
}


void UIEdit_StringML::SetText(const char* str)
{
	text.SetString(str);
}

void UIEdit_StringML::SetString(const char* str)
{
	textbox_text.clear();
	textbox_string.clear();

	cursor = 0;
	cursor_line = 0;
	firstline_shown = 0;

	AddLine();

	sys::String buf;
	for(int i = 0, ln = 0; ; ++i)
	{
		if(str[i] == '\n')
		{
			textbox_string[ln] = buf;
			textbox_text[ln]->SetString(buf.C_Str());

			AddLine();
			buf.Clear();

			++ln;
			continue;
		}

		char buf_[2];
		buf_[0] = str[i];
		buf_[1] = '\0';
		buf += buf_;

		if(str[i] == '\0')
		{
			textbox_string[ln] = buf;
			textbox_text[ln]->SetString(buf.C_Str());
			break;
		}
	}
}

sys::String UIEdit_StringML::GetString()
{
	sys::String str;
	for(int i = 0; i < textbox_string.size(); ++i)
	{
		str += textbox_string[i];
		if(i != textbox_string.size()-1)
			str += GD_NEWLINE;
	}
	return str;
}

void UIEdit_StringML::AddLine()
{
	sf::Text* txt = new sf::Text(GD_NULLSTR, sf::Font::GetDefaultFont(), 12);
	txt->SetPosition(4.0f, 16.0f);
	txt->SetColor(sf::Color::Black);

	textbox_text.push_back(txt);
	textbox_string.push_back(sys::String(GD_NULLSTR));
}

void UIEdit_StringML::InsertLine()
{
	sf::Text* txt = new sf::Text(GD_NULLSTR, sf::Font::GetDefaultFont(), 12);
	txt->SetPosition(4.0f, 16.0f);
	txt->SetColor(sf::Color::Black);

	sys::String newstr = textbox_string[cursor_line];
	newstr.Erase(0, cursor); //get contents from cursor |-> and right

	//erase contents from cursor |-> and right
	textbox_string[cursor_line].Erase(cursor, textbox_string[cursor_line].Length()-cursor);
	textbox_text[cursor_line]->SetString(textbox_string[cursor_line].Str_C());

	//insert new text
	textbox_text.insert(textbox_text.begin() + cursor_line+1, txt);
	textbox_string.insert(textbox_string.begin() + cursor_line+1, sys::String(GD_NULLSTR));

	textbox_string[cursor_line+1] = newstr;

	++cursor_line;
	cursor = 0;

	if(cursor_line == firstline_shown+textbox_linesshown)
		++firstline_shown;
}

void UIEdit_StringML::SetHeight(int lines)
{
	textbox_linesshown = lines;
	textbox_height = static_cast<int>(16.0f + ((lines-1) * 12.0f ));
	Size.Y = 24 + textbox_height;

	textbox_shp = sf::Shape::Rectangle(
		sf::FloatRect(4.0f, 16.0f, TEXTBOX_WIDTH, static_cast<float>(textbox_height)),
		sf::Color(250, 250, 250),
		1.0,
		sf::Color(123, 150, 185)
	);
	textbox_shp.SetPointColor(0, sf::Color(204, 204, 204));
	textbox_shp.SetPointColor(1, sf::Color(204, 204, 204));
}

void UIEdit_StringML::Tick(float dt)
{
}

#define rw game::Renderer::Ref()
void UIEdit_StringML::Draw(const sys::Point& screenPos)
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

	for(int i = firstline_shown; i < textbox_text.size() && i < firstline_shown+textbox_linesshown; ++i)
	{
		sf::Text* txt = textbox_text[i];

		txt->Move(screenPos.X, screenPos.Y + (i-firstline_shown)*12.0f);
		rw->Draw(*txt);
		if(allowtext && i == cursor_line)
		{
			//cursor/caret
			sf::Vector2f cursorvec = txt->GetCharacterPos(cursor) + txt->GetPosition();
			rw->Draw( sf::Shape::Line(cursorvec.x + 0.5f, cursorvec.y + 2.0f, cursorvec.x + 0.5f, cursorvec.y + 12.0f + 2.0f, 1.0f, sf::Color::Black) );
		}
		txt->Move(-screenPos.X, -(screenPos.Y + (i-firstline_shown)*12.0f));
	}
}
#undef rw

void UIEdit_StringML::UpdateTextboxColour()
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
	
void UIEdit_StringML::OnMouseHover(int localx, int localy)
{
}
void UIEdit_StringML::OnMouseMoved(int localx, int localy, int dx, int dy)
{
}
void UIEdit_StringML::OnMouseMovedInside(int localx, int localy, int dx, int dy)
{
}
void UIEdit_StringML::OnMouseEnter(int localx, int localy)
{
}
void UIEdit_StringML::OnMouseLeave(int localx, int localy)
{
}
void UIEdit_StringML::OnMouseDown(int x, int y, unsigned int button)
{
}
void UIEdit_StringML::OnMouseUp(int x, int y, unsigned int button)
{
}
void UIEdit_StringML::OnMouseClick(int localx, int localy, unsigned int button)
{
	allowtext = ((localy >= 16) && (localx >= 4));
	UpdateTextboxColour();
}
void UIEdit_StringML::OnMouseUnclick(int localx, int localy, unsigned int button)
{
}
void UIEdit_StringML::OnMouseWheel(int localx, int localy, int d)
{
}
void UIEdit_StringML::OnMouseWheelInside(int localx, int localy, int d)
{
	if(d > 0)
	{
		//simulate key press up
		OnKeyPress(293, false, false, false);
	}
	else if(d < 0)
	{
		//simulate key press down
		OnKeyPress(294, false, false, false);
	}
}
void UIEdit_StringML::OnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
	bool update_string = false;

	switch(key)
	{
	case 291: //left <-
		--cursor;
		if(cursor < 0)
			cursor = 0;
		break;
	case 292: //right ->
		++cursor;
		if(cursor > textbox_string[cursor_line].Length())
			cursor = textbox_string[cursor_line].Length();
		break;
	case 293: //up ^
		if(cursor_line == firstline_shown)
		{
			--firstline_shown;
			if(firstline_shown < 0)
				firstline_shown = 0;
		}
		--cursor_line;
		if(cursor_line < 0)
			cursor_line = 0;

		if(cursor > textbox_string[cursor_line].Length())
				cursor = textbox_string[cursor_line].Length();
		break;
	case 294: //down v
		++cursor_line;
		if(cursor_line >= textbox_text.size())
			cursor_line = textbox_text.size()-1;
		if(cursor_line == firstline_shown+textbox_linesshown)
		{
			++firstline_shown;
		}

		if(cursor > textbox_string[cursor_line].Length())
				cursor = textbox_string[cursor_line].Length();
		break;
	case 281: //PgUp
		if(cursor_line == firstline_shown)
		{
			cursor_line -= textbox_linesshown;
			if(cursor_line < 0)
				cursor_line = 0;

			firstline_shown = cursor_line;

			if(cursor > textbox_string[cursor_line].Length())
				cursor = textbox_string[cursor_line].Length();
		}
		else
		{
			cursor_line = firstline_shown;

			if(cursor > textbox_string[cursor_line].Length())
				cursor = textbox_string[cursor_line].Length();
		}
		break;
	case 282: //PgDn
		if(cursor_line == firstline_shown+textbox_linesshown-1)
		{
			cursor_line += textbox_linesshown;
			if(cursor_line >= textbox_text.size())
				cursor_line = textbox_text.size()-1;

			firstline_shown = cursor_line-textbox_linesshown+1;

			if(cursor > textbox_string[cursor_line].Length())
				cursor = textbox_string[cursor_line].Length();
		}
		else
		{
			cursor_line = firstline_shown+textbox_linesshown-1;
			if(cursor_line >= textbox_text.size())
				cursor_line = textbox_text.size()-1;

			if(cursor > textbox_string[cursor_line].Length())
				cursor = textbox_string[cursor_line].Length();
		}
		break;
	case 284: //Home
		cursor = 0;
		break;
	case 283: //End
		cursor = textbox_string[cursor_line].Length();
		break;

	case 286: //DEL
		if(cursor >= textbox_string[cursor_line].Length())
		{
			if(cursor_line != textbox_string.size()-1)
			{
				// combine this line and below line to this line
				textbox_string[cursor_line].Append(textbox_string[cursor_line+1]);
				update_string = true;

				delete textbox_text[cursor_line+1];
				textbox_text.erase(textbox_text.begin() + cursor_line+1);
				textbox_string.erase(textbox_string.begin() + cursor_line+1);
			}
		}
		else if(cursor >= 0)
		{
			textbox_string[cursor_line].Erase(cursor, 1);
			update_string = true;
		}
		break;
	}
	
	if(update_string)
		textbox_text[cursor_line]->SetString(textbox_string[cursor_line].Str_C());
}
void UIEdit_StringML::OnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_StringML::OnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_StringML::OnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_StringML::OnText(int key)
{
	bool update_string = false;

	switch(key)
	{
		default:
		{
			if(key == '\n' || key == 13)
			{
				InsertLine();

				update_string = true;
			}
			else if(isprint(key))
			{
				char fakestring[2];
				sprintf(fakestring, "%c", static_cast<char>(key));
				textbox_string[cursor_line].Insert(fakestring, cursor);

				++cursor;

				update_string = true;
			}
		} break;

		case '\b': //BACKSPACE
		{
			if(cursor <= 0)
			{
				if(cursor_line != 0)
				{
					//combine this line and above line to above line
					textbox_string[cursor_line-1].Append(textbox_string[cursor_line]);
					textbox_text[cursor_line-1]->SetString(textbox_string[cursor_line-1].Str_C());

					cursor = textbox_string[cursor_line-1].Length() - textbox_string[cursor_line].Length();

					delete textbox_text[cursor_line];
					textbox_text.erase(textbox_text.begin() + cursor_line);
					textbox_string.erase(textbox_string.begin() + cursor_line);

					--cursor_line;

					if(cursor_line < firstline_shown)
						--firstline_shown;
				}
			}
			else if(cursor <= textbox_string[cursor_line].Length())
			{
				textbox_string[cursor_line].Erase(cursor-1, 1);
				--cursor;

				update_string = true;
			}

		} break;
	}

	if(update_string)
		textbox_text[cursor_line]->SetString(textbox_string[cursor_line].Str_C());
}
