#pragma once

#include "lua/lua.hpp"
#include "luna.h"
#include "Port.h"

#include "Renderer.h"
#include "GameLoader.h"

#include "GameState.h"
#include "UI/UIConsole.h"
#include "GameData.h"

#define LUA_FUNC(funcname) int funcname(lua_State* l)
#define LUA_FUNC_DEF(classname, funcname) int classname::funcname(lua_State* l)
#define REG_LUNA_FUNC(class, name) {#name, &class::name}

/***************************************************************/
/***************************************************************/
/***************************************************************/

namespace game
{

class Color;
class Vector;
class Shape;
class Image;
class Sprite;
class Text;
class RenderTexture;

#pragma region Lua Classes

class Color
{
public:
	sf::Color* myColor;
	Color(sf::Color* c);
	Color(lua_State* L);
	~Color();

	int R(lua_State* L);
	int G(lua_State* L);
	int B(lua_State* L);
	int A(lua_State* L);

	// registry
	static Luna<Color>::RegType registerMethods[];
	static const char className[];
};

class Vector
{
public:
	sf::Vector2f* myVec;
	Vector(sf::Vector2f* v);
	Vector(lua_State* L);
	~Vector();

	// number X(number x)
	// number X()
	int X(lua_State* L);
	// number Y(number y)
	// number Y()
	int Y(lua_State* L);

	// nil Set(number x, number y);
	int Set(lua_State* L);

	// nil Move(Vector offset)
	// nil Move(number offset_x, number offset_y);
	int Move(lua_State* L);

	// registry
	static Luna<Vector>::RegType registerMethods[];
	static const char className[];
};

class Shape
{
public:
	sf::Shape* myShape;
	Shape(sf::Shape* s);
	Shape(lua_State* L);
	~Shape();

	// nil AddPoint(Vector pos)
	int AddPoint(lua_State* L);
	// nil SetPointPos(number idx, Vector pos)
	int SetPointPos(lua_State* L);
	// Vector GetPointPos(number idx)
	int GetPointPos(lua_State* L);
	// nil SetPointColor(number idx, Color col)
	int SetPointColor(lua_State* L);
	// Color GetPointColor(number idx)
	int GetPointColor(lua_State* L);
	// nil SetOutlineWidth(number width)
	int SetOutlineWidth(lua_State* L);
	// number GetOutlineWidth()
	int GetOutlineWidth(lua_State* L);
	// nil SetPointOutlineColor(number idx, Color col)
	int SetPointOutlineColor(lua_State* L);
	// Color GetPointOutlineColor(number idx)
	int GetPointOutlineColor(lua_State* L);
	// number GetPointCount()
	int GetPointCount(lua_State* L);

	// nil SetPos(Vector pos)
	int SetPos(lua_State* L);
	// Vector GetPos()
	int GetPos(lua_State* L);

	// nil SetRot(number angle)
	int SetRot(lua_State* L);
	// number GetRot()
	int GetRot(lua_State* L);

	// nil SetScale(Vector scale)
	int SetScale(lua_State* L);
	// Vector GetScale()
	int GetScale(lua_State* L);

	// nil SetBlendMode(number mode)
	int SetBlendMode(lua_State* L);
	// number GetBlendMode()
	int GetBlendMode(lua_State* L);

	// nil SetColor(Color col)
	int SetColor(lua_State* L);
	// Color GetColor()
	int GetColor(lua_State* L);

	// nil SetOrigin(Vector center)
	int SetOrigin(lua_State* L);
	// Vector GetOrigin()
	int GetOrigin(lua_State* L);

	// nil Move(Vector offset)
	// nil Move(number offset_x, number offset_y);
	int Move(lua_State* L);
	
	int X(lua_State* L);
	int Y(lua_State* L);

	// registry
	static Luna<Shape>::RegType registerMethods[];
	static const char className[];
};

class Image
{
public:
	sf::Image* myImage;
	Image(sf::Image* i);
	Image(lua_State* L);
	~Image();

	// bool Load(string filename)
	int Load(lua_State* L);

	// number GetWidth()
	int GetWidth(lua_State* L);

	// number GetHeight()
	int GetHeight(lua_State* L);

	// registry
	static Luna<Image>::RegType registerMethods[];
	static const char className[];
};

class Sprite
{
public:
	sf::Sprite* mySprite;
	Sprite(sf::Sprite* s);
	Sprite(lua_State* L);
	~Sprite();

	// nil SetImage(Image img)
	int SetImage(lua_State* L);
	int GetImage(lua_State* L);

	// nil SetPos(Vector pos)
	int SetPos(lua_State* L);
	// Vector GetPos()
	int GetPos(lua_State* L);

	// nil SetRot(number angle)
	int SetRot(lua_State* L);
	// number GetRot()
	int GetRot(lua_State* L);

