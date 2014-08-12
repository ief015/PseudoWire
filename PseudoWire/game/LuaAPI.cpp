#include "LuaAPI.h"
#include "GameData.h"

#include "luna.h"
#include "lua/lua.hpp"

#include "Port.h"
#include "GameLoader.h"

using namespace game;

////////////////////////////////////////////////////////////
// Color
////////////////////////////////////////////////////////////

Color::Color(lua_State* L)
{
	myColor = new sf::Color();

	if(lua_isnumber(L, 1))
	{
		myColor->r = static_cast<unsigned char>(lua_tointeger(L, 1));
	}
	else
	{
		myColor->r = 0;
	}

	if(lua_isnumber(L, 2))
	{
		myColor->g = static_cast<unsigned char>(lua_tointeger(L, 2));
	}
	else
	{
		myColor->g = 0;
	}

	if(lua_isnumber(L, 3))
	{
		myColor->b = static_cast<unsigned char>(lua_tointeger(L, 3));
	}
	else
	{
		myColor->b = 0;
	}

	if(lua_isnumber(L, 4))
	{
		myColor->a = static_cast<unsigned char>(lua_tointeger(L, 4));
	}
	else
	{
		myColor->a = 255;
	}
}
Color::Color(sf::Color* c)
: myColor(c)
{
}
Color::~Color()
{
	delete myColor;
}
int Color::R(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		//set
		myColor->r = static_cast<unsigned char>(lua_tointeger(L, 1));
		lua_pushinteger(L, myColor->r);
	}
	else
	{
		//get
		lua_pushinteger(L, myColor->r);
	}
	return 1;
}
int Color::G(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		//set
		myColor->g = static_cast<unsigned char>(lua_tointeger(L, 1));
		lua_pushinteger(L, myColor->g);
	}
	else
	{
		//get
		lua_pushinteger(L, myColor->g);
	}
	return 1;
}
int Color::B(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		//set
		myColor->b = static_cast<unsigned char>(lua_tointeger(L, 1));
		lua_pushinteger(L, myColor->b);
	}
	else
	{
		//get
		lua_pushinteger(L, myColor->b);
	}
	return 1;
}
int Color::A(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		//set
		myColor->a = static_cast<unsigned char>(lua_tointeger(L, 1));
		lua_pushinteger(L, myColor->a);
	}
	else
	{
		//get
		lua_pushinteger(L, myColor->a);
	}
	return 1;
}

const char Color::className[] = "Color";
Luna<Color>::RegType Color::registerMethods[] =
{
	{"R", &Color::R},
	{"G", &Color::G},
	{"B", &Color::B},
	{"A", &Color::A},
	{NULL,NULL}
};


////////////////////////////////////////////////////////////
// Vector
////////////////////////////////////////////////////////////

Vector::Vector(lua_State* L)
{
	myVec = new sf::Vector2f();
	myVec->x = luaL_checknumberf(L, 1);
	myVec->y = luaL_checknumberf(L, 2);
}
Vector::Vector(sf::Vector2f* v)
: myVec(v)
{
}
Vector::~Vector()
{
	delete myVec;
}
int Vector::X(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		//set
		myVec->x = lua_tonumberf(L, 1);
		lua_pushnumber(L, myVec->x);
	}
	else
	{
		//get
		lua_pushnumber(L, myVec->x);
	}
	return 1;
}
int Vector::Y(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		//set
		myVec->y = lua_tonumberf(L, 1);
		lua_pushnumber(L, myVec->y);
	}
	else
	{
		//get
		lua_pushnumber(L, myVec->y);
	}
	return 1;
}
int Vector::Set(lua_State* L)
{
	myVec->x = luaL_checknumberf(L, 1);
	myVec->y = luaL_checknumberf(L, 2);
	return 0;
}
int Vector::Move(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		// num args
		myVec->x += luaL_checknumberf(L, 1);
		myVec->y += luaL_checknumberf(L, 2);
	}
	else
	{
		// vec args
		sf::Vector2f& vec = * Luna<Vector>::check(L, 1)->myVec;
		myVec->x += vec.x;
		myVec->y += vec.y;
	}
	
	return 0;
}
const char Vector::className[] = "Vector";
Luna<Vector>::RegType Vector::registerMethods[] =
{
	{"X", &Vector::X},
	{"Y", &Vector::Y},
	{"Set", &Vector::Set},
	{"Move", &Vector::Move},
	{NULL, NULL}
};


