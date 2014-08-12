#include "UITree.h"
#include <sys/sys.h>
#include "UI_DATA.h"
#include "../Fonts.h"
#include "../GameData.h"
#include "../GameState.h"
#include "UIConsole.h"

#include "../GameLoader.h"
#include "../Renderer.h"

using namespace game::ui;

sf::Text UITree::UITreeText = sf::Text();
sf::Image UITree::UITreeImgCollapsed = sf::Image();
sf::Image UITree::UITreeImgClosed = sf::Image();

void UITree::StaticInit()
{
	UITreeText = sf::Text("", Fonts::font_COMMS, 12);
	UITreeText.SetColor( sf::Color(16, 16, 16) );

	UITreeImgClosed.LoadFromFile(GD_RES_BULLET_PLUS);
	UITreeImgCollapsed.LoadFromFile(GD_RES_BULLET_MINUS);
}

UITree::UITree(sys::State* parent)
: sys::Component(parent)
{
	UserData = (void*)(new ui::UI_DATATYPE(ui::UI_TREE));

	numItemsOpen = 0;

	//traverse all packages
	for(int pack = 0; pack < AddonReader::Ref().Packages.size(); ++pack)
	{
		//read in all categories within package
		for(int c = 0; c < AddonReader::Ref().Packages[pack]->categories.size(); ++c, ++numItemsOpen)
		{
			AddonCategory* cat = AddonReader::Ref().Packages[pack]->categories[c];

			//insert items if category with same name exists
			bool exists = false;
			int exists_i;
			for(exists_i = 0; exists_i < categories.size(); ++exists_i)
			{
				if(categories[exists_i].name == cat->name)
				{
					exists = true;
					break;
				}
			}

			if(exists)
			{
				for(int i = 0; i < cat->items.size(); ++i)
					categories[exists_i].item_refs.push_back( cat->items[i] );
			}
			else
			{
				UITree_Category newitem;
				newitem.name = cat->name;

				for(int i = 0; i < cat->items.size(); ++i)
					newitem.item_refs.push_back( cat->items[i] );

				categories.push_back( newitem );
			}
		}
	}

	Size.X = 160;
	Size.Y = 260;
	listItemSize = 20.0f;
	numItemsAtOnce = Size.Y / static_cast<int>(listItemSize);
	scrollUpCount = 0;
	selected_item = 0;
	last_printed_item = -1;
	ReadyToClose = false;

	shp_listbox = sf::Shape::Rectangle(
		0.0f, 0.0f,
		static_cast<float>(Size.X+2), static_cast<float>(Size.Y+2),
		sf::Color(190, 200, 205),
		1.0f,
		sf::Color(20, 40, 50, 160)
	);
	//shp_listbox.SetPointColor(1, sf::Color(230, 240, 250));
	//shp_listbox.SetPointColor(2, sf::Color(230, 240, 250));
	shp_listbox.SetPointColor(2, sf::Color(230, 240, 250));
	shp_listbox.SetPointColor(3, sf::Color(230, 240, 250));

	shp_selectionbox = sf::Shape::Rectangle(
		0.0f, 0.0f,
		static_cast<float>(Size.X), listItemSize,
		sf::Color(36, 158, 255, 128),
		1.0f,
		sf::Color(16, 125, 185, 128)
	);

	spr_closed.SetImage(UITreeImgClosed);
	spr_collapsed.SetImage(UITreeImgCollapsed);
}

UITree::~UITree()
{
	delete UserData;
}

void UITree::Draw(const sys::Point &screenpos)
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
	for(int c = 0, printcount = 0, _iter = 0; c < categories.size() && printcount < numItemsAtOnce; ++c, ++_iter)
	{
		bool draw = _iter >= scrollUpCount;
		if(draw)
		{
			// Draw category text
			UITreeText.SetString( categories[c].name );
			UITreeText.SetPosition( static_cast<float>(screenpos.X + 16), screenpos.Y + (printcount * listItemSize) + 2 );
			rw->Draw( UITreeText );

			//++printcount;
		}

		if(categories[c].collapsed)
		{
			if(draw)
			{
				// Draw collapsed box
				spr_collapsed.SetPosition( static_cast<float>(screenpos.X), screenpos.Y + (printcount * listItemSize) + 2 );
				rw->Draw(spr_collapsed);

				++printcount;
			}

			for(int i = 0; i < categories[c].item_refs.size() && printcount < numItemsAtOnce; ++i)
			{
				++_iter;
				draw = _iter >= scrollUpCount;
				if(draw)
				{
					// Draw item text
					AddonItem* item = categories[c].item_refs[i];

					UITreeText.SetString( item->name );
					UITreeText.SetPosition( static_cast<float>(screenpos.X + 32), screenpos.Y + ((printcount) * listItemSize) + 2 );
					rw->Draw( UITreeText );

					++printcount;
				}
			}
		}
		else
		{
			if(draw)
			{
				// Draw closed box
				spr_closed.SetPosition( static_cast<float>(screenpos.X), screenpos.Y + (printcount * listItemSize) + 2 );
				rw->Draw(spr_closed);

				++printcount;
			}
		}
	}

