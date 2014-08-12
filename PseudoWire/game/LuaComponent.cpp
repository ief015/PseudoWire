#include "LuaComponent.h"
#include "UI/UI_DATA.h"
#include "GameData.h"

#include <sys/sys.h>
#include "lua/lua.hpp"
#include "luna.h"
#include "LuaAPI.h"

#include "UI/UIEdit_Check.h"
#include "UI/UIEdit_Radio.h"
#include "UI/UIEdit_String.h"
#include "UI/UIEdit_StringML.h"
#include "UI/UIEdit_Number.h"
#include "UI/UIEdit_Key.h"
#include "UI/UIEdit_Separator.h"
#include "UI/UIEdit_Label.h"

#include "GameState.h"
#include "GameLoader.h"
#include "Fonts.h"
#include <SFML/Graphics.hpp>

#include "Port.h"

using namespace game;

sf::Image LuaComponent::DefaultImage = sf::Image();

/***************************************************************/
/***************************************************************/
/***************************************************************/

LuaComponent::LuaComponent(sys::State* parent)
: sys::Component(parent)
{
	gamestate = (GameState*)parent;
	Init();
}
LuaComponent::LuaComponent(lua_State* l)
: sys::Component( sys::Engine::Ref().GetState() )
{
	gamestate = NULL;
	Init();
}

void LuaComponent::Init()
{
	UserData = (void*)(new ui::UI_DATATYPE(ui::UI_LUACOMPONENT));
	_doneupdating = false;
	_ischanged = true;
	_forceischanged = false;
	_needsupdatenexttick = false;
	_deadend = false;
	_forcedeadend = false;
	_outputtriggered = false;
	_loopgate = false;
	_pathstackinit = false;
	allowNewEditComps = false;
	error = false;

	L = NULL;

	_hovering = false;

	MySprite = sf::Sprite(DefaultImage);
	Size.X = DefaultImage.GetWidth();
	Size.Y = DefaultImage.GetHeight();
}

void LuaComponent::StaticInit()
{
	if( !DefaultImage.LoadFromFile(GD_RES_DEFAULTGATE) )
	{
		DefaultImage.Create(48, 48, sf::Color(0x33,0x33,0x33));
	}
}

LuaComponent::~LuaComponent()
{
	_inputneedsupdating.Kill();

	if (UserData)
		delete UserData;
}

#pragma region Lua Component Events

void LuaComponent::Tick(float dt)
{
	lua_pushnumber(L, dt);
	luaCallObjectFunction(GD_LUA_FNAME_ONTICK, 1, 0);
}

void LuaComponent::Draw(const sys::Point& screenPos)
{
	if(_hovering)
	{
		Renderer::Ref()->Draw(sf::Shape::Rectangle( sf::FloatRect(screenPos.X, screenPos.Y, Size.X, Size.Y), sf::Color(0,0,0,0), 1.5f, sf::Color(245, 245, 245, 64) ));
	}

	// Draw sprite
	if(ShowSprite)
	{
		MySprite.SetPosition(screenPos.X, screenPos.Y);
		game::Renderer::Ref()->Draw(MySprite);
	}
	
	// Call Lua onDraw
	luaCallObjectFunction(GD_LUA_FNAME_ONDRAW);
}