	// nil SetScale(Vector scale)
	int SetScale(lua_State* L);
	// Vector GetScale()
	int GetScale(lua_State* L);

	// nil SetBlendMode(number mode)
	int SetBlendMode(lua_State* L);
	// number GetBlendMode()
	int GetBlendMode(lua_State* L);

	// nil SetColor(Color col)
	int SetColor(lua_State* L);
	// Color GetColor()
	int GetColor(lua_State* L);

	// nil SetOrigin(Vector center)
	int SetOrigin(lua_State* L);
	// Vector GetOrigin()
	int GetOrigin(lua_State* L);

	// nil Move(Vector offset)
	// nil Move(number offset_x, number offset_y);
	int Move(lua_State* L);
	
	int X(lua_State* L);
	int Y(lua_State* L);

	// nil SetSubRect(number x, number y, number width, number height)
	int SetSubRect(lua_State* L);

	// registry
	static Luna<Sprite>::RegType registerMethods[];
	static const char className[];
};

class RenderTexture
{
public:
	sf::RenderImage* myRenderImage;
	sf::Sprite* mySprite;
	RenderTexture(sf::RenderImage* r);
	RenderTexture(lua_State* L);
	~RenderTexture();

	// boolean Create(Vector size)
	// boolean Create(number width, number height)
	int Create(lua_State* L);

	// nil Clear()
	// nil Clear(Color color)
	int Clear(lua_State* L);

	// nil Draw(Sprite sprite)
	// nil Draw(Text text)
	// nil Draw(Shape shape)
	// nil Draw(RenderTexture rt)
	int Draw(lua_State* L);

	// nil Display()
	int Display(lua_State* L);

	// nil SetPos(Vector pos)
	int SetPos(lua_State* L);
	// Vector GetPos()
	int GetPos(lua_State* L);

	// nil SetRot(number angle)
	int SetRot(lua_State* L);
	// number GetRot()
	int GetRot(lua_State* L);

	// nil SetScale(Vector scale)
	int SetScale(lua_State* L);
	// Vector GetScale()
	int GetScale(lua_State* L);

	// nil SetBlendMode(number mode)
	int SetBlendMode(lua_State* L);
	// number GetBlendMode()
	int GetBlendMode(lua_State* L);

	// nil SetColor(Color col)
	int SetColor(lua_State* L);
	// Color GetColor()
	int GetColor(lua_State* L);

	// nil SetOrigin(Vector center)
	int SetOrigin(lua_State* L);
	// Vector GetOrigin()
	int GetOrigin(lua_State* L);

	// number GetWidth()
	int GetWidth(lua_State* L);
	// number GetHeight()
	int GetHeight(lua_State* L);

	// nil Move(Vector offset)
	// nil Move(number offset_x, number offset_y);
	int Move(lua_State* L);
	
	int X(lua_State* L);
	int Y(lua_State* L);

	// nil SetSubRect(number x, number y, number width, number height)
	int SetSubRect(lua_State* L);

	// registry
	static Luna<RenderTexture>::RegType registerMethods[];
	static const char className[];
};

class Text
{
public:
	sf::Text* myText;
	Text(sf::Text* t);
	Text(lua_State* L);
	~Text();

	// number GetWidth/Height()
	int GetWidth(lua_State* L);
	int GetHeight(lua_State* L);

	// number GetSize()
	int GetSize(lua_State* L);
	// nil SetSize(number s)
	int SetSize(lua_State* L);

	// string GetText()
	int GetText(lua_State* L);
	// nil SetText(string text)
	int SetText(lua_State* L);

	// bool SetFont(string fontfilename)
	int SetFont(lua_State* L);
	
	// nil SetPos(Vector pos)
	int SetPos(lua_State* L);
	// Vector GetPos()
	int GetPos(lua_State* L);

	// Color GetColor()
	int GetColor(lua_State* L);
	// nil SetColor(Color col)
	int SetColor(lua_State* L);

	////////////////////////////////////////////////

	// nil SetOrigin(Vector center)
	int SetOrigin(lua_State* L);
	// Vector GetOrigin()
	int GetOrigin(lua_State* L);

	// nil SetScale(Vector scale)
	int SetScale(lua_State* L);
	// Vector GetScale()
	int GetScale(lua_State* L);

	// nil SetBlendMode(number mode)
	int SetBlendMode(lua_State* L);
	// number GetBlendMode()
	int GetBlendMode(lua_State* L);

	// nil SetRot(number angle)
	int SetRot(lua_State* L);
	// number GetRot()
	int GetRot(lua_State* L);

	// nil Move(Vector offset)
	// nil Move(number offset_x, number offset_y);
	int Move(lua_State* L);

	int X(lua_State* L);
	int Y(lua_State* L);

