#include "UIEdit.h"
#include "UI_DATA.h"
#include "../GameData.h"

#include "../lua/lua.hpp"

#include <sys/sys.h>
#include "../LuaComponent.h"

#include "UIEdit_Check.h"
#include "UIEdit_Radio.h"
#include "UIEdit_String.h"
#include "UIEdit_StringML.h"
#include "UIEdit_Number.h"
#include "UIEdit_Key.h"

#include "../Fonts.h"
#include <SFML/Graphics.hpp>

using namespace game;
using namespace game::ui;

const int UIEdit::SPACE_BETWEEN = 8;
const int UIEdit::TITLEBAR_HEIGHT = 18;
const int UIEdit::WINDOW_WIDTH = 256;

sf::Image UIEdit::img_chkbox_on = sf::Image();
sf::Image UIEdit::img_chkbox_off = sf::Image();
sf::Image UIEdit::img_radio_on = sf::Image();
sf::Image UIEdit::img_radio_off = sf::Image();
sf::Image UIEdit::img_accept = sf::Image();
sf::Image UIEdit::img_decline = sf::Image();

UIEdit::UIEdit(sys::State* s) : sys::XComponent(s)
, c(NULL)
{
	UserData = (void*)(new ui::UI_DATATYPE(ui::UI_EDIT));
	opened = false;
	dragging_window = false;
	selectedClose = 0;

	titlebar_text = sf::Text(GD_NULLSTR, sf::Font::GetDefaultFont(), 12);
	titlebar_text.SetColor( sf::Color::Black );

	spr_accept.SetImage( img_accept );
	spr_decline.SetImage( img_decline );

	spr_accept.SetColor( sf::Color(196,196,196, 128) );
	spr_decline.SetColor( sf::Color(196,196,196, 128) );

	Size.X = WINDOW_WIDTH;
}
UIEdit::~UIEdit()
{
	delete UserData;
}

void UIEdit::StaticInit()
{
	img_chkbox_on.LoadFromFile(GD_RES_CHECKBOX_CHECKED);
	img_chkbox_off.LoadFromFile(GD_RES_CHECKBOX);
	img_radio_on.LoadFromFile(GD_RES_RADIO_CHECKED);
	img_radio_off.LoadFromFile(GD_RES_RADIO);

	img_accept.LoadFromFile(GD_RES_ACCEPT);
	img_decline.LoadFromFile(GD_RES_CANCEL);
}

void UIEdit::XTick(float dt)
{
}

#define rw game::Renderer::Ref()
void UIEdit::XDraw(const sys::Point& screenPos)
{
	rw->Draw(bg);
	rw->Draw(titlebar);
	rw->Draw(titlebar_text);

	rw->Draw(spr_accept);
	rw->Draw(spr_decline);
}
#undef rw

	
void UIEdit::XOnMouseHover(int localx, int localy)
{
}
void UIEdit::XOnMouseMoved(int localx, int localy, int dx, int dy)
{
	if(dragging_window)
	{
		Position.X += dx;
		Position.Y += dy;

		bg.Move(dx, dy);
		titlebar.Move(dx, dy);
		titlebar_text.Move(dx, dy);
		spr_accept.Move(dx, dy);
		spr_decline.Move(dx, dy);
	}
}
void UIEdit::XOnMouseMovedInside(int localx, int localy, int dx, int dy)
{
	int x = localx + Position.X;
	spr_accept.SetColor(sf::Color(196,196,196, 128));
	spr_decline.SetColor(sf::Color(196,196,196, 128));
	selectedClose = 0;

	if(localy < TITLEBAR_HEIGHT)
	{
		//in titlebar
		if(x >= spr_accept.GetPosition().x && x < spr_accept.GetPosition().x + img_accept.GetWidth())
		{
			//accept
			spr_accept.SetColor(sf::Color(255,255,255, 255));
			selectedClose = 1;
		}
		else if(x >= spr_decline.GetPosition().x && x < spr_decline.GetPosition().x + img_decline.GetWidth())
		{
			//decline
			spr_decline.SetColor(sf::Color(255,255,255, 255));
			selectedClose = 2;
		}
	}
}
void UIEdit::XOnMouseEnter(int localx, int localy)
{
}
void UIEdit::XOnMouseLeave(int localx, int localy)
{
	spr_accept.SetColor(sf::Color(196,196,196, 128));
	spr_decline.SetColor(sf::Color(196,196,196, 128));
	selectedClose = 0;
}
void UIEdit::XOnMouseDown(int x, int y, unsigned int button)
{
}
void UIEdit::XOnMouseUp(int x, int y, unsigned int button)
{
	dragging_window = false;
}
void UIEdit::XOnMouseClick(int localx, int localy, unsigned int button)
{
	if(button == 0)
	{
		if(selectedClose == 1)
		{
			//accept
			Close(false);
		}
		else if(selectedClose == 2)
		{
			//decline
			Close(true);
		}
		else
		{
			//drag window
			dragging_window = true;
		}
	}
}
void UIEdit::XOnMouseUnclick(int localx, int localy, unsigned int button)
{
}
void UIEdit::XOnMouseWheel(int localx, int localy, int d)
{
}
void UIEdit::XOnMouseWheelInside(int localx, int localy, int d)
{
}
void UIEdit::XOnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit::XOnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit::XOnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit::XOnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
}