void LuaComponent::OnMouseHover(int localx, int localy)
{
	gamestate->HoverComponent = this;
	_hovering = true;
	if(!gamestate->EditMode)
	{
		lua_pushinteger(L, localx);
		lua_pushinteger(L, localy);
		luaCallObjectFunction(GD_LUA_FNAME_ONMOUSEHOVER, 2, 0);
	}
}
void LuaComponent::OnMouseMoved(int localx, int localy, int dx, int dy)
{
	if(dx || dy) // only if the mouse on the screen actually moved
	{
		if(!gamestate->EditMode)
		{
			lua_pushinteger(L, localx);
			lua_pushinteger(L, localy);
			lua_pushinteger(L, dx);
			lua_pushinteger(L, dy);
			luaCallObjectFunction(GD_LUA_FNAME_ONMOUSEMOVED, 4, 0);
		}
	}
}
void LuaComponent::OnMouseMovedInside(int localx, int localy, int dx, int dy)
{
	if(dx || dy) // only if the mouse on the screen actually moved
	{
		if(!gamestate->EditMode)
		{
			lua_pushinteger(L, localx);
			lua_pushinteger(L, localy);
			lua_pushinteger(L, dx);
			lua_pushinteger(L, dy);
			luaCallObjectFunction(GD_LUA_FNAME_ONMOUSEMOVEDINSIDE, 4, 0);
		}
	}
}
void LuaComponent::OnMouseEnter(int localx, int localy)
{
	if(!gamestate->EditMode)
	{
		lua_pushnumber(L, localx);
		lua_pushnumber(L, localy);
		luaCallObjectFunction(GD_LUA_FNAME_ONMOUSEENTER, 2, 0);
	}
}
void LuaComponent::OnMouseLeave(int localx, int localy)
{
	if(!gamestate->EditMode)
	{
		lua_pushinteger(L, localx);
		lua_pushinteger(L, localy);
		luaCallObjectFunction(GD_LUA_FNAME_ONMOUSELEAVE, 2, 0);
	}
	gamestate->HoverComponent = NULL;
	_hovering = false;

}
void LuaComponent::OnMouseDown(int x, int y, unsigned int button)
{
	if(!gamestate->EditMode)
	{
		lua_pushnumber(L, x);
		lua_pushnumber(L, y);
		lua_pushnumber(L, button);
		luaCallObjectFunction(GD_LUA_FNAME_ONMOUSEDOWN, 3, 0);
	}
}
void LuaComponent::OnMouseUp(int x, int y, unsigned int button)
{
	if(!gamestate->EditMode)
	{
		lua_pushinteger(L, x);
		lua_pushinteger(L, y);
		lua_pushinteger(L, button);
		luaCallObjectFunction(GD_LUA_FNAME_ONMOUSEUP, 3, 0);
	}
}
void LuaComponent::OnMouseClick(int localx, int localy, unsigned int button)
{
	if(!gamestate->EditMode)
	{
		lua_pushinteger(L, localx);
		lua_pushinteger(L, localy);
		lua_pushinteger(L, button);
		luaCallObjectFunction(GD_LUA_FNAME_ONMOUSECLICK, 3, 0);
	}
}
void LuaComponent::OnMouseUnclick(int localx, int localy, unsigned int button)
{
	if(!gamestate->EditMode)
	{
		lua_pushinteger(L, localx);
		lua_pushinteger(L, localy);
		lua_pushinteger(L, button);
		luaCallObjectFunction(GD_LUA_FNAME_ONMOUSEUNCLICK, 3, 0);
	}
}
void LuaComponent::OnMouseWheel(int localx, int localy, int d)
{
	if(!gamestate->EditMode)
	{
		lua_pushinteger(L, localx);
		lua_pushinteger(L, localy);
		lua_pushinteger(L, d);
		luaCallObjectFunction(GD_LUA_FNAME_ONMOUSEWHEEL, 3, 0);
	}
}
void LuaComponent::OnMouseWheelInside(int localx, int localy, int d)
{
	if(!gamestate->EditMode)
	{
		lua_pushinteger(L, localx);
		lua_pushinteger(L, localy);
		lua_pushinteger(L, d);
		luaCallObjectFunction(GD_LUA_FNAME_ONMOUSEWHEELINSIDE, 3, 0);
	}
}
void LuaComponent::OnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
	if(!gamestate->EditMode)
	{
		lua_pushinteger(L, key);
		lua_pushboolean(L, shift ? 1 : 0);
		lua_pushboolean(L, ctrl ? 1 : 0);
		lua_pushboolean(L, alt ? 1 : 0);
		luaCallObjectFunction(GD_LUA_FNAME_ONKEYPRESSED, 4, 0);
	}
}
void LuaComponent::OnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
	if(!gamestate->EditMode)
	{
		lua_pushinteger(L, key);
		lua_pushboolean(L, shift ? 1 : 0);
		lua_pushboolean(L, ctrl ? 1 : 0);
		lua_pushboolean(L, alt ? 1 : 0);
		luaCallObjectFunction(GD_LUA_FNAME_ONKEYRELEASED, 4, 0);
	}
}
void LuaComponent::OnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
	if(!gamestate->EditMode)
	{
		lua_pushinteger(L, key);
		lua_pushboolean(L, shift ? 1 : 0);
		lua_pushboolean(L, ctrl ? 1 : 0);
		lua_pushboolean(L, alt ? 1 : 0);
		luaCallObjectFunction(GD_LUA_FNAME_ONKEYDOWN, 4, 0);
	}
}
void LuaComponent::OnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
	if(!gamestate->EditMode)
	{
		lua_pushinteger(L, key);
		lua_pushboolean(L, shift ? 1 : 0);
		lua_pushboolean(L, ctrl ? 1 : 0);
		lua_pushboolean(L, alt ? 1 : 0);
		luaCallObjectFunction(GD_LUA_FNAME_ONKEYUP, 4, 0);
	}
}

#pragma endregion

/***************************************************************/
/***************************************************************/
/***************************************************************/

#pragma region Lua Component Library "self"

/* **** REGISTRY **** */

const char LuaComponent::className[] = "Component";
Luna<LuaComponent>::RegType LuaComponent::registerMethods[] =
{
	// Object methods
	{"SetImage", &LuaComponent::SetImage},
	{"GetWidth", &LuaComponent::GetWidth},
	{"GetHeight", &LuaComponent::GetHeight},

	{"ToLocal", &LuaComponent::ToLocal},
	{"ToGlobal", &LuaComponent::ToGlobal},

	{"SetInputs", &LuaComponent::SetInputs},
	{"SetOutputs", &LuaComponent::SetOutputs},
	{"SetBoundsSize", &LuaComponent::SetBoundsSize},
	{"GetNumInputs", &LuaComponent::GetNumInputs},
	{"GetNumOutputs", &LuaComponent::GetNumOutputs},
	{"SetToolTip", &LuaComponent::SetToolTip},
	{"TriggerOutput", &LuaComponent::TriggerOutput},
	{"GetInputValue", &LuaComponent::GetInputValue},
	{"GetAllInputValues", &LuaComponent::GetAllInputValues},
	{"GetInputsTable", &LuaComponent::GetInputsTable},
	{"GetInputType", &LuaComponent::GetInputType},
	{"IsInputTriggered", &LuaComponent::IsInputTriggered},
	{"IsConnected", &LuaComponent::IsConnected},

	{"Draw", &LuaComponent::Draw},

	{"AddUINumber", &LuaComponent::AddUINumber},
	{"AddUIString", &LuaComponent::AddUIString},
	{"AddUICheck", &LuaComponent::AddUICheck},
	{"AddUIRadio", &LuaComponent::AddUIRadio},
	{"AddUIKey", &LuaComponent::AddUIKey},
	{"AddUISeparator", &LuaComponent::AddUISeparator},
	{"AddUILabel", &LuaComponent::AddUILabel},

	// End
	{ NULL, NULL }
};

/* **** Lua API Definitions **** */