	// registry
	static Luna<Text>::RegType registerMethods[];
	static const char className[];
};

#pragma endregion

#pragma region Lua Game Functions

// game.create_rect(number x, number y, number width, number height, Color col [, number outline_width [, Color outline_col]])
static LUA_FUNC(create_rect)
{
	if(lua_isnumber(l, 6))
	{
		if(lua_isuserdata(l, 7))
		{
			Luna<Shape>::pushobject(l, new Shape(new sf::Shape(sf::Shape::Rectangle(
				sf::FloatRect(luaL_checknumberf(l, 1), luaL_checknumberf(l, 2), luaL_checknumberf(l, 3), luaL_checknumberf(l, 4)),
				*Luna<Color>::check(l, 5)->myColor,
				luaL_checknumberf(l, 6),
				*Luna<Color>::check(l, 7)->myColor
			))));
		}
		else
		{
			Luna<Shape>::pushobject(l, new Shape(new sf::Shape(sf::Shape::Rectangle(
				sf::FloatRect(luaL_checknumberf(l, 1), luaL_checknumberf(l, 2), luaL_checknumberf(l, 3), luaL_checknumberf(l, 4)),
				*Luna<Color>::check(l, 5)->myColor,
				luaL_checknumberf(l, 6)
			))));
		}
	}
	else
	{
		Luna<Shape>::pushobject(l, new Shape(new sf::Shape(sf::Shape::Rectangle(
				sf::FloatRect(luaL_checknumberf(l, 1), luaL_checknumberf(l, 2), luaL_checknumberf(l, 3), luaL_checknumberf(l, 4)),
				*Luna<Color>::check(l, 5)->myColor
			))));
	}
	return 1;
}

// game.create_circle(number x, number y, number radius, Color col [, number outline_width [, Color ouline_col]])
static LUA_FUNC(create_circle)
{
	if(lua_isnumber(l, 5))
	{
		if(lua_isuserdata(l, 6))
		{
			Luna<Shape>::pushobject(l, new Shape( new sf::Shape( sf::Shape::Circle(
				luaL_checknumberf(l, 1), luaL_checknumberf(l, 2),
				luaL_checknumberf(l, 3),
				*Luna<Color>::check(l, 4)->myColor,
				luaL_checknumberf(l, 5),
				*Luna<Color>::check(l, 6)->myColor
			))));
		}
		else
		{
			Luna<Shape>::pushobject(l, new Shape( new sf::Shape( sf::Shape::Circle(
				luaL_checknumberf(l, 1), luaL_checknumberf(l, 2),
				luaL_checknumberf(l, 3),
				*Luna<Color>::check(l, 4)->myColor,
				luaL_checknumberf(l, 5)
			))));
		}
	}
	else
	{
		Luna<Shape>::pushobject(l, new Shape( new sf::Shape( sf::Shape::Circle(
			luaL_checknumberf(l, 1), luaL_checknumberf(l, 2),
			luaL_checknumberf(l, 3),
			*Luna<Color>::check(l, 4)->myColor
		))));
	}
	return 1;
}

// game.create_line(number x, number y, number x2, number y2, number thickness, Color col [, number outline_width [, Color ouline_col]])
static LUA_FUNC(create_line)
{
	if(lua_isnumber(l, 7))
	{
		if(lua_isuserdata(l, 8))
		{
			Luna<Shape>::pushobject(l, new Shape( new sf::Shape( sf::Shape::Line(
				luaL_checknumberf(l, 1), luaL_checknumberf(l, 2), luaL_checknumberf(l, 3), luaL_checknumberf(l, 4),
				luaL_checknumberf(l, 5),
				*Luna<Color>::check(l, 6)->myColor,
				luaL_checknumberf(l, 7),
				*Luna<Color>::check(l, 8)->myColor
			))));
		}
		else
		{
			Luna<Shape>::pushobject(l, new Shape( new sf::Shape( sf::Shape::Line(
				luaL_checknumberf(l, 1), luaL_checknumberf(l, 2), luaL_checknumberf(l, 3), luaL_checknumberf(l, 4),
				luaL_checknumberf(l, 5),
				*Luna<Color>::check(l, 6)->myColor,
				luaL_checknumberf(l, 7)
			))));
		}
	}
	else
	{
		Luna<Shape>::pushobject(l, new Shape( new sf::Shape( sf::Shape::Line(
			luaL_checknumberf(l, 1), luaL_checknumberf(l, 2), luaL_checknumberf(l, 3), luaL_checknumberf(l, 4),
			luaL_checknumberf(l, 5),
			*Luna<Color>::check(l, 6)->myColor
		))));
	}
	return 1;
}

//Mouse position on window
// number game.mouse_x()
static LUA_FUNC(mouse_x)
{
	lua_pushnumber(l, game::Renderer::Ref()->GetInput().GetMouseX());
	return 1;
}
// number game.mouse_y()
static LUA_FUNC(mouse_y)
{
	lua_pushnumber(l, game::Renderer::Ref()->GetInput().GetMouseY());
	return 1;
}

//Window size
// number game.scr_w()
static LUA_FUNC(scr_w)
{
	lua_pushnumber(l, game::Renderer::Ref()->GetWidth());
	return 1;
}
// number game.scr_h()
static LUA_FUNC(scr_h)
{
	lua_pushnumber(l, game::Renderer::Ref()->GetHeight());
	return 1;
}

//Addon Info
// string game.addon_name()
static LUA_FUNC(addon_name)
{
	lua_getfield(l, LUA_REGISTRYINDEX, GD_LUA_REGISTRY_ADDONID);
	int id = static_cast<int>(lua_tonumber(l, -1));
	lua_pop(l, 1);
	lua_pushstring(l, AddonReader::Ref().Packages[id]->name.c_str());
	return 1;
}

// string game.addon_author()
static LUA_FUNC(addon_author)
{
	lua_getfield(l, LUA_REGISTRYINDEX, GD_LUA_REGISTRY_ADDONID);
	int id = static_cast<int>(lua_tonumber(l, -1));
	lua_pop(l, 1);
	lua_pushstring(l, AddonReader::Ref().Packages[id]->author.c_str());
	return 1;
}

// string game.addon_version()
static LUA_FUNC(addon_version)
{
	lua_getfield(l, LUA_REGISTRYINDEX, GD_LUA_REGISTRY_ADDONID);
	int id = static_cast<int>(lua_tonumber(l, -1));
	lua_pop(l, 1);
	lua_pushstring(l, AddonReader::Ref().Packages[id]->version.c_str());
	return 1;
}

// number game.tick()
static LUA_FUNC(tick)
{
	lua_getfield(l, LUA_REGISTRYINDEX, GD_LUA_REGISTRY_GAMEPTR);
	GameState* game = (GameState*)lua_touserdata(l, -1);
	lua_pop(l, 1);

	lua_pushinteger(l, game->ITER);
	return 1;
}

//FUNCTION REGISTRY
static const luaL_reg gamelib[] =
{
	{ "create_rect", create_rect },
	{ "create_circle", create_circle },
	{ "create_line", create_line },

	{ "mouse_x", mouse_x },
	{ "mouse_y", mouse_y },

	{ "scr_w", scr_w },
	{ "scr_h", scr_h },

	{ "tick", tick },

	//must end with:
	{ NULL, NULL }
};
#pragma endregion

/***************************************************************/
/***************************************************************/
/***************************************************************/

#pragma region Lua Helper Functions

//Bitwise Operators
static LUA_FUNC(BIT_LSHIFT)
{
	lua_pushinteger(l, luaL_checkint(l, 1) << luaL_checkint(l, 2));
	return 1;
}
static LUA_FUNC(BIT_RSHIFT)
{
	lua_pushinteger(l, luaL_checkint(l, 1) >> luaL_checkint(l, 2));
	return 1;
}
static LUA_FUNC(BIT_AND)
{
	lua_pushinteger(l, luaL_checkint(l, 1) & luaL_checkint(l, 2));
	return 1;
}
static LUA_FUNC(BIT_OR)
{
	lua_pushinteger(l, luaL_checkint(l, 1) | luaL_checkint(l, 2));
	return 1;
}
static LUA_FUNC(BIT_XOR)
{
	lua_pushinteger(l, luaL_checkint(l, 1) ^ luaL_checkint(l, 2));
	return 1;
}
static LUA_FUNC(BIT_NOT)
{
	lua_pushinteger(l, ~luaL_checkint(l, 1));
	return 1;
}

//ifelse ?: trinary operator
static LUA_FUNC(IFELSE)
{
	if(lua_isboolean(l, 1))
	{
		if(lua_toboolean(l, 1))
			lua_pushvalue(l, 2);
		else
			lua_pushvalue(l, 3);
	}
	else if(lua_isnumber(l, 1))
	{
		if(lua_tointeger(l, 1))
			lua_pushvalue(l, 2);
		else
			lua_pushvalue(l, 3);
	}
	else
	{
		lua_pushvalue(l, 3);
	}

	return 1;
}

//bool Is*(val)
static LUA_FUNC(IsNil)
{
	lua_pushboolean(l, lua_isnil(l,1));
	return 1;
}
static LUA_FUNC(IsNumber)
{
	lua_pushboolean(l, lua_isnumber(l, 1));
	return 1;
}
static LUA_FUNC(IsString)
{
	lua_pushboolean(l, lua_isstring(l, 1));
	return 1;
}
static LUA_FUNC(IsBoolean)
{
	lua_pushboolean(l, lua_isboolean(l, 1));
	return 1;
}
static LUA_FUNC(IsFunction)
{
	lua_pushboolean(l, lua_isfunction(l, 1));
	return 1;
}
static LUA_FUNC(IsTable)
{
	lua_pushboolean(l, lua_istable(l, 1));
	return 1;
}
static LUA_FUNC(IsUserdata)
{
	lua_pushboolean(l, lua_isuserdata(l, 1));
	return 1;
}

static const char* KeycodeToString_C(int code)
{
	switch(code)
	{
	default: return GD_NULLSTR;

	case sf::Key::A: return "A";
	case sf::Key::B: return "B";
	case sf::Key::C: return "C";
	case sf::Key::D: return "D";
	case sf::Key::E: return "E";
	case sf::Key::F: return "F";
	case sf::Key::G: return "G";
	case sf::Key::H: return "H";
	case sf::Key::I: return "I";
	case sf::Key::J: return "J";
	case sf::Key::K: return "K";
	case sf::Key::L: return "L";
	case sf::Key::M: return "M";
	case sf::Key::N: return "N";
	case sf::Key::O: return "O";
	case sf::Key::P: return "P";
	case sf::Key::Q: return "Q";
	case sf::Key::R: return "R";
	case sf::Key::S: return "S";
	case sf::Key::T: return "T";
	case sf::Key::U: return "U";
	case sf::Key::V: return "V";
	case sf::Key::W: return "W";
	case sf::Key::X: return "X";
	case sf::Key::Y: return "Y";
	case sf::Key::Z: return "Z";

	case sf::Key::Num0: return "0";
	case sf::Key::Num1: return "1";
	case sf::Key::Num2: return "2";
	case sf::Key::Num3: return "3";
	case sf::Key::Num4: return "4";
	case sf::Key::Num5: return "5";
	case sf::Key::Num6: return "6";
	case sf::Key::Num7: return "7";
	case sf::Key::Num8: return "8";
	case sf::Key::Num9: return "9";

	case sf::Key::Numpad0: return "Numpad 0";
	case sf::Key::Numpad1: return "Numpad 1";
	case sf::Key::Numpad2: return "Numpad 2";
	case sf::Key::Numpad3: return "Numpad 3";
	case sf::Key::Numpad4: return "Numpad 4";
	case sf::Key::Numpad5: return "Numpad 5";
	case sf::Key::Numpad6: return "Numpad 6";
	case sf::Key::Numpad7: return "Numpad 7";
	case sf::Key::Numpad8: return "Numpad 8";
	case sf::Key::Numpad9: return "Numpad 9";

	case sf::Key::F1: return "F1";
	case sf::Key::F2: return "F2";
	case sf::Key::F3: return "F3";
	case sf::Key::F4: return "F4";
	case sf::Key::F5: return "F5";
	case sf::Key::F6: return "F6";
	case sf::Key::F7: return "F7";
	case sf::Key::F8: return "F8";
	case sf::Key::F9: return "F9";
	case sf::Key::F10: return "F10";
	case sf::Key::F11: return "F11";
	case sf::Key::F12: return "F12";
	case sf::Key::F13: return "F13";
	case sf::Key::F14: return "F14";
	case sf::Key::F15: return "F15";

	case sf::Key::Add: return "Add";
	case sf::Key::Back: return "Backspace";
	case sf::Key::BackSlash: return "\\";
	case sf::Key::Comma: return ",";
	case sf::Key::Dash: return "-";
	case sf::Key::Delete: return "Delete";
	case sf::Key::Divide: return "Divide";
	case sf::Key::Down: return "Down";
	case sf::Key::End: return "End";
	case sf::Key::Equal: return "=";
	case sf::Key::Escape: return "Escape";
	case sf::Key::Home: return "Home";
	case sf::Key::Insert: return "Insert";
	case sf::Key::LAlt: return "Alt (left)";
	case sf::Key::LBracket: return "[";
	case sf::Key::LControl: return "Control (left)";
	case sf::Key::Left: return "Left";
	case sf::Key::LShift: return "Shift (Left)";
	case sf::Key::LSystem: return "System (Left)";
	case sf::Key::Menu: return "Menu";
	case sf::Key::Multiply: return "Multiply";
	case sf::Key::PageDown: return "Page Down";
	case sf::Key::PageUp: return "Page Up";
	case sf::Key::Pause: return "Pause";
	case sf::Key::Period: return ".";
	case sf::Key::Quote: return "'";
	case sf::Key::RAlt: return "Alt (Right)";
	case sf::Key::RBracket: return "]";
	case sf::Key::RControl: return "Control (Right)";
	case sf::Key::Return: return "Return";
	case sf::Key::Right: return "Right";
	case sf::Key::RShift: return "Shift (Right)";
	case sf::Key::RSystem: return "System (Right)";
	case sf::Key::SemiColon: return ";";
	case sf::Key::Slash: return "/";
	case sf::Key::Space: return "Space";
	case sf::Key::Subtract: return "Subtract";
	case sf::Key::Tab: return "Tab";
	case sf::Key::Tilde: return "~";
	case sf::Key::Up: return "Up";
	}
}

static LUA_FUNC(KeycodeToString)
{
	lua_pushstring(l, KeycodeToString_C(luaL_checkint(l, 1) ));
	return 1;
}

static LUA_FUNC(Msg)
{
	lua_getfield(l, LUA_REGISTRYINDEX, GD_LUA_REGISTRY_GAMEPTR);
	GameState* game = (GameState*)lua_touserdata(l, -1);
	lua_pop(l, 1);

	game->Console->Print(ui::UIConsole::MSG_TNORMAL, luaL_checkstring(l, 1));

	return 0;
}

static LUA_FUNC(C_IsControl)
{
	lua_pushboolean(l, iscntrl(luaL_checkint(l, 1)));
	return 1;
}

static LUA_FUNC(C_IsGraphical)
{
	lua_pushboolean(l, isgraph(luaL_checkint(l, 1)));
	return 1;
}

static LUA_FUNC(C_IsPrint)
{
	lua_pushboolean(l, isprint(luaL_checkint(l, 1)));
	return 1;
}

static LUA_FUNC(C_IsDigit)
{
	lua_pushboolean(l, isdigit(luaL_checkint(l, 1)));
	return 1;
}

static LUA_FUNC(C_IsPunct)
{
	lua_pushboolean(l, ispunct(luaL_checkint(l, 1)));
	return 1;
}

static LUA_FUNC(C_IsAlpha)
{
	lua_pushboolean(l, isalpha(luaL_checkint(l, 1)));
	return 1;
}

static LUA_FUNC(C_IsAlphaNum)
{
	lua_pushboolean(l, isalnum(luaL_checkint(l, 1)));
	return 1;
}

static LUA_FUNC(C_IsXDigit)
{
	lua_pushboolean(l, isxdigit(luaL_checkint(l, 1)));
	return 1;
}

static LUA_FUNC(C_IsUpper)
{
	lua_pushboolean(l, isupper(luaL_checkint(l, 1)));
	return 1;
}

static LUA_FUNC(C_IsLower)
{
	lua_pushboolean(l, islower(luaL_checkint(l, 1)));
	return 1;
}


//FUNCTION REGISTRY
static const luaL_reg standalonelib[] =
{
	//{ "DBG_STACKDUMP", lua_stackdump },
	{ "BIT_LSHIFT", BIT_LSHIFT },
	{ "BIT_RSHIFT", BIT_RSHIFT },
	{ "BIT_AND", BIT_AND },
	{ "BIT_OR", BIT_OR },
	{ "BIT_XOR", BIT_XOR },
	{ "BIT_NOT", BIT_NOT },

	{ "IFELSE", IFELSE },

	{ "IsNil", IsNil },
	{ "IsNumber", IsNumber },
	{ "IsString", IsString },
	{ "IsBoolean", IsBoolean },
	{ "IsFunction", IsFunction },
	{ "IsTable", IsTable },
	{ "IsUserdata", IsUserdata },

	{ "CharIsControl", C_IsControl },
	{ "CharIsGraphical", C_IsGraphical },
	{ "CharIsPrintable", C_IsPrint },
	{ "CharIsDigit", C_IsDigit },
	{ "CharIsPunctuation", C_IsPunct },
	{ "CharIsAlphabetical", C_IsAlpha },
	{ "CharIsAlphaNum", C_IsAlphaNum },
	{ "CharIsXDigit", C_IsXDigit },
	{ "CharIsUppercase", C_IsUpper },
	{ "CharIsLowercase", C_IsLower },

	{ "KeycodeToString", KeycodeToString },

	{ "Msg", Msg },

	//must end with:
	{ NULL, NULL }
};
#pragma endregion

/***************************************************************/
/***************************************************************/
/***************************************************************/

enum MOUSEBUTTON
{
	MOUSEBUTTON_LEFT = 0,
	MOUSEBUTTON_RIGHT,
	MOUSEBUTTON_MIDDLE,
	MOUSEBUTTON_X1,
	MOUSEBUTTON_X2,

