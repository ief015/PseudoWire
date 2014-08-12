#include "UICompContext.h"
#include <sys/sys.h>
#include "UI_DATA.h"
#include "../GameData.h"

#include "../LuaComponent.h"
#include "../Fonts.h"

#include "../GameLoader.h"
#include "../Renderer.h"

#include <SFML/Graphics.hpp>

using namespace game::ui;

sf::Text UICompContext::myText = sf::Text();

void UICompContext::StaticInit()
{
	myText = sf::Text(GD_NULLSTR, Fonts::font_COMMS, 12);
	myText.SetColor( sf::Color(16, 16, 16) );

}

UICompContext::UICompContext(sys::State* parent, UICONTEXT_TYPE type)
: sys::Component(parent)
, Type(type)
{
	UserData = (void*)(new ui::UI_DATATYPE(ui::UI_CONTEXT));

	Size.X = 160;
	listItemSize = 16.0f;
	numItemsAtOnce = 0;
	Size.Y = listItemSize * numItemsAtOnce;

	selected_item = 0;

	shp_selectionbox = sf::Shape::Rectangle(
		0.0f, 0.0f,
		static_cast<float>(Size.X), listItemSize,
		sf::Color(36, 158, 255, 128),
		1.0f,
		sf::Color(16, 125, 185, 128)
	);
}

UICompContext::~UICompContext()
{
	delete UserData;
}

void UICompContext::UpdateList(game::LuaComponent* comp)
{
	itemStr.clear();
	comp_ref = comp;

	switch(Type)
	{
	case UICONTEXT_INIT:
		for(int i = 0; i < NUM_UICONTEXTITEM_INIT; ++i)
		{
			switch(i)
			{
			case UICONTEXTITEM_INIT_WIREINPUT: itemStr.push_back(GD_CONTEXT_INIT_WIREINPUT); break;
			case UICONTEXTITEM_INIT_WIREOUTPUT: itemStr.push_back(GD_CONTEXT_INIT_WIREOUTPUT); break;
			case UICONTEXTITEM_INIT_EDIT: itemStr.push_back(GD_CONTEXT_INIT_EDIT); break;
			case UICONTEXTITEM_INIT_UNWIREINPUT: itemStr.push_back(GD_CONTEXT_INIT_UNWIREINPUT); break;
			case UICONTEXTITEM_INIT_UNWIREOUTPUT: itemStr.push_back(GD_CONTEXT_INIT_UNWIREOUTPUT); break;
			case UICONTEXTITEM_INIT_REMOVE: itemStr.push_back(GD_CONTEXT_INIT_REMOVE);break;
			default: break;
			}
		}
		numItemsAtOnce = NUM_UICONTEXTITEM_INIT;
		break;
	case UICONTEXT_SELECTINPUT_WIRE:
		for(int i = 0; i < comp->InputPorts.size(); ++i)
		{
			itemStr.push_back( comp->InputPorts[i].port_name );
		}
		numItemsAtOnce = comp->InputPorts.size();
		break;
	case UICONTEXT_SELECTOUTPUT_WIRE:
		for(int i = 0; i < comp->OutputPorts.size(); ++i)
		{
			itemStr.push_back( comp->OutputPorts[i].port_name );
		}
		numItemsAtOnce = comp->OutputPorts.size();
		break;
	case UICONTEXT_SELECTINPUT_UNWIRE:
		itemStr.push_back(GD_CONTEXT_UNWIRE_ALL);
		for(int i = 0; i < comp->InputPorts.size(); ++i)
		{
			itemStr.push_back( comp->InputPorts[i].port_name );
		}
		numItemsAtOnce = comp->InputPorts.size()+1;
		break;
	case UICONTEXT_SELECTOUTPUT_UNWIRE:
		itemStr.push_back(GD_CONTEXT_UNWIRE_ALL);
		for(int i = 0; i < comp->OutputPorts.size(); ++i)
		{
			itemStr.push_back( comp->OutputPorts[i].port_name );
		}
		numItemsAtOnce = comp->OutputPorts.size()+1;
		break;
	}

	Size.X = 160;
	listItemSize = 16.0f;
	numItemsAtOnce = itemStr.size();
	Size.Y = listItemSize * numItemsAtOnce;

	shp_listbox = sf::Shape::Rectangle(
		0.0f, 0.0f,
		static_cast<float>(Size.X+2), static_cast<float>(Size.Y+2),
		sf::Color(245, 235, 160),
		1.0f,
		sf::Color(50, 40, 20, 160)
	);
	//shp_listbox.SetPointColor(1, sf::Color(230, 240, 250));
	//shp_listbox.SetPointColor(2, sf::Color(230, 240, 250));
	shp_listbox.SetPointColor(2, sf::Color(255, 245, 225));
	shp_listbox.SetPointColor(3, sf::Color(255, 245, 235));
}

void UICompContext::Draw(const sys::Point &screenpos)
{
#define rw game::Renderer::Ref()

	// Draw list box
	shp_listbox.SetPosition(static_cast<float>(screenpos.X-1), static_cast<float>(screenpos.Y-1));
	rw->Draw( shp_listbox );

	// Draw selection box
	if(mouse_inside)
	{
		shp_selectionbox.SetPosition( static_cast<float>(screenpos.X), screenpos.Y + (selected_item * listItemSize) );
		rw->Draw( shp_selectionbox );
	}

	// Draw categories and items
	for(int i = 0; i < itemStr.size(); ++i)
	{
		myText.SetString( itemStr[i] );
		myText.SetPosition( static_cast<float>(screenpos.X + 16), screenpos.Y + (i * listItemSize) );
		rw->Draw( myText );
	}

#undef rw;
}

void UICompContext::OnMouseHover(int localx, int localy)
{
}
void UICompContext::OnMouseMoved(int localx, int localy, int dx, int dy)
{
}
void UICompContext::OnMouseMovedInside(int localx, int localy, int dx, int dy)
{
	//update selected item
	mouse_inside = true;
	selected_item = static_cast<int>(floor(localy / listItemSize));
}
void UICompContext::OnMouseEnter(int localx, int localy)
{
}
void UICompContext::OnMouseLeave(int localx, int localy)
{
	mouse_inside = false;
}
void UICompContext::OnMouseDown(int x, int y, unsigned int button)
{
}
void UICompContext::OnMouseUp(int x, int y, unsigned int button)
{
}
void UICompContext::OnMouseClick(int localx, int localy, unsigned int button)
{
	ItemClicked = static_cast<int>(localy / listItemSize);
	//selected_item = 0;
}
void UICompContext::OnMouseUnclick(int localx, int localy, unsigned int button)
{
}
void UICompContext::OnMouseWheel(int localx, int localy, int d)
{
}
void UICompContext::OnMouseWheelInside(int localx, int localy, int d)
{
}
void UICompContext::OnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
}
void UICompContext::OnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}
void UICompContext::OnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
}
void UICompContext::OnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
}
