#include "UIEdit_Explorer.h"

#include <sys/sys.h>
#include <sys/io/io.h>
#include "../Renderer.h"
#include "../Fonts.h"
#include "UIEdit.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "UIScrollBar.h"

using namespace game;
using namespace game::ui;

const int UIEdit_Explorer::ITEM_HEIGHT = 16;
const int UIEdit_Explorer::VIEW_HEIGHT_ITEMS = 10;

UIEdit_Explorer::UIEdit_Explorer(sys::State* s) : sys::XComponent(s)
{
	Size.X = 224.0f;
	Size.Y = ITEM_HEIGHT * VIEW_HEIGHT_ITEMS;

	shpview = sf::Shape::Rectangle(
		sf::FloatRect(0.0f, 0.0f, static_cast<float>(Size.X), static_cast<float>(Size.Y)),
		sf::Color(250, 250, 250),
		1.0,
		sf::Color(123, 150, 185)
	);
	shpview.SetPointColor(0, sf::Color(204, 204, 204));
	shpview.SetPointColor(1, sf::Color(204, 204, 204));

	shphighlight = sf::Shape::Rectangle(
		sf::FloatRect(0.0f, 0.0f, static_cast<float>(Size.X), static_cast<float>(ITEM_HEIGHT)),
		sf::Color(36, 158, 255, 128),
		1.0f,
		sf::Color(16, 125, 185, 128)
	);

	scrollamount = 0;
	selecteditem = -1;

	scrollbar = new UIScrollBar(s);
	scrollbar->SetOrientation(UIScrollBar::ORIENT_VERT);
	scrollbar->SetPxWidth(20);
	scrollbar->SetPxLength(ITEM_HEIGHT * VIEW_HEIGHT_ITEMS);
	scrollbar->Position.X = this->Size.X - scrollbar->GetPxWidth();
	scrollbar->Position.Y = 0;
	scrollbar->SetMin(0);
	scrollbar->SetMax(texts.size()-VIEW_HEIGHT_ITEMS);
	scrollbar->SetValue(scrollamount);
	this->AddChild(scrollbar);
}

UIEdit_Explorer::~UIEdit_Explorer()
{
	for(int i = 0; i < texts.size(); ++i)
		delete texts[i];

	this->RemoveChild(scrollbar);
	delete scrollbar;
}

void UIEdit_Explorer::OpenFolder(const char* foldername)
{
	for(int i = 0; i < texts.size(); ++i)
		delete texts[i];
	texts.clear();

	sys::io::Path folder(foldername);
	std::vector<sys::io::File> files;
	folder.GetAllFiles(files);

	for(int i = 0; i < files.size(); ++i)
	{
		sf::Text* txt = new sf::Text(files[i].GetName(), sf::Font::GetDefaultFont(), 12);
		txt->SetColor(sf::Color::Black);
		txt->SetPosition(4.0f, 0.0f);
		texts.push_back(txt);
	}

	scrollbar->SetMax(texts.size()-VIEW_HEIGHT_ITEMS);
	scrollbar->SetValue(scrollamount);
}

std::string UIEdit_Explorer::ItemSelected() const
{
	if(selecteditem == -1)
		return GD_NULLSTR;
	return texts[selecteditem]->GetString().ToAnsiString();
}

void UIEdit_Explorer::XTick(float dt)
{
	if (scrollbar->PollChanged())
	{
		scrollamount = scrollbar->GetValue();
	}
}

#define rw game::Renderer::Ref()
void UIEdit_Explorer::XDraw(const sys::Point& screenPos)
{
	shpview.Move(screenPos.X, screenPos.Y);
	rw->Draw(shpview);
	shpview.Move(-screenPos.X, -screenPos.Y);

	for(int i = scrollamount; i < texts.size() && i < scrollamount + VIEW_HEIGHT_ITEMS; ++i)
	{
		int d = i - scrollamount;

		if(i == selecteditem)
		{
			shphighlight.Move(screenPos.X, static_cast<float>(d*ITEM_HEIGHT) + screenPos.Y);
			rw->Draw(shphighlight);
			shphighlight.Move(-screenPos.X,-static_cast<float>(d*ITEM_HEIGHT) - screenPos.Y);
		}

		texts[i]->Move(screenPos.X, static_cast<float>(d*ITEM_HEIGHT) + screenPos.Y);
		rw->Draw(*texts[i]);
		texts[i]->Move(-screenPos.X,-static_cast<float>(d*ITEM_HEIGHT) -screenPos.Y);
	}
}
#undef rw
	
void UIEdit_Explorer::XOnMouseHover(int localx, int localy)
{
}
void UIEdit_Explorer::XOnMouseMoved(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Explorer::XOnMouseMovedInside(int localx, int localy, int dx, int dy)
{
}
void UIEdit_Explorer::XOnMouseEnter(int localx, int localy)
{
}
void UIEdit_Explorer::XOnMouseLeave(int localx, int localy)
{
}
void UIEdit_Explorer::XOnMouseDown(int x, int y, unsigned int button)
{
}
void UIEdit_Explorer::XOnMouseUp(int x, int y, unsigned int button)
{
}
void UIEdit_Explorer::XOnMouseClick(int localx, int localy, unsigned int button)
{
	selecteditem = (localy / ITEM_HEIGHT)+scrollamount;

	if(selecteditem >= texts.size())
		selecteditem = -1;
}
void UIEdit_Explorer::XOnMouseUnclick(int localx, int localy, unsigned int button)
{
}
void UIEdit_Explorer::XOnMouseWheel(int localx, int localy, int d)
{
	if (this->IsFocused())
	{
		if(d < 0)
		{
			scrollbar->ScrollUp();
		}
		else if(d > 0)
		{
			scrollbar->ScrollDown();
		}
	}
}
void UIEdit_Explorer::XOnMouseWheelInside(int localx, int localy, int d)
{
}
void UIEdit_Explorer::XOnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
	if(key == sf::Key::Up)
		OnMouseWheelInside(0,0,1);
	else if(key == sf::Key::Down)
		OnMouseWheelInside(0,0,-1);
}
void UIEdit_Explorer::XOnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Explorer::XOnKeyDown(int key, bool shift, bool ctrl, bool alt)
{
	
}
void UIEdit_Explorer::XOnKeyUp(int key, bool shift, bool ctrl, bool alt)
{
}
void UIEdit_Explorer::XOnText(int key)
{
}