////////////////////////////////////////////////////////////
// Shape
////////////////////////////////////////////////////////////

Shape::Shape(lua_State* L)
{
	myShape = new sf::Shape();
}
Shape::Shape(sf::Shape* s)
: myShape(s)
{
}
Shape::~Shape()
{
	delete myShape;
}
int Shape::AddPoint(lua_State* L)
{
	myShape->AddPoint(*Luna<Vector>::check(L, 1)->myVec);
	return 0;
}
int Shape::GetBlendMode(lua_State* L)
{
	lua_pushinteger(L, myShape->GetBlendMode());
	return 1;
}
int Shape::GetColor(lua_State* L)
{
	Luna<Color>::pushobject(L, new Color(new sf::Color(myShape->GetColor())));
	return 1;
}
int Shape::GetOrigin(lua_State* L)
{
	Luna<Vector>::pushobject(L, new Vector(new sf::Vector2f(myShape->GetOrigin())));
	return 1;
}
int Shape::GetOutlineWidth(lua_State* L)
{
	lua_pushnumber(L, myShape->GetOutlineWidth());
	return 1;
}
int Shape::GetPointColor(lua_State* L)
{
	Luna<Color>::pushobject(L, new Color(new sf::Color(myShape->GetPointColor(luaL_checkint(L, 1)))));
	return 1;
}
int Shape::GetPointCount(lua_State* L)
{
	lua_pushinteger(L, myShape->GetPointsCount());
	return 1;
}
int Shape::GetPointOutlineColor(lua_State* L)
{
	Luna<Color>::pushobject(L, new Color(new sf::Color(myShape->GetPointOutlineColor(luaL_checkint(L, 1)))));
	return 1;
}
int Shape::GetPointPos(lua_State* L)
{
	Luna<Vector>::pushobject(L, new Vector(new sf::Vector2f(myShape->GetPointPosition(luaL_checkint(L, 1)))));
	return 1;
}
int Shape::GetPos(lua_State* L)
{
	Luna<Vector>::pushobject(L, new Vector(new sf::Vector2f(myShape->GetPosition())));
	return 1;
}
int Shape::GetRot(lua_State* L)
{
	lua_pushnumber(L, myShape->GetRotation());
	return 1;
}
int Shape::GetScale(lua_State* L)
{
	Luna<Vector>::pushobject(L, new Vector(new sf::Vector2f(myShape->GetScale())));
	return 1;
}
int Shape::Move(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		// num args
		myShape->Move(lua_tonumberf(L, 1), lua_tonumberf(L, 2));
	}
	else
	{
		// vec args
		myShape->Move(* Luna<Vector>::check(L, 1)->myVec);
	}
	return 0;
}
int Shape::X(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		//set
		myShape->SetX( lua_tonumberf(L, 1) );
		lua_pushnumber(L, myShape->GetPosition().x);
		return 1;
	}
	else
	{
		//get
		lua_pushnumber(L, myShape->GetPosition().x);
		return 1;
	}
}
int Shape::Y(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		//set
		myShape->SetY( lua_tonumberf(L, 1) );
		lua_pushnumber(L, myShape->GetPosition().y);
		return 1;
	}
	else
	{
		//get
		lua_pushnumber(L, myShape->GetPosition().y);
		return 1;
	}
}
int Shape::SetBlendMode(lua_State* L)
{
	myShape->SetBlendMode(static_cast<sf::Blend::Mode>(luaL_checkint(L, 1)));
	return 0;
}
int Shape::SetColor(lua_State* L)
{
	Color* col = Luna<Color>::check(L, 1);
	for(unsigned i = 0; i < myShape->GetPointsCount(); ++i)
		myShape->SetPointColor(i, *col->myColor);
	myShape->SetColor(*col->myColor);
	return 0;
}
int Shape::SetOrigin(lua_State* L)
{
	myShape->SetOrigin(*Luna<Vector>::check(L, 1)->myVec);
	return 0;
}
int Shape::SetOutlineWidth(lua_State* L)
{
	myShape->SetOutlineWidth(luaL_checknumberf(L, 1));
	return 0;
}
int Shape::SetPointColor(lua_State* L)
{
	myShape->SetPointColor( luaL_checkint(L, 1), *Luna<Color>::check(L, 2)->myColor );
	return 0;
}
int Shape::SetPointOutlineColor(lua_State* L)
{
	myShape->SetPointOutlineColor( luaL_checkint(L, 1), *Luna<Color>::check(L, 2)->myColor );
	return 0;
}
int Shape::SetPointPos(lua_State* L)
{
	myShape->SetPointPosition( luaL_checkint(L, 1), *Luna<Vector>::check(L, 2)->myVec );
	return 0;
}
int Shape::SetPos(lua_State* L)
{
	myShape->SetPosition(*Luna<Vector>::check(L, 1)->myVec);
	return 0;
}
int Shape::SetRot(lua_State* L)
{
	myShape->SetRotation( luaL_checknumberf(L, 1) );
	return 0;
}
int Shape::SetScale(lua_State* L)
{
	myShape->SetScale(*Luna<Vector>::check(L, 1)->myVec);
	return 0;
}
const char Shape::className[] = "Shape";
Luna<Shape>::RegType Shape::registerMethods[] =
{
	{"AddPoint", &Shape::AddPoint},
	{"SetPointPos", &Shape::SetPointPos},
	{"GetPointPos", &Shape::GetPointPos},
	{"SetPointColor", &Shape::SetPointColor},
	{"GetPointColor", &Shape::GetPointColor},
	{"SetOutlineWidth", &Shape::SetOutlineWidth},
	{"GetOutlineWidth", &Shape::GetOutlineWidth},
	{"SetPointOutlineColor", &Shape::SetPointOutlineColor},
	{"GetPointOutlineColor", &Shape::GetPointOutlineColor},
	{"GetPointCount", &Shape::GetPointCount},
	{"SetPos", &Shape::SetPos},
	{"GetPos", &Shape::GetPos},
	{"SetRot", &Shape::SetRot},
	{"GetRot", &Shape::GetRot},
	{"SetScale", &Shape::SetScale},
	{"GetScale", &Shape::GetScale},
	{"SetBlendMode", &Shape::SetBlendMode},
	{"GetBlendMode", &Shape::GetBlendMode},
	{"SetColor", &Shape::SetColor},
	{"GetColor", &Shape::GetColor},
	{"SetOrigin", &Shape::SetOrigin},
	{"GetOrigin", &Shape::GetOrigin},
	{"Move", &Shape::Move},
	{"X", &Shape::X},
	{"Y", &Shape::Y},

	{NULL,NULL}
};