// boolean SetImage(string filename)
int LuaComponent::SetImage(lua_State* l)
{
	const char* filename = luaL_checkstring(l, 1);

	if(strcmp(filename, GD_NULLSTR)==0)
	{
		ShowSprite = false;
	}
	else
	{
		sf::Image* img = AddonReader::Ref().GetImage(filename, myaddonitem->p_category->p_package->id);

		if(img)
		{
			MySprite.SetImage(*img);
			Size.X = img->GetWidth();
			Size.Y = img->GetHeight();
			MySprite.SetSubRect( sf::IntRect( 0, 0, Size.X, Size.Y ) );
		}
		else
		{
			gamestate->Console->Print(ui::UIConsole::MSG_TWARNING, GD_LUA_COMP_ERROR_IMAGENOTFOUND, filename);

			MySprite.SetImage(DefaultImage);
			Size.X = DefaultImage.GetWidth();
			Size.Y = DefaultImage.GetHeight();
			MySprite.SetSubRect( sf::IntRect( 0, 0, Size.X, Size.Y ) );

			lua_pushboolean(l, 0);
			return 1;
		}

		ShowSprite = true;
	}

	lua_pushboolean(l, 1);
	return 1;
}

// number GetWidth()
int LuaComponent::GetWidth(lua_State* l)
{
	lua_pushinteger(L, MySprite.GetImage()->GetWidth());
	return 1;
}

// number GetHeight()
int LuaComponent::GetHeight(lua_State* l)
{
	lua_pushinteger(L, MySprite.GetImage()->GetHeight());
	return 1;
}

// nil SetBoundsSize(Vector size)
int LuaComponent::SetBoundsSize(lua_State* l)
{
	Vector* v = Luna<Vector>::check(l, 1);
	Size.X = v->myVec->x;
	Size.Y = v->myVec->y;
	return 0;
}

// Vector GetBoundsSize()
int LuaComponent::GetBoundsSize(lua_State* l)
{
	Vector* v = new Vector(l);
	v->myVec->x = Size.X;
	v->myVec->y = Size.Y;
	Luna<Vector>::pushobject(l, v);
	return 1;
}

// Vector ToLocal(Vector global)
int LuaComponent::ToLocal(lua_State* l)
{
	Vector* g = Luna<Vector>::check(l, 1);
	Vector* local = new Vector(l);
	local->myVec->x = g->myVec->x + this->Position.X;
	local->myVec->y = g->myVec->y + this->Position.Y;
	Luna<Vector>::pushobject(l, local);
	return 1;
}

// Vector ToGlobal(Vector local)
int LuaComponent::ToGlobal(lua_State* l)
{
	Vector* local = Luna<Vector>::check(l, 1);
	Vector* g = new Vector(l);
	g->myVec->x = local->myVec->x - this->Position.X;
	g->myVec->y = local->myVec->y - this->Position.Y;
	Luna<Vector>::pushobject(l, g);
	return 1;
}

// nil SetInputs(table ports)
int LuaComponent::SetInputs(lua_State* l)
{
	luaL_checktype(l, 1, LUA_TTABLE);

	for(int i = 0; i < InputPorts.size(); ++i)
		this->UnconnectInput(i);

	InputPorts.clear();

	lua_pushnil(l);
	while(lua_next(l, 1))
	{
		if(lua_type(l, -1) == LUA_TTABLE)
		{
			//inner tables

			lua_pushnil(l);
			do //easy escape! :D
			{
				InPort newin;

				//string name
				lua_next(l, -2);
				if(lua_isstring(l, -1))
				{
					newin.port_name = lua_tostring(l, -1);
					lua_pop(l, 1);
				}
				else
				{
					lua_pop(l, 1);
					break;
				}

				//number enum type
				lua_next(l, -2);
				if(lua_isnumber(l, -1))
				{
					switch(lua_toint(l, -1))
					{
					case TYPE_NUMBER: newin.val_type = TYPE_NUMBER; break;
					case TYPE_STRING: newin.val_type = TYPE_STRING; break;
					default: case TYPE_ANY: newin.val_type = TYPE_ANY;
					}
					lua_pop(l, 1);
				}
				else
				{
					lua_pop(l, 1);
					break;
				}

				//number X
				lua_next(l, -2);
				if(lua_isnumber(l, -1))
				{
					newin.x = lua_tonumber(l, -1);
					lua_pop(l, 1);
				}
				else
				{
					lua_pop(l, 1);
					break;
				}

				//number Y
				lua_next(l, -2);
				if(lua_isnumber(l, -1))
				{
					newin.y = lua_tonumber(l, -1);
					lua_pop(l, 1);
				}
				else
				{
					lua_pop(l, 1);
					break;
				}

				//done!
				InputPorts.push_back(newin);

				lua_pop(l, 1);
			} while(false);

			lua_pop(l, 1);
		}
		else
		{
			lua_pop(l, 1);
			break;
		}
	}
	lua_pop(l, 1);

	_inputneedsupdating.Create( InputPorts.size() );

	//kill input wire point arrays
	for(unsigned i = 0; i < InputWirePoints.size(); ++i)
	{
		InputWirePoints[i].Kill();
	}
	InputWirePoints.clear();

	//'zero' _inputneedsupdating array just in case, and create InputWirePoints array
	for(unsigned i = 0; i < _inputneedsupdating.Size(); ++i)
	{
		_inputneedsupdating[i] = false;
		InputWirePoints.push_back( sys::ArrayOf<sys::Vectori>() );
	}

	return 0;
}