#undef rw;
}

void UITree::PrintSelected(bool force)
{
	if(selected_item == last_printed_item && !force)
		return; //no need to print

	int itemHover = selected_item + scrollUpCount;

	// find the actual item selected
	for(int i = 0, cur = 0; i < categories.size(); ++i, ++cur)
	{
		if(cur == itemHover)
		{
			((GameState*)GetParentState())->Console->Print(ui::UIConsole::MSG_TINFO_NOCONSOLE, categories[i].name.c_str());
		}

		//selected inside a category
		if(categories[i].collapsed)
		{
			bool foundItem = false;
			for(int j = 0; j < categories[i].item_refs.size(); ++j, ++cur)
			{
				//selected an item
				if(cur+1 == itemHover)
				{
					((GameState*)GetParentState())->Console->Print(ui::UIConsole::MSG_TINFO_NOCONSOLE, categories[i].item_refs[j]->description.c_str());
					foundItem = true;
					break;
				}
			}

			if(foundItem)
				break;
		}
	}

	last_printed_item = selected_item;
}

void UITree::OnMouseHover(int localx, int localy)
{
}
void UITree::OnMouseMoved(int localx, int localy, int dx, int dy)
{
}
void UITree::OnMouseMovedInside(int localx, int localy, int dx, int dy)
{
	// update selected item
	mouse_inside = true;
	selected_item = static_cast<int>(floor(localy / listItemSize));

	PrintSelected();
}
void UITree::OnMouseEnter(int localx, int localy)
{
}
void UITree::OnMouseLeave(int localx, int localy)
{
	mouse_inside = false;
}
void UITree::OnMouseDown(int x, int y, unsigned int button)
{
}
void UITree::OnMouseUp(int x, int y, unsigned int button)
{
}
void UITree::OnMouseClick(int localx, int localy, unsigned int button)
{
	int itemClicked = static_cast<int>(floor(localy / listItemSize)) + scrollUpCount;
	ReadyToClose = true;
	ItemSelected = NULL;

	// find the actual item selected
	for(int i = 0, cur = 0; i < categories.size(); ++i, ++cur)
	{
		//selected a category -- toggle it
		if(cur == itemClicked)
		{
			if(categories[i].collapsed)
				numItemsOpen -= categories[i].item_refs.size();
			else
				numItemsOpen += categories[i].item_refs.size();
			
			categories[i].collapsed = !categories[i].collapsed;
			ReadyToClose = false;
			break;
		}

		//selected inside a category
		if(categories[i].collapsed)
		{
			bool foundItem = false;
			for(int j = 0; j < categories[i].item_refs.size(); ++j, ++cur)
			{
				//selected an item
				if(cur+1 == itemClicked)
				{
					ItemSelected = categories[i].item_refs[j];
					foundItem = true;
					break;
				}
			}

			if(foundItem)
				break;
		}
	}
}
void UITree::OnMouseUnclick(int localx, int localy, unsigned int button)
{
}
void UITree::OnMouseWheel(int localx, int localy, int d)
{
}
void UITree::OnMouseWheelInside(int localx, int localy, int d)
{
	//scroll up or down
	if(d<0)
	{
		//down
		int before_scroll = scrollUpCount++;

		if(numItemsOpen < (scrollUpCount+numItemsAtOnce) || numItemsOpen <= numItemsAtOnce)
			--scrollUpCount;

		if(before_scroll != scrollUpCount)
			PrintSelected(true);
	}
	else if (d>0)
	{
		//up
		scrollUpCount--;

		if(scrollUpCount < 0)
			scrollUpCount = 0;
		else
			PrintSelected(true);
	}
}
void UITree::OnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
}
void UITree::OnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}
void UITree::OnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
	if(!mouse_inside)
		return;

	if(key == sf::Key::Up)
		OnMouseWheelInside(0,0,1);
	else if(key == sf::Key::Down)
		OnMouseWheelInside(0,0,-1);
}
void UITree::OnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
}