////////////////////////////////////////////////////////////
// Image
////////////////////////////////////////////////////////////

Image::Image(lua_State* L)
{
	myImage = NULL;
}
Image::Image(sf::Image* i)
: myImage(i)
{
}
Image::~Image()
{
}
int Image::Load(lua_State* L)
{
	lua_getfield(L, LUA_REGISTRYINDEX, GD_LUA_REGISTRY_ADDONID);
	sf::Image* img = AddonReader::Ref().GetImage(luaL_checkstring(L, 1), lua_toint(L, -1));
	lua_pop(L, 1);

	if(!img)
	{
		lua_pushboolean(L, 0);
		return 1;
	}

	myImage = img;

	lua_pushboolean(L, 1);
	return 1;
}
int Image::GetWidth(lua_State* L)
{
	lua_pushinteger(L, myImage->GetWidth());
	return 1;
}
int Image::GetHeight(lua_State* L)
{
	lua_pushinteger(L, myImage->GetHeight());
	return 1;
}
const char Image::className[] = "Image";
Luna<Image>::RegType Image::registerMethods[] =
{
	{"Load", &Image::Load},
	{"GetWidth", &Image::GetWidth},
	{"GetHeight", &Image::GetHeight},

	{NULL,NULL}
};


////////////////////////////////////////////////////////////
// Sprite
////////////////////////////////////////////////////////////