// nil SetOutputs(table ports)
int LuaComponent::SetOutputs(lua_State* l)
{
	luaL_checktype(l, 1, LUA_TTABLE);

	for(unsigned i = 0; i < OutputPorts.size(); ++i)
		this->UnconnectOutput(i);

	OutputPorts.clear();

	lua_pushnil(l);
	while(lua_next(l, 1))
	{
		if(lua_type(l, -1) == LUA_TTABLE)
		{
			//inner tables

			lua_pushnil(l);
			do //easy escape! :D
			{
				OutPort newout;

				//string name
				lua_next(l, -2);
				if(lua_isstring(l, -1))
				{
					newout.port_name = lua_tostring(l, -1);
					lua_pop(l, 1);
				}
				else
				{
					lua_pop(l, 1);
					break;
				}

				//number enum type
				lua_next(l, -2);
				if(lua_isnumber(l, -1))
				{
					switch(lua_toint(l, -1))
					{
					case TYPE_NUMBER: default: newout.val_type = TYPE_NUMBER; break;
					case TYPE_STRING: newout.val_type = TYPE_STRING; break;
					case TYPE_ANY:
						{
							lua_pop(L, 4);
							lua_pushstring(L, GD_LUA_COMP_ERROR_OUTPUTANY);
							lua_error(L);
							break;
						}
					}
					lua_pop(l, 1);
				}
				else
				{
					lua_pop(l, 1);
					break;
				}

				//number X
				lua_next(l, -2);
				if(lua_isnumber(l, -1))
				{
					newout.x = lua_tonumber(l, -1);
					lua_pop(l, 1);
				}
				else
				{
					lua_pop(l, 1);
					break;
				}

				//number Y
				lua_next(l, -2);
				if(lua_isnumber(l, -1))
				{
					newout.y = lua_tonumber(l, -1);
					lua_pop(l, 1);
				}
				else
				{
					lua_pop(l, 1);
					break;
				}

				//done!
				OutputPorts.push_back(newout);

				lua_pop(l, 1);
			} while(false);

			lua_pop(l, 1);
		}
		else
		{
			lua_pop(l, 1);
			break;
		}
	}
	lua_pop(l, 1);

	return 0;
}

// nil SetToolTip(string tip)
int LuaComponent::SetToolTip(lua_State* l)
{
	this->Tooltip.assign(lua_tostring(l, 1));
	return 0;
}

// number GetNumInputs()
int LuaComponent::GetNumInputs(lua_State* l)
{
	lua_pushinteger(l, InputPorts.size());
	return 1;
}

// number GetNumOutputs()
int LuaComponent::GetNumOutputs(lua_State* l)
{
	lua_pushinteger(l, OutputPorts.size());
	return 1;
}

// number GetInputValue(number port)
// string GetInputValue(number port)
int LuaComponent::GetInputValue(lua_State* l)
{
	int port = luaL_checkint(l, 1)-1;

	if(port < 0 || port >= this->InputPorts.size())
	{
		lua_pushstring(l, GD_LUA_COMP_ERROR_INVALIDINPUTINDEX);
		return lua_error(l);
	}

	Opposition* opp = &InputPorts[port].opposing;

	if(this->InputPorts[port].val_type == TYPE_NUMBER)
	{
		if(opp->comp)
		{
			lua_pushnumber(l, opp->comp->OutputPorts[opp->port].val_num);
		}
		else
		{
			lua_pushnumber(l, 0.0);
		}
	}
	else if(this->InputPorts[port].val_type == TYPE_STRING)
	{
		if(opp->comp)
		{
			lua_pushstring(l, opp->comp->OutputPorts[opp->port].val_str.c_str());
		}
		else
		{
			lua_pushstring(l, "");
		}
	}
	else if(this->InputPorts[port].val_type == TYPE_ANY)
	{
		if(opp->comp)
		{
			if(opp->comp->OutputPorts[opp->port].val_type == TYPE_NUMBER)
			{
				lua_pushnumber(l, opp->comp->OutputPorts[opp->port].val_num);
			}
			else if(opp->comp->OutputPorts[opp->port].val_type == TYPE_STRING)
			{
				lua_pushstring(l, opp->comp->OutputPorts[opp->port].val_str.c_str());
			}
			else
			{
				lua_pushnil(l);
			}
		}
		else
		{
			lua_pushnil(l);
		}
	}
	else
	{
		lua_pushstring(l, GD_LUA_COMP_ERROR_INVALIDINPUTTYPE);
		return lua_error(l);
	}

	return 1;
}

// ... GetAllInputValues()
int LuaComponent::GetAllInputValues(lua_State* l)
{
	if(this->InputPorts.size() == 0)
	{
		lua_pushnil(l);
		return 1;
	}

	for(unsigned i = 0; i < InputPorts.size(); ++i)
	{
		Opposition* opp = &InputPorts[i].opposing;

		if(this->InputPorts[i].val_type == TYPE_NUMBER)
		{
			if(opp->comp)
			{
				lua_pushnumber(l, opp->comp->OutputPorts[opp->port].val_num);
			}
			else
			{
				lua_pushnumber(l, 0.0);
			}
		}
		else if(this->InputPorts[i].val_type == TYPE_STRING)
		{
			if(opp->comp)
			{
				lua_pushstring(l, opp->comp->OutputPorts[opp->port].val_str.c_str());
			}
			else
			{
				lua_pushstring(l, "");
			}
		}
		else if(this->InputPorts[i].val_type == TYPE_ANY)
		{
			if(opp->comp)
			{
				if(opp->comp->OutputPorts[opp->port].val_type == TYPE_NUMBER)
				{
					lua_pushnumber(l, opp->comp->OutputPorts[opp->port].val_num);
				}
				else if(opp->comp->OutputPorts[opp->port].val_type == TYPE_STRING)
				{
					lua_pushstring(l, opp->comp->OutputPorts[opp->port].val_str.c_str());
				}
				else
				{
					lua_pushnil(l);
				}
			}
			else
			{
				lua_pushnil(l);
			}
		}
		else
		{
			lua_pushstring(l, GD_LUA_COMP_ERROR_INVALIDINPUTTYPE);
			return lua_error(l);
		}
	}

	return InputPorts.size();
}