	NUM_MOUSEBUTTONS
};

static void luaOpenPseudoWireAPI(lua_State* lua)
{
	//standalone library
	luaL_register(lua, GD_LUA_LIB_GAME, gamelib);
	for(int i = 0; standalonelib[i].func; ++i)
	{
		lua_register(lua, standalonelib[i].name, standalonelib[i].func);
	}
	lua_pop(lua, 1);

	// **** GLOBALS ****

	// Types

	lua_pushnumber(lua, TYPE_NUMBER); lua_setglobal(lua, GD_LUA_GLOBALS_TYPE_NUMBER);
	lua_pushnumber(lua, TYPE_STRING); lua_setglobal(lua, GD_LUA_GLOBALS_TYPE_STRING);
	lua_pushnumber(lua, TYPE_ANY); lua_setglobal(lua, GD_LUA_GLOBALS_TYPE_ANY);

	// Blend modes

	lua_pushnumber(lua, sf::Blend::None); lua_setglobal(lua, GD_LUA_GLOBALS_BLEND_NONE);
	lua_pushnumber(lua, sf::Blend::Alpha); lua_setglobal(lua, GD_LUA_GLOBALS_BLEND_ALPHA);
	lua_pushnumber(lua, sf::Blend::Add); lua_setglobal(lua, GD_LUA_GLOBALS_BLEND_ADD);
	lua_pushnumber(lua, sf::Blend::Multiply); lua_setglobal(lua, GD_LUA_GLOBALS_BLEND_MULTIPLY);

	// Mouse Buttons

	lua_pushnumber(lua, MOUSEBUTTON_LEFT); lua_setglobal(lua, GD_LUA_GLOBALS_MOUSEBUTTON_LEFT);
	lua_pushnumber(lua, MOUSEBUTTON_RIGHT); lua_setglobal(lua, GD_LUA_GLOBALS_MOUSEBUTTON_RIGHT);
	lua_pushnumber(lua, MOUSEBUTTON_MIDDLE); lua_setglobal(lua, GD_LUA_GLOBALS_MOUSEBUTTON_MIDDLE);
	lua_pushnumber(lua, MOUSEBUTTON_X1); lua_setglobal(lua, GD_LUA_GLOBALS_MOUSEBUTTON_X1);
	lua_pushnumber(lua, MOUSEBUTTON_X2); lua_setglobal(lua, GD_LUA_GLOBALS_MOUSEBUTTON_X2);

	// Keys
	
	lua_pushnumber(lua, sf::Key::A); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_A);
	lua_pushnumber(lua, sf::Key::B); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_B);
	lua_pushnumber(lua, sf::Key::C); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_C);
	lua_pushnumber(lua, sf::Key::D); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_D);
	lua_pushnumber(lua, sf::Key::E); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_E);
	lua_pushnumber(lua, sf::Key::F); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F);
	lua_pushnumber(lua, sf::Key::G); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_G);
	lua_pushnumber(lua, sf::Key::H); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_H);
	lua_pushnumber(lua, sf::Key::I); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_I);
	lua_pushnumber(lua, sf::Key::J); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_J);
	lua_pushnumber(lua, sf::Key::K); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_K);
	lua_pushnumber(lua, sf::Key::L); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_L);
	lua_pushnumber(lua, sf::Key::M); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_M);
	lua_pushnumber(lua, sf::Key::N); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_N);
	lua_pushnumber(lua, sf::Key::O); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_O);
	lua_pushnumber(lua, sf::Key::P); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_P);
	lua_pushnumber(lua, sf::Key::Q); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_Q);
	lua_pushnumber(lua, sf::Key::R); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_R);
	lua_pushnumber(lua, sf::Key::S); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_S);
	lua_pushnumber(lua, sf::Key::T); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_T);
	lua_pushnumber(lua, sf::Key::U); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_U);
	lua_pushnumber(lua, sf::Key::V); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_V);
	lua_pushnumber(lua, sf::Key::W); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_W);
	lua_pushnumber(lua, sf::Key::X); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_X);
	lua_pushnumber(lua, sf::Key::Y); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_Y);
	lua_pushnumber(lua, sf::Key::Z); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_Z);
	lua_pushnumber(lua, sf::Key::Num0); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_0);
	lua_pushnumber(lua, sf::Key::Num1); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_1);
	lua_pushnumber(lua, sf::Key::Num2); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_2);
	lua_pushnumber(lua, sf::Key::Num3); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_3);
	lua_pushnumber(lua, sf::Key::Num4); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_4);
	lua_pushnumber(lua, sf::Key::Num5); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_5);
	lua_pushnumber(lua, sf::Key::Num6); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_6);
	lua_pushnumber(lua, sf::Key::Num7); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_7);
	lua_pushnumber(lua, sf::Key::Num8); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_8);
	lua_pushnumber(lua, sf::Key::Num9); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_9);
	lua_pushnumber(lua, sf::Key::Numpad0); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_NUMPAD0);
	lua_pushnumber(lua, sf::Key::Numpad1); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_NUMPAD1);
	lua_pushnumber(lua, sf::Key::Numpad2); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_NUMPAD2);
	lua_pushnumber(lua, sf::Key::Numpad3); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_NUMPAD3);
	lua_pushnumber(lua, sf::Key::Numpad4); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_NUMPAD4);
	lua_pushnumber(lua, sf::Key::Numpad5); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_NUMPAD5);
	lua_pushnumber(lua, sf::Key::Numpad6); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_NUMPAD6);
	lua_pushnumber(lua, sf::Key::Numpad7); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_NUMPAD7);
	lua_pushnumber(lua, sf::Key::Numpad8); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_NUMPAD8);
	lua_pushnumber(lua, sf::Key::Numpad9); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_NUMPAD9);
	lua_pushnumber(lua, sf::Key::F1); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F1);
	lua_pushnumber(lua, sf::Key::F2); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F2);
	lua_pushnumber(lua, sf::Key::F3); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F3);
	lua_pushnumber(lua, sf::Key::F4); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F4);
	lua_pushnumber(lua, sf::Key::F5); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F5);
	lua_pushnumber(lua, sf::Key::F6); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F6);
	lua_pushnumber(lua, sf::Key::F7); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F7);
	lua_pushnumber(lua, sf::Key::F8); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F8);
	lua_pushnumber(lua, sf::Key::F9); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F9);
	lua_pushnumber(lua, sf::Key::F10); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F10);
	lua_pushnumber(lua, sf::Key::F11); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F11);
	lua_pushnumber(lua, sf::Key::F12); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F12);
	lua_pushnumber(lua, sf::Key::F13); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F13);
	lua_pushnumber(lua, sf::Key::F14); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F14);
	lua_pushnumber(lua, sf::Key::F15); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_F15);
	lua_pushnumber(lua, sf::Key::Escape); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_ESCAPE);
	lua_pushnumber(lua, sf::Key::LControl); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_LCONTROL);
	lua_pushnumber(lua, sf::Key::LShift); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_LSHIFT);
	lua_pushnumber(lua, sf::Key::LAlt); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_LALT);
	lua_pushnumber(lua, sf::Key::LSystem); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_LSYSTEM);
	lua_pushnumber(lua, sf::Key::RControl); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_RCONTROL);
	lua_pushnumber(lua, sf::Key::RShift); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_RSHIFT);
	lua_pushnumber(lua, sf::Key::RAlt); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_RALT);
	lua_pushnumber(lua, sf::Key::RSystem); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_RSYSTEM);
	lua_pushnumber(lua, sf::Key::Menu); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_MENU);
	lua_pushnumber(lua, sf::Key::LBracket); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_LBRACKET);
	lua_pushnumber(lua, sf::Key::RBracket); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_RBRACKET);
	lua_pushnumber(lua, sf::Key::SemiColon); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_SEMICOLON);
	lua_pushnumber(lua, sf::Key::Comma); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_COMMA);
	lua_pushnumber(lua, sf::Key::Period); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_PERIOD);
	lua_pushnumber(lua, sf::Key::Quote); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_QUOTE);
	lua_pushnumber(lua, sf::Key::Slash); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_FRONTSLASH);
	lua_pushnumber(lua, sf::Key::BackSlash); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_BACKSLASH);
	lua_pushnumber(lua, sf::Key::Tilde); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_TILDE);
	lua_pushnumber(lua, sf::Key::Equal); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_EQUAL);
	lua_pushnumber(lua, sf::Key::Dash); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_DASH);
	lua_pushnumber(lua, sf::Key::Space); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_SPACE);
	lua_pushnumber(lua, sf::Key::Return); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_RETURN);
	lua_pushnumber(lua, sf::Key::Back); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_BACKSPACE);
	lua_pushnumber(lua, sf::Key::Tab); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_TAB);
	lua_pushnumber(lua, sf::Key::PageUp); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_PAGEUP);
	lua_pushnumber(lua, sf::Key::PageDown); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_PAGEDOWN);
	lua_pushnumber(lua, sf::Key::End); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_END);
	lua_pushnumber(lua, sf::Key::Home); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_HOME);
	lua_pushnumber(lua, sf::Key::Insert); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_INSERT);
	lua_pushnumber(lua, sf::Key::Delete); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_DELETE);
	lua_pushnumber(lua, sf::Key::Add); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_ADD);
	lua_pushnumber(lua, sf::Key::Subtract); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_SUBTRACT);
	lua_pushnumber(lua, sf::Key::Multiply); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_MULTIPLY);
	lua_pushnumber(lua, sf::Key::Divide); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_DIVIDE);
	lua_pushnumber(lua, sf::Key::Left); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_LEFT);
	lua_pushnumber(lua, sf::Key::Right); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_RIGHT);
	lua_pushnumber(lua, sf::Key::Up); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_UP);
	lua_pushnumber(lua, sf::Key::Down); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_DOWN);
	lua_pushnumber(lua, sf::Key::Pause); lua_setglobal(lua, GD_LUA_GLOBALS_KEY_PAUSE);

}

}