Sprite::Sprite(lua_State* L)
{
	mySprite = new sf::Sprite();
}
Sprite::Sprite(sf::Sprite* s)
: mySprite(s)
{
}
Sprite::~Sprite()
{
	delete mySprite;
}
int Sprite::SetImage(lua_State* L)
{
	Image* img = Luna<Image>::check(L, 1);
	if(img->myImage)
		mySprite->SetImage(*img->myImage, true);
	return 0;
}
int Sprite::GetImage(lua_State* L)
{
	Luna<Image>::pushobject(L, new Image(new sf::Image(*mySprite->GetImage())));
	return 1;
}
int Sprite::GetBlendMode(lua_State* L)
{
	lua_pushinteger(L, mySprite->GetBlendMode());
	return 1;
}
int Sprite::GetColor(lua_State* L)
{
	Luna<Color>::pushobject(L, new Color(new sf::Color(mySprite->GetColor())));
	return 1;
}
int Sprite::GetOrigin(lua_State* L)
{
	Luna<Vector>::pushobject(L, new Vector(new sf::Vector2f(mySprite->GetOrigin())));
	return 1;
}
int Sprite::GetPos(lua_State* L)
{
	Luna<Vector>::pushobject(L, new Vector(new sf::Vector2f(mySprite->GetPosition())));
	return 1;
}
int Sprite::GetRot(lua_State* L)
{
	lua_pushnumber(L, mySprite->GetRotation());
	return 1;
}
int Sprite::GetScale(lua_State* L)
{
	Luna<Vector>::pushobject(L, new Vector(new sf::Vector2f(mySprite->GetScale())));
	return 1;
}
int Sprite::Move(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		// num args
		mySprite->Move(lua_tonumberf(L, 1), lua_tonumberf(L, 2));
	}
	else
	{
		// vec args
		mySprite->Move(* Luna<Vector>::check(L, 1)->myVec);
	}
	return 0;
}
int Sprite::X(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		//set
		mySprite->SetX( lua_tonumberf(L, 1) );
		lua_pushnumber(L, mySprite->GetPosition().x);
		return 1;
	}
	else
	{
		//get
		lua_pushnumber(L, mySprite->GetPosition().x);
		return 1;
	}
}
int Sprite::Y(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		//set
		mySprite->SetY( lua_tonumberf(L, 1) );
		lua_pushnumber(L, mySprite->GetPosition().y);
		return 1;
	}
	else
	{
		//get
		lua_pushnumber(L, mySprite->GetPosition().y);
		return 1;
	}
}
int Sprite::SetBlendMode(lua_State* L)
{
	mySprite->SetBlendMode(static_cast<sf::Blend::Mode>(luaL_checkint(L, 1)));
	return 0;
}
int Sprite::SetColor(lua_State* L)
{
	mySprite->SetColor(*Luna<Color>::check(L, 1)->myColor);
	return 0;
}
int Sprite::SetOrigin(lua_State* L)
{
	mySprite->SetOrigin(*Luna<Vector>::check(L, 1)->myVec);
	return 0;
}
int Sprite::SetPos(lua_State* L)
{
	mySprite->SetPosition(*Luna<Vector>::check(L, 1)->myVec);
	return 0;
}
int Sprite::SetRot(lua_State* L)
{
	mySprite->SetRotation( luaL_checknumberf(L, 1) );
	return 0;
}
int Sprite::SetScale(lua_State* L)
{
	mySprite->SetScale(*Luna<Vector>::check(L, 1)->myVec);
	return 0;
}
int Sprite::SetSubRect(lua_State* L)
{
	mySprite->SetSubRect( sf::IntRect(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4)) );
	return 0;
}
const char Sprite::className[] = "Sprite";
Luna<Sprite>::RegType Sprite::registerMethods[] =
{
	{"SetImage", &Sprite::SetImage},
	{"GetImage", &Sprite::GetImage},
	{"SetPos", &Sprite::SetPos},
	{"GetPos", &Sprite::GetPos},
	{"SetRot", &Sprite::SetRot},
	{"GetRot", &Sprite::GetRot},
	{"SetScale", &Sprite::SetScale},
	{"GetScale", &Sprite::GetScale},
	{"SetBlendMode", &Sprite::SetBlendMode},
	{"GetBlendMode", &Sprite::GetBlendMode},
	{"SetColor", &Sprite::SetColor},
	{"GetColor", &Sprite::GetColor},
	{"SetOrigin", &Sprite::SetOrigin},
	{"GetOrigin", &Sprite::GetOrigin},
	{"Move", &Sprite::Move},
	{"X", &Sprite::X},
	{"Y", &Sprite::Y},
	{"SetSubRect", &Sprite::SetSubRect},

	{NULL,NULL}
};