// table GetInputsTable()
int LuaComponent::GetInputsTable(lua_State* l)
{
	lua_newtable(l);

	for(unsigned i = 0; i < InputPorts.size(); ++i)
	{
		lua_pushnumber(l, i+1);

		Opposition* opp = &InputPorts[i].opposing;

		if(this->InputPorts[i].val_type == TYPE_NUMBER)
		{
			if(opp->comp)
			{
				lua_pushnumber(l, opp->comp->OutputPorts[opp->port].val_num);
			}
			else
			{
				lua_pushnumber(l, 0.0);
			}
		}
		else if(this->InputPorts[i].val_type == TYPE_STRING)
		{
			if(opp->comp)
			{
				lua_pushstring(l, opp->comp->OutputPorts[opp->port].val_str.c_str());
			}
			else
			{
				lua_pushstring(l, "");
			}
		}
		else if(this->InputPorts[i].val_type == TYPE_ANY)
		{
			if(opp->comp)
			{
				if(opp->comp->OutputPorts[opp->port].val_type == TYPE_NUMBER)
				{
					lua_pushnumber(l, opp->comp->OutputPorts[opp->port].val_num);
				}
				else if(opp->comp->OutputPorts[opp->port].val_type == TYPE_STRING)
				{
					lua_pushstring(l, opp->comp->OutputPorts[opp->port].val_str.c_str());
				}
				else
				{
					lua_pushnil(l);
				}
			}
			else
			{
				lua_pushnil(l);
			}
		}
		else
		{
			lua_pushstring(l, GD_LUA_COMP_ERROR_INVALIDINPUTTYPE);
			return lua_error(l);
		}

		lua_settable(l, -3);
	}

	return 1;
}

// number GetInputType(number port)
int LuaComponent::GetInputType(lua_State* l)
{
	int port = luaL_checkint(L, 1)-1;
	if(port < 0 || port >= this->InputPorts.size())
	{
		lua_pushstring(L, GD_LUA_COMP_ERROR_INVALIDOUTPUTINDEX);
		return lua_error(L);
	}

	Opposition& opp = this->InputPorts[port].opposing;
	if(opp.comp)
	{
		//push connected output's type to get the type of values being inputted
		lua_pushnumber(l, static_cast<double>(opp.comp->OutputPorts[opp.port].val_type));
	}
	else
	{
		//not connected
		lua_pushinteger(l, 0);
	}

	return 1;
}

// boolean IsInputTriggered(number port)
int LuaComponent::IsInputTriggered(lua_State* l)
{
	int port = luaL_checkint(L, 1)-1;
	if(port < 0 || port >= this->InputPorts.size())
	{
		lua_pushstring(L, GD_LUA_COMP_ERROR_INVALIDOUTPUTINDEX);
		return lua_error(L);
	}

	// true if input has changed for this onInputsTriggered event
	lua_pushboolean(l, this->_inputneedsupdating[port] ? 1 : 0);
	return 1;
}

// nil TriggerOutput(number port, number value)
// nil TriggerOutput(number port, string value)
int LuaComponent::TriggerOutput(lua_State* l)
{
	int port = luaL_checkint(L, 1)-1;

	if(port < 0 || port >= this->OutputPorts.size())
	{
		lua_pushstring(L, GD_LUA_COMP_ERROR_INVALIDOUTPUTINDEX);
		return lua_error(L);
	}

	OutPort& outport = this->OutputPorts[port];
	
	if(lua_isnumber(L, 2) && outport.val_type == TYPE_NUMBER)
	{
		outport.val_num = lua_tonumber(L, 2);
	}
	else if(lua_isstring(L, 2) && outport.val_type == TYPE_STRING)
	{
		outport.val_str = lua_tostring(L, 2);
	}
	else
	{
		outport.draw_state = DRAW_STATE_ERROR;
		lua_pushstring(L, GD_LUA_COMP_ERROR_INVALIDOUTPUTTYPE);
		return lua_error(L);
	}

	// Update input states of connected components
	for (int i = 0, sz = outport.opposing_list.size(); i < sz; ++i)
	{
		Opposition& opp = outport.opposing_list[i];

		if (opp.comp)
			opp.comp->_inputneedsupdating[opp.port] = true;
	}

	outport.draw_state = DRAW_STATE_TRIGGERED;

	_outputtriggered = true;

	return 0;
}

// bool IsConnected(number port)
int LuaComponent::IsConnected(lua_State* l)
{
	int port = luaL_checkint(L, 1)-1;

	if(port < 0 || port >= this->InputPorts.size())
	{
		lua_pushstring(L, GD_LUA_COMP_ERROR_INVALIDOUTPUTINDEX);
		return lua_error(L);
	}

	lua_pushboolean(l, (this->InputPorts[port].opposing.comp != NULL) ? 1 : 0);

	return 1;
}