void UIEdit::Move(int x, int y)
{
	Position.X += x;
	Position.Y += y;

	bg.Move(x, y);
	titlebar.Move(x, y);
	titlebar_text.Move(x, y);
	spr_decline.Move(x, y);
	spr_accept.Move(x, y);
}

bool UIEdit::Show(LuaComponent* comp)
{
	c = comp;

	sys::String ttl = GD_NULLSTR;
	ttl << c->myaddonitem->p_category->name << " - " << c->myaddonitem->name;
	titlebar_text.SetString(ttl.Str_C());

	//begin
	comp->allowNewEditComps = true;
	comp->luaCallObjectFunction( GD_LUA_FNAME_ONEDIT, 0, 1 );
	comp->allowNewEditComps = false;
	//end
	
	opened = false;

	if(lua_isboolean(comp->L, -1))
	{
		opened = (lua_toboolean(comp->L, -1) != 0);

		if(opened)
		{
			for(int i = 0; i < comp->editComponents.size(); ++i)
			{
				this->AddChild(comp->editComponents[i].c);
			}
		}

		Size.Y = TITLEBAR_HEIGHT + SPACE_BETWEEN;
		for(int i = 0; i < this->GetChildCount(); ++i)
		{
			sys::Component* ic = this->GetChild(i);

			ic->Position.Y = Size.Y;
			Size.Y += ic->Size.Y;
			Size.Y += SPACE_BETWEEN;
		}

		bg = sf::Shape::Rectangle( 
			sf::FloatRect(static_cast<float>(Position.X), static_cast<float>(Position.Y), static_cast<float>(WINDOW_WIDTH), static_cast<float>(Size.Y)),
			sf::Color(52, 52, 52),
			2.0f,
			//sf::Color::Black );
			sf::Color(42, 42, 42) );

		titlebar = sf::Shape::Rectangle( 
			sf::FloatRect(static_cast<float>(Position.X), static_cast<float>(Position.Y), static_cast<float>(WINDOW_WIDTH), static_cast<float>(TITLEBAR_HEIGHT)),
			sf::Color(205, 220, 255),
			1.0f,
			//sf::Color(225, 245, 255) );
			//sf::Color::Black );
			//sf::Color(105, 175, 195) );
			sf::Color(198, 205, 210) );

		titlebar.SetPointColor(2, sf::Color(160, 190, 210));
		titlebar.SetPointColor(3, sf::Color(160, 190, 210));

		titlebar_text.SetPosition(static_cast<float>(Position.X + 4.0f), static_cast<float>(Position.Y + 1.0f));
		
		spr_decline.SetPosition( static_cast<float>(WINDOW_WIDTH - img_decline.GetWidth()) + Position.X, 1.0f + Position.Y);
		spr_accept.SetPosition( static_cast<float>(WINDOW_WIDTH - img_decline.GetWidth() - 2.0f - img_accept.GetWidth()) + Position.X, 1.0f + Position.Y);
	}

	if(opened)
		comp->gamestate->AddComponent(this);

	spr_accept.SetColor(sf::Color(196,196,196, 128));
	spr_decline.SetColor(sf::Color(196,196,196, 128));
	selectedClose = 0;
	return opened;
}

void UIEdit::Close(bool cancel)
{
	if(!cancel)
	{
		//create settings table and call submit event
		lua_newtable(c->L);

		for(int i = 0; i < c->editComponents.size(); ++i)
		{
			//create key (by ref / unique_id)
			LuaComponent::EditComponent& editc = c->editComponents[i];
			if(editc.type == 0)
			{
				//unreferenced object (probably just a dummy (or an improperly handed object))
				continue;
			}

			lua_pushstring(c->L, editc.ref.c_str());
			
			//create value
			switch(editc.type)
			{
				case 1: //check
				{
					UIEdit_Check* uic = (UIEdit_Check*)editc.c;
					lua_pushboolean(c->L, uic->Checked() ? 1 : 0);
					break;
				}
				case 2: //number
				{
					UIEdit_Number* uin = (UIEdit_Number*)editc.c;
					lua_pushnumber(c->L, uin->GetNumber());
					break;
				}
				case 3: //radio
				{
					UIEdit_Radio* uir = (UIEdit_Radio*)editc.c;
					lua_pushboolean(c->L, uir->Checked() ? 1 : 0);
					break;
				}
				case 4: //string
				{
					UIEdit_String* uis = (UIEdit_String*)editc.c;
					lua_pushstring(c->L, uis->GetString());
					break;
				}
				case -4: //multiline string
				{
					UIEdit_StringML* uis = (UIEdit_StringML*)editc.c;
					lua_pushstring(c->L, uis->GetString());
					break;
				}
				case 5: //key
				{
					UIEdit_Key* uik = (UIEdit_Key*)editc.c;
					lua_pushnumber(c->L, uik->mykey);
					break;
				}
			}

			//set field
			lua_settable(c->L, -3);
		}

		c->luaCallObjectFunction( GD_LUA_FNAME_ONEDITSUBMIT, 1, 0 );
	}

	for(int i = 0; i < this->GetChildCount(); ++i)
	{
		this->RemoveChild(i--, true);
	}
	c->editComponents.clear();
	c->lastRadios.clear();

	c->gamestate->RemoveComponent(this);
}