////////////////////////////////////////////////////////////
// RenderTexture
////////////////////////////////////////////////////////////

RenderTexture::RenderTexture(lua_State* L)
{
	myRenderImage = new sf::RenderImage();
	mySprite = new sf::Sprite();
}
RenderTexture::RenderTexture(sf::RenderImage* r)
: myRenderImage(r)
{
	mySprite = new sf::Sprite();
}
RenderTexture::~RenderTexture()
{
	delete mySprite;
	delete myRenderImage;
}
int RenderTexture::Create(lua_State* l)
{
	bool err = false;
	if(lua_isnumber(l, 1))
	{
		// num args
		if(!(err = myRenderImage->Create(lua_toint(l, 1), lua_toint(l, 2))))
		{
			lua_getfield(l, LUA_REGISTRYINDEX, GD_LUA_REGISTRY_GAMEPTR);
			GameState* game = (GameState*)lua_touserdata(l, -1);
			lua_pop(l, 1);

			game->Console->Print(ui::UIConsole::MSG_TWARNING, GD_MSG_LUA_WARNING_CREATERT);
		}
	}
	else
	{
		// vec args
		Vector* vec = Luna<Vector>::check(l, 1);
		if(!(err=myRenderImage->Create(static_cast<unsigned int>(vec->myVec->x), static_cast<unsigned int>(vec->myVec->y))))
		{
			lua_getfield(l, LUA_REGISTRYINDEX, GD_LUA_REGISTRY_GAMEPTR);
			GameState* game = (GameState*)lua_touserdata(l, -1);
			lua_pop(l, 1);

			game->Console->Print(ui::UIConsole::MSG_TWARNING, GD_MSG_LUA_WARNING_CREATERT);
		}
	}
	mySprite->SetImage(myRenderImage->GetImage(), true);
	return 0;
}
int RenderTexture::Clear(lua_State* l)
{
	Color* col = Luna<Color>::totype(l, 1);
	if(col)
	{
		myRenderImage->Clear(* col->myColor);
	}
	else
	{
		myRenderImage->Clear();
	}
	return 0;
}
int RenderTexture::Draw(lua_State* l)
{
	void* ptr = NULL;

	if(ptr = (void*)Luna<Sprite>::totype(l, 1))
	{
		//Sprite
		Sprite* spr = (Sprite*)ptr;
		myRenderImage->Draw(*spr->mySprite);
	}
	else if(ptr = (void*)Luna<Shape>::totype(l, 1))
	{
		//Shape
		Shape* shp = (Shape*)ptr;
		myRenderImage->Draw(*shp->myShape);
	}
	else if(ptr = (void*)Luna<Text>::totype(l, 1))
	{
		//Text
		Text* txt = (Text*)ptr;
		myRenderImage->Draw(*txt->myText);
	}
	else if(ptr = (void*)Luna<RenderTexture>::totype(l, 1))
	{
		//RenderTexture
		RenderTexture* rt = (RenderTexture*)ptr;
		myRenderImage->Draw(*rt->mySprite);
	}

	return 0;
}
int RenderTexture::Display(lua_State* l)
{
	myRenderImage->Display();
	return 0;
}
int RenderTexture::GetBlendMode(lua_State* L)
{
	lua_pushinteger(L, mySprite->GetBlendMode());
	return 1;
}
int RenderTexture::GetColor(lua_State* L)
{
	Luna<Color>::pushobject(L, new Color(new sf::Color(mySprite->GetColor())));
	return 1;
}
int RenderTexture::GetOrigin(lua_State* L)
{
	Luna<Vector>::pushobject(L, new Vector(new sf::Vector2f(mySprite->GetOrigin())));
	return 1;
}
int RenderTexture::GetPos(lua_State* L)
{
	Luna<Vector>::pushobject(L, new Vector(new sf::Vector2f(mySprite->GetPosition())));
	return 1;
}
int RenderTexture::GetRot(lua_State* L)
{
	lua_pushnumber(L, mySprite->GetRotation());
	return 1;
}
int RenderTexture::GetScale(lua_State* L)
{
	Luna<Vector>::pushobject(L, new Vector(new sf::Vector2f(mySprite->GetScale())));
	return 1;
}
int RenderTexture::GetWidth(lua_State* L)
{
	lua_pushinteger(L, myRenderImage->GetWidth());
	return 1;
}
int RenderTexture::GetHeight(lua_State* L)
{
	lua_pushinteger(L, myRenderImage->GetHeight());
	return 1;
}
int RenderTexture::Move(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		// num args
		mySprite->Move(lua_tonumberf(L, 1), lua_tonumberf(L, 2));
	}
	else
	{
		// vec args
		mySprite->Move(* Luna<Vector>::check(L, 1)->myVec);
	}
	return 0;
}
int RenderTexture::X(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		//set
		mySprite->SetX( lua_tonumberf(L, 1) );
		lua_pushnumber(L, mySprite->GetPosition().x);
		return 1;
	}
	else
	{
		//get
		lua_pushnumber(L, mySprite->GetPosition().x);
		return 1;
	}
}
int RenderTexture::Y(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		//set
		mySprite->SetY( lua_tonumberf(L, 1) );
		lua_pushnumber(L, mySprite->GetPosition().x);
		return 1;
	}
	else
	{
		//get
		lua_pushnumber(L, mySprite->GetPosition().y);
		return 1;
	}
}
int RenderTexture::SetBlendMode(lua_State* L)
{
	mySprite->SetBlendMode(static_cast<sf::Blend::Mode>(luaL_checkint(L, 1)));
	return 0;
}
int RenderTexture::SetColor(lua_State* L)
{
	mySprite->SetColor(*Luna<Color>::check(L, 1)->myColor);
	return 0;
}
int RenderTexture::SetOrigin(lua_State* L)
{
	mySprite->SetOrigin(*Luna<Vector>::check(L, 1)->myVec);
	return 0;
}
int RenderTexture::SetPos(lua_State* L)
{
	mySprite->SetPosition(*Luna<Vector>::check(L, 1)->myVec);
	return 0;
}
int RenderTexture::SetRot(lua_State* L)
{
	mySprite->SetRotation( luaL_checknumberf(L, 1) );
	return 0;
}
int RenderTexture::SetScale(lua_State* L)
{
	mySprite->SetScale(*Luna<Vector>::check(L, 1)->myVec);
	return 0;
}
int RenderTexture::SetSubRect(lua_State* L)
{
	mySprite->SetSubRect( sf::IntRect(luaL_checkint(L, 1), luaL_checkint(L, 2), luaL_checkint(L, 3), luaL_checkint(L, 4)) );
	return 0;
}
const char RenderTexture::className[] = "RenderTexture";
Luna<RenderTexture>::RegType RenderTexture::registerMethods[] =
{
	{"Create", &RenderTexture::Create},
	{"Clear", &RenderTexture::Clear},
	{"Draw", &RenderTexture::Draw},
	{"Display", &RenderTexture::Display},
	{"SetPos", &RenderTexture::SetPos},
	{"GetPos", &RenderTexture::GetPos},
	{"SetRot", &RenderTexture::SetRot},
	{"GetRot", &RenderTexture::GetRot},
	{"SetScale", &RenderTexture::SetScale},
	{"GetScale", &RenderTexture::GetScale},
	{"SetBlendMode", &RenderTexture::SetBlendMode},
	{"GetBlendMode", &RenderTexture::GetBlendMode},
	{"SetColor", &RenderTexture::SetColor},
	{"GetColor", &RenderTexture::GetColor},
	{"SetOrigin", &RenderTexture::SetOrigin},
	{"GetOrigin", &RenderTexture::GetOrigin},
	{"GetWidth", &RenderTexture::GetWidth},
	{"GetHeight", &RenderTexture::GetHeight},
	{"Move", &RenderTexture::Move},
	{"X", &RenderTexture::X},
	{"Y", &RenderTexture::Y},
	{"SetSubRect", &RenderTexture::SetSubRect},

	{NULL,NULL}
};