// nil Draw(Sprite spr)
// nil Draw(Shape shp)
// nil Draw(Text txt)
// nil Draw(RenderTexture rt)
int LuaComponent::Draw(lua_State* l)
{
#define rw Renderer::Ref()

	//get class name
	void* ptr = NULL;

	if(ptr = (void*)Luna<Sprite>::totype(l, 1))
	{
		//Sprite
		Sprite* spr = (Sprite*)ptr;
		spr->mySprite->Move(this->Position.X, this->Position.Y); //move to screen pos
		rw->Draw(*spr->mySprite);
		spr->mySprite->Move(-this->Position.X, -this->Position.Y); //move back to local pos
	}
	else if(ptr = (void*)Luna<Shape>::totype(l, 1))
	{
		//Shape
		//Shapes have many points to move. So to make this fast in most cases, change view instead of moving
		Shape* shp = (Shape*)ptr;
		sf::View v = rw->GetView();
		v.Move(-this->Position.X, -this->Position.Y);
		rw->SetView(v);
		rw->Draw(*shp->myShape);
		v.Move(this->Position.X, this->Position.Y);
		rw->SetView(v);
	}
	else if(ptr = (void*)Luna<Text>::totype(l, 1))
	{
		//Text
		Text* txt = (Text*)ptr;
		txt->myText->Move(this->Position.X, this->Position.Y); //move to screen pos
		rw->Draw(*txt->myText);
		txt->myText->Move(-this->Position.X, -this->Position.Y); //move back to local pos
	}
	else if(ptr = (void*)Luna<RenderTexture>::totype(l, 1))
	{
		//RenderTexture
		RenderTexture* rt = (RenderTexture*)ptr;
		rt->mySprite->Move(this->Position.X, this->Position.Y); //move to screen pos
		rt->mySprite->SetImage(rt->myRenderImage->GetImage());
		rw->Draw(*rt->mySprite);
		rt->mySprite->Move(-this->Position.X, -this->Position.Y); //move back to local pos
	}

	return 0;
#undef rw
}

// nil AddUICheck(string unique_id, string text [, bool checked = false])
int LuaComponent::AddUICheck(lua_State* L)
{
	if(!this->allowNewEditComps) return 0;

	//reference checkbox
	EditComponent editc;
	editc.ref = luaL_checkstring(L, 1);
	editc.type = 1;

	//check if reference is already in use
	for(unsigned i = 0; i < editComponents.size(); ++i)
	{
		if(editComponents[i].ref == editc.ref)
		{
			//reference already in use, dispatch error
			lua_pushstring(L, GD_LUA_COMP_ERROR_DUPLICATEIDS);
			return lua_error(L);
		}
	}

	//create checkbox
	ui::UIEdit_Check* c = new ui::UIEdit_Check(GetParentState());
	editc.c = c;

	//set text
	c->SetText(luaL_checkstring(L, 2));

	//optional boolean, auto-check
	if(lua_isboolean(L, 3))
		c->SetCheck( lua_toboolean(L, 3)!=0 );

	editComponents.push_back( editc );
	return 0;
}

// nil AddUINumber(string unique_id, string text [, number default = 0 [, number min, number max] ])
int LuaComponent::AddUINumber(lua_State* L)
{
	if(!this->allowNewEditComps) return 0;

	//reference radio button
	EditComponent editc;
	editc.ref = luaL_checkstring(L, 1);
	editc.type = 2;

	//check if reference is already in use
	for(unsigned i = 0; i < editComponents.size(); ++i)
	{
		if(editComponents[i].ref == editc.ref)
		{
			//reference already in use, dispatch error
			lua_pushstring(L, GD_LUA_COMP_ERROR_DUPLICATEIDS);
			return lua_error(L);
		}
	}

	//create checkbox
	ui::UIEdit_Number* c = new ui::UIEdit_Number(GetParentState());
	editc.c = c;

	//set text
	c->SetText(luaL_checkstring(L, 2));

	//optional boolean, auto-check
	if(lua_isnumber(L, 3))
		c->SetString( sys::StringHelper::ToStr(lua_tonumber(L, 3)).c_str() );

	if(lua_isnumber(L, 4))
	{
		c->min = lua_tonumber(L, 4);
		c->max = luaL_checknumber(L, 5);

		c->enable_minmax = true;
	}
	else
	{
		c->min = 0;
		c->max = 0;
		c->enable_minmax = false;
	}

	editComponents.push_back( editc );
	return 0;
}

// nil AddUIRadio(string unique_id, string text, string group_id [, bool checked = false])
int LuaComponent::AddUIRadio(lua_State* L)
{
	if(!this->allowNewEditComps) return 0;

	//reference radio button
	EditComponent editc;
	editc.ref = luaL_checkstring(L, 1);
	editc.type = 3;

	//check if reference is already in use
	for(int i = 0; i < editComponents.size(); ++i)
	{
		if(editComponents[i].ref == editc.ref)
		{
			//reference already in use, dispatch error
			lua_pushstring(L, GD_LUA_COMP_ERROR_DUPLICATEIDS);
			return lua_error(L);
		}
	}

	//get group_id
	std::string groupid = luaL_checkstring(L, 3);
	LastRadioItem* lastRadioItem_ref = NULL;
	for(unsigned i = 0; i < lastRadios.size(); ++i)
	{
		if(lastRadios[i].group == groupid)
		{
			lastRadioItem_ref = &lastRadios[i];
			break;
		}
	}
	if(!lastRadioItem_ref)
	{
		//create new group
		LastRadioItem item;
		item.group = groupid;
		item.last = NULL;
		lastRadios.push_back(item);

		lastRadioItem_ref = &lastRadios[lastRadios.size()-1];
	}

	//create radio
	ui::UIEdit_Radio* c = new ui::UIEdit_Radio(GetParentState(), groupid.c_str(), (ui::UIEdit_Radio*)lastRadioItem_ref->last);
	editc.c = c;
	lastRadioItem_ref->last = c;

	//set text
	c->SetText(luaL_checkstring(L, 2));

	//optional boolean, auto-check
	if(lua_isboolean(L, 4))
		c->SetCheck( lua_toboolean(L, 4)!=0 );

	editComponents.push_back( editc );
	return 0;
}