////////////////////////////////////////////////////////////
// Text
////////////////////////////////////////////////////////////

Text::Text(lua_State* L)
{
	myText = new sf::Text();
}
Text::Text(sf::Text* t)
: myText(t)
{
}
Text::~Text()
{
	delete myText;
}
int Text::GetWidth(lua_State* L)
{
	lua_pushnumber(L, myText->GetRect().Width);
	return 1;
}
int Text::GetHeight(lua_State* L)
{
	lua_pushnumber(L, myText->GetRect().Height);
	return 1;
}
int Text::GetSize(lua_State* L)
{
	lua_pushinteger(L, myText->GetCharacterSize());
	return 1;
}
int Text::SetSize(lua_State* L)
{
	myText->SetCharacterSize(luaL_checkint(L, 1));
	return 0;
}
int Text::GetText(lua_State* L)
{
	lua_pushstring(L, myText->GetString().ToAnsiString().c_str());
	return 1;
}
int Text::SetText(lua_State* L)
{
	myText->SetString(lua_tostring(L, 1));
	return 0;
}
int Text::SetFont(lua_State* L)
{
	const char* file = luaL_checkstring(L, 1);

	if(strcmp(file, GD_NULLSTR)==0)
	{
		//load default font
		myText->SetFont(sf::Font::GetDefaultFont());
		lua_pushboolean(L, 1);
	}
	else
	{
		//load custom font
		lua_getfield(L, LUA_REGISTRYINDEX, GD_LUA_REGISTRY_ADDONID);
		sf::Font* fnt = AddonReader::Ref().GetFont(file, lua_toint(L, -1));
		lua_pop(L, 1);

		if(!fnt)
		{
			lua_pushboolean(L, 0);
			return 1;
		}

		myText->SetFont(*fnt);

		lua_pushboolean(L, 1);
	}
	return 1;
}
int Text::SetPos(lua_State* L)
{
	myText->SetPosition(*Luna<Vector>::check(L, 1)->myVec);
	return 0;
}
int Text::GetPos(lua_State* L)
{
	Luna<Vector>::pushobject(L, new Vector(new sf::Vector2f(myText->GetPosition())));
	return 1;
}
int Text::GetColor(lua_State* L)
{
	Luna<Color>::pushobject(L, new Color(new sf::Color(myText->GetColor())));
	return 1;
}
int Text::SetColor(lua_State* L)
{
	myText->SetColor(*Luna<Color>::check(L, 1)->myColor);
	return 0;
}
int Text::SetOrigin(lua_State* L)
{
	myText->SetOrigin(*Luna<Vector>::check(L, 1)->myVec);
	return 0;
}
int Text::GetOrigin(lua_State* L)
{
	Luna<Vector>::pushobject(L, new Vector(new sf::Vector2f(myText->GetOrigin())));
	return 1;
}
int Text::SetScale(lua_State* L)
{
	myText->SetScale(*Luna<Vector>::check(L, 1)->myVec);
	return 0;
}
int Text::GetScale(lua_State* L)
{
	Luna<Vector>::pushobject(L, new Vector(new sf::Vector2f(myText->GetScale())));
	return 1;
}
int Text::SetBlendMode(lua_State* L)
{
	myText->SetBlendMode(static_cast<sf::Blend::Mode>(luaL_checkint(L, 1)));
	return 0;
}
int Text::GetBlendMode(lua_State* L)
{
	lua_pushnumber(L, myText->GetBlendMode());
	return 1;
}
int Text::SetRot(lua_State* L)
{
	myText->SetRotation(luaL_checknumberf(L, 1));
	return 0;
}
int Text::GetRot(lua_State* L)
{
	lua_pushnumber(L, myText->GetRotation());
	return 1;
}
int Text::Move(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		// num args
		myText->Move(lua_tonumberf(L, 1), lua_tonumberf(L, 2));
	}
	else
	{
		// vec args
		myText->Move(* Luna<Vector>::check(L, 1)->myVec);
	}
	return 0;
}
int Text::X(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		//set

		myText->SetX( lua_tonumberf(L, 1) );
		lua_pushnumber(L, myText->GetPosition().x);
		return 1;
	}
	else
	{
		//get
		lua_pushnumber(L, myText->GetPosition().x);
		return 1;
	}
}
int Text::Y(lua_State* L)
{
	if(lua_isnumber(L, 1))
	{
		//set
		myText->SetY( lua_tonumberf(L, 1) );
		lua_pushnumber(L, myText->GetPosition().y);
		return 1;
	}
	else
	{
		//get
		lua_pushnumber(L, myText->GetPosition().y);
		return 1;
	}
}
const char Text::className[] = "Text";
Luna<Text>::RegType Text::registerMethods[] =
{
	{"GetWidth", &Text::GetWidth},
	{"GetHeight", &Text::GetHeight},
	{"GetSize", &Text::GetSize},
	{"SetSize", &Text::SetSize},
	{"GetText", &Text::GetText},
	{"SetText", &Text::SetText},
	{"SetFont", &Text::SetFont},
	{"SetPos", &Text::SetPos},
	{"GetPos", &Text::GetPos},
	{"GetColor", &Text::GetColor},
	{"SetColor", &Text::SetColor},
	{"SetOrigin", &Text::SetOrigin},
	{"GetOrigin", &Text::GetOrigin},
	{"SetScale", &Text::SetScale},
	{"GetScale", &Text::GetScale},
	{"SetBlendMode", &Text::SetBlendMode},
	{"GetBlendMode", &Text::GetBlendMode},
	{"SetRot", &Text::SetRot},
	{"GetRot", &Text::GetRot},

	{NULL,NULL}
};