// nil AddUIString(string unique_id, string text [, bool multiline = false [, string default = "" [, number height_in_lines]]])
int LuaComponent::AddUIString(lua_State* L)
{
	if(!this->allowNewEditComps) return 0;

	//reference checkbox
	EditComponent editc;
	editc.ref = luaL_checkstring(L, 1);
	editc.type = 4;

	//check if reference is already in use
	for(unsigned i = 0; i < editComponents.size(); ++i)
	{
		if(editComponents[i].ref == editc.ref)
		{
			//reference already in use, dispatch error
			lua_pushstring(L, GD_LUA_COMP_ERROR_DUPLICATEIDS);
			return lua_error(L);
		}
	}

	bool multiline = false;
	if(lua_isboolean(L, 3))
		multiline = (lua_toboolean(L, 3)!=0);

	//create checkbox
	if(multiline)
	{
		ui::UIEdit_StringML* c = new ui::UIEdit_StringML(GetParentState());
		editc.c = c;

		editc.type = -4;

		//set text
		c->SetText(luaL_checkstring(L, 2));

		//optional default string
		if(lua_isstring(L, 4))
			c->SetString(lua_tostring(L, 4));

		//optional height_in_lines number for multiline
		if(lua_isnumber(L, 5))
			c->SetHeight(static_cast<int>(lua_tonumber(L, 5)));
	}
	else
	{
		ui::UIEdit_String* c = new ui::UIEdit_String(GetParentState());
		editc.c = c;

		//set text
		c->SetText(luaL_checkstring(L, 2));

		//optional default string
		if(lua_isstring(L, 4))
			c->SetString(lua_tostring(L, 4));
	}

	editComponents.push_back( editc );
	return 0;
}

// nil AddUIKey(string unique_id, string text [, number default = 0])
int LuaComponent::AddUIKey(lua_State* L)
{
	if(!this->allowNewEditComps) return 0;

	//reference checkbox
	EditComponent editc;
	editc.ref = luaL_checkstring(L, 1);
	editc.type = 5;

	//check if reference is already in use
	for(unsigned i = 0; i < editComponents.size(); ++i)
	{
		if(editComponents[i].ref == editc.ref)
		{
			//reference already in use, dispatch error
			lua_pushstring(L, GD_LUA_COMP_ERROR_DUPLICATEIDS);
			return lua_error(L);
		}
	}

	//create checkbox
	ui::UIEdit_Key* c = new ui::UIEdit_Key(GetParentState());
	editc.c = c;

	//set text
	c->SetText(luaL_checkstring(L, 2));

	//optional number, key
	if(lua_isnumber(L, 3))
		c->mykey = static_cast<int>(lua_tonumber(L, 3));

	editComponents.push_back( editc );
	return 0;
}

// nil AddUISeparator()
int LuaComponent::AddUISeparator(lua_State* L)
{
	if(!this->allowNewEditComps) return 0;

	EditComponent editc;
	editc.type = 0;

	//create separator
	ui::UIEdit_Separator* c = new ui::UIEdit_Separator(GetParentState());
	editc.c = c;

	editComponents.push_back( editc );
	return 0;
}

// nil AddUILabal(string text)
int LuaComponent::AddUILabel(lua_State* L)
{
	if(!this->allowNewEditComps) return 0;

	EditComponent editc;
	editc.type = 0;

	//create separator
	ui::UIEdit_Label* c = new ui::UIEdit_Label(GetParentState());
	editc.c = c;

	c->SetString(luaL_checkstring(L, 1));

	editComponents.push_back( editc );
	return 0;
}

#pragma endregion

/***************************************************************/
/***************************************************************/
/***************************************************************/

void LuaComponent::ConnectOutput(LuaComponent* opposingComp, int our_outport, int their_inport)
{
	//do a check if already connected
	for(unsigned i = 0, sz = this->OutputPorts[our_outport].opposing_list.size(); i < sz; ++i)
	{
		if( this->OutputPorts[our_outport].opposing_list[i].comp == opposingComp &&
			this->OutputPorts[our_outport].opposing_list[i].port == their_inport )
		{
			//already connected.
			return;
		}
	}

	//create the link
	Opposition opp;
	opp.comp = opposingComp;
	opp.port = their_inport;
	this->OutputPorts[our_outport].opposing_list.push_back(opp);
	//this->OutputPorts[our_outport]._outputtriggered = true; // we just connected, so we need to send our value
	opposingComp->InputPorts[their_inport].opposing.comp = this;
	opposingComp->InputPorts[their_inport].opposing.port = our_outport;
}

void LuaComponent::ConnectInput(LuaComponent* opposingComp, int our_inport, int their_outport)
{
	//do a check if already connected
	for(unsigned i = 0, sz = opposingComp->OutputPorts[their_outport].opposing_list.size(); i < sz; ++i)
	{
		if( opposingComp->OutputPorts[their_outport].opposing_list[i].comp == this &&
			opposingComp->OutputPorts[their_outport].opposing_list[i].port == our_inport )
		{
			//already connected
			return;
		}
	}

	//create the link
	this->InputPorts[our_inport].opposing.comp = opposingComp;
	this->InputPorts[our_inport].opposing.port = their_outport;
	//opposingComp->OutputPorts[their_outport]._outputtriggered = true; // we just connected, so we need to retrieve their data
	Opposition opp;
	opp.comp = this;
	opp.port = our_inport;
	opposingComp->OutputPorts[their_outport].opposing_list.push_back(opp);
}

void LuaComponent::UnconnectInput(int port)
{
	//find component this port is connected too
	Opposition& opp_opposition = this->InputPorts[port].opposing;
	if(opp_opposition.port == -1) return;
	OutPort& opp_outport = opp_opposition.comp->OutputPorts[opp_opposition.port];
	for(unsigned i = 0; i < opp_outport.opposing_list.size(); ++i)
	{
		if( opp_outport.opposing_list[i].comp == this && 
			opp_outport.opposing_list[i].port == port )
		{
			//unlink
			opp_outport.opposing_list.erase( opp_outport.opposing_list.begin() + i);
			opp_opposition.comp = NULL;
			opp_opposition.port = -1;

			//call onInputsTriggered
			this->_inputneedsupdating[port] = true;

			return;
		}
	}
}

void LuaComponent::UnconnectOutput(int port)
{
	//find component this port is connected to
	for(unsigned i = 0; i < this->OutputPorts[port].opposing_list.size(); ++i)
	{
		Opposition& opp_opposition = this->OutputPorts[port].opposing_list[i];
		InPort& opp_inport = opp_opposition.comp->InputPorts[opp_opposition.port];

		if( opp_inport.opposing.comp == this &&
			opp_inport.opposing.port == port )
		{
			//call onInputsTriggered
			opp_opposition.comp->_inputneedsupdating[opp_opposition.port] = true;

			//unlink
			opp_inport.opposing.comp = NULL;
			opp_inport.opposing.port = -1;
			this->OutputPorts[port].opposing_list.erase(this->OutputPorts[port].opposing_list.begin() + i);
			--i;

			//don't return. Outputs may have multiple links, so continue for any more
			//return;
		}
	}
}

void LuaComponent::luaLoadScript(const char* filename)
{
	sys::Dbg::Assert(L==NULL);

	//Create a new state
	L = luaL_newstate();

	//Load libraries to state
	luaL_openlibs(L);

	//Load pseudo-wire libraries
	Luna<LuaComponent>::Register(L);
	Luna<Color>::Register(L);
	Luna<Vector>::Register(L);
	Luna<Shape>::Register(L);
	Luna<Image>::Register(L);
	Luna<Sprite>::Register(L);
	Luna<Text>::Register(L);
	Luna<RenderTexture>::Register(L);
	luaOpenPseudoWireAPI(L);

	lua_pushnumber(L, myaddonitem->p_category->p_package->id);
	lua_setfield(L, LUA_REGISTRYINDEX, GD_LUA_REGISTRY_ADDONID);

	//Load script
	if( luaL_loadfile(L, filename) )
	{
		//an error occured...
		if(lua_isstring(L, -1)) //probably an error message
		{
			gamestate->Console->Print(ui::UIConsole::MSG_TERROR, "%s: %s\n", GD_LUA_ERROR_COMPILE, luaL_checklstring(L, -1, NULL));
			lua_pop(L, 1);

			error = true;
		}
	}

	//Run script
	if( lua_pcall(L, 0, LUA_MULTRET, 0) )
	{
		//an error occured...
		if(lua_isstring(L, -1)) //probably an error message
		{
			if(!error)
				gamestate->Console->Print(ui::UIConsole::MSG_TERROR, "%s: %s\n", GD_LUA_ERROR_INITRUN, luaL_checklstring(L, -1, NULL));
			
			lua_pop(L, 1);

			error = true;
		}
	}

	lua_pushlightuserdata(L, gamestate);
	lua_setfield(L, LUA_REGISTRYINDEX, GD_LUA_REGISTRY_GAMEPTR);

	//Create lua userdata for "self"
	Luna<LuaComponent>::pushobject(L, this);
	lua_setfield(L, LUA_REGISTRYINDEX, GD_LUA_REGISTRY_SELF);

	//Call onSpawn component event
	luaCallObjectFunction(GD_LUA_FNAME_ONSPAWN);
}

int LuaComponent::luaCallFunction(const char* func, int num_args, int num_results)
{
	if(L==NULL)
		return -1;

	//get the function
	lua_getfield(L, LUA_GLOBALSINDEX, func);

	//check if the function exists
	if(lua_type(L, -1) != LUA_TFUNCTION)
	{
		lua_pop(L, 1 + num_args);
		return -1;
	}

	//move the function below the pushed arguments
	lua_insert(L, -(num_args+1));

	//call the function
	if(lua_pcall(L, num_args, num_results, NULL))
	{
		//an error occured...
		if(lua_isstring(L, -1)) //probably an error message
		{
			gamestate->Console->Print(ui::UIConsole::MSG_TERROR, "%s: %s\n", GD_LUA_ERROR_RUN, luaL_checklstring(L, -1, NULL));
			lua_remove(L, -1);
		}
		return 1;
	}

	return 0;
}

int LuaComponent::luaCallObjectFunction(const char* func, int num_args, int num_results)
{
	if(L==NULL)
		return -1;

	//get self
	lua_getfield(L, LUA_REGISTRYINDEX, GD_LUA_REGISTRY_SELF);

	//get the function in self
	lua_getfield(L, -1, func);

	//check if the function exists
	if(lua_type(L, -1) != LUA_TFUNCTION)
	{
		lua_pop(L, num_args+2);
		return -1;
	}

	//move the function below the pushed arguments
	lua_insert(L, -(num_args+2));

	//push "this" as first argument (thus, "self" in lua)
	if(num_args) lua_insert(L, -(num_args+1));
	
	//call the function
	if(lua_pcall(L, num_args+1, num_results, NULL))
	{
		//an error occured...
		if(lua_isstring(L, -1)) //probably an error message
		{
			gamestate->Console->Print(ui::UIConsole::MSG_TERROR, "%s: %s\n", GD_LUA_ERROR_RUN, luaL_checklstring(L, -1, NULL));
			lua_pop(L, 1);
		}
		return 1;
	}

	return 0;
}
