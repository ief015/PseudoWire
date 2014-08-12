#include "GameState.h"

#include "Renderer.h"
#include "UI/UI_DATA.h"
#include <sys/sys.h>
#include <sys/io/io.h>
#include "GameData.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "LuaComponent.h"
#include "GameLoader.h"
#include "UI/UITree.h"
#include "UI/UICompContext.h"
#include "UI/UIMenu.h"
#include "UI/UIEdit.h"
#include "UI/UIWindow.h"
#include "UI/UIConsole.h"
#include "UI/UIButton.h"
#include "UI/UIEdit_Check.h"
#include "UI/UIEdit_Radio.h"
#include "UI/UIEdit_String.h"
#include "UI/UIEdit_Number.h"
#include "UI/UIEdit_Label.h"
#include "UI/UIEdit_Sprite.h"
#include "UI/UIEdit_Explorer.h"
#include "Fonts.h"
#include "tinyxml/tinyxml.h"

#include <windows.h>
#include <psapi.h>

#include "lua/lua.hpp"

#include "userdefs.h"

using namespace game;

/****************************************/
/************ USER INTERFACE ************/
/****************************************/

#define max(a,b) ((a>b)?a:b)
#define min(a,b) ((a<b)?a:b)

GameState::GameState()
{
}

GameState::~GameState()
{
}

void GameState::OnInitialized()
{
	//Initializations
	EditMode = true;
	GridMode = false;
	GridSize = 8;
	Panning = false;
	Wiring = false;
	context_state = 0;
	ShowDebug = false;
	ITER = 0;
	DragComponent = NULL;
	HoverComponent = NULL;
	ScrW = SCR_WIDTH;
	ScrH = SCR_HEIGHT;
	ScrFullscreen = false;
	LockMenu = false;

	luacomps.clear();
	deadendlist.clear();
	updatestack.Reserve(0x80);
	pathstack.Reserve(0x80);

	//Load user settings
	LoadUserSettings(GD_SETTINGS_FILENAME);
	if( ScrW != SCR_WIDTH || ScrH != SCR_HEIGHT || ScrFullscreen )
	{
		//not using defaults
		Renderer::Ref().Create(ScrW, ScrH, GD_WINDOWTITLE, ScrFullscreen ? sf::Style::Fullscreen : sf::Style::Default, 4);
		sys::Engine::Ref().SetSize(ScrW, ScrH);
	}

	//Draw loading screen
	DrawLoading();

	//RGBA Window icon
	Renderer::Ref()->SetIcon(16, 16, GD_ICON_DATA);

	//Catalogue addons
	AddonReader::CreateSingleton();
	AddonReader::Ref().RefreshAddons();
	
	//Initialize UI components
	Fonts::StaticInit();
	LuaComponent::StaticInit();
	ui::UITree::StaticInit();
	ui::UICompContext::StaticInit();
	ui::UIWindow_Static::StaticInit();
	ui::UIEdit::StaticInit();
	ui::UIConsole::StaticInit();

	//Create component list
	TreeContextOpened = false;
	TreeContext = new ui::UITree(this);

	//Create contexts
	ContextComponent = new ui::UICompContext(this, ui::UICONTEXT_INIT);
	ContextInputs = new ui::UICompContext(this, ui::UICONTEXT_SELECTINPUT_WIRE);
	ContextOutputs = new ui::UICompContext(this, ui::UICONTEXT_SELECTOUTPUT_WIRE);
	ContextInputs_Unwire = new ui::UICompContext(this, ui::UICONTEXT_SELECTINPUT_UNWIRE);
	ContextOutputs_Unwire = new ui::UICompContext(this, ui::UICONTEXT_SELECTOUTPUT_UNWIRE);
	EditWindow = new ui::UIEdit(this);

	//Create menu
	Menu = new ui::UIMenu(this, Renderer::Ref()->GetWidth());
	Menu->Position = sys::Point(0, 0);
	Menu->AddItem(GD_GAME_MENU_NEW, GD_RES_WHITEPAGE);
	Menu->AddItem(GD_GAME_MENU_LOAD, GD_RES_FOLDER);
	Menu->AddItem(GD_GAME_MENU_SAVE, GD_RES_DISK);
	Menu->AddItem(GD_GAME_MENU_GAMEMODE, GD_RES_CONTROLLER);
	Menu->AddItem(GD_GAME_MENU_ENABLEGRID, GD_RES_GRID);
	Menu->AddItem(GD_GAME_MENU_OPTIONS, GD_RES_COG);
	Menu->AddItem(GD_GAME_MENU_ABOUT, GD_RES_ABOUT);
	Menu->AddItem(GD_GAME_MENU_EXIT, GD_RES_GET_OUT);
	this->AddComponent(Menu);

	//Create console logger
	Console = new ui::UIConsole(this, Renderer::Ref()->GetWidth(), Renderer::Ref()->GetHeight());
	Console->Print(ui::UIConsole::MSG_TWELCOME, GD_MSG_WELCOME);
	this->AddComponent(Console);

	//Create Load/Save windows
	WindowLoad = new ui::UIWindow<GameState>(this);
	WindowSave = new ui::UIWindow<GameState>(this);
	WindowLoad->OnResult_Obj = this;
	WindowSave->OnResult_Obj = this;
	WindowLoad->OnResult = &GameState::WindowLoad_OnResult;
	WindowSave->OnResult = &GameState::WindowSave_OnResult;
	WindowLoad->SetTitle(GD_WINDOW_LOAD_TITLE);
	WindowSave->SetTitle(GD_WINDOW_SAVE_TITLE);
	WindowLoad->Position.X = 24;
	WindowLoad->Position.Y = 32;
	WindowSave->Position.X = 24;
	WindowSave->Position.Y = 32;

	WindowSave_txtFile = new ui::UIEdit_String(this);
	WindowSave_txtFile->Position.X = 10;
	WindowSave_txtFile->Position.Y = 32;
	WindowSave_txtFile->SetText(GD_WINDOW_SAVE_MESSAGE);
	WindowSave_txtFile->SetString(GD_WINDOW_SAVE_DEFUALT);
	WindowSave->AddChild(WindowSave_txtFile);
	WindowSave->Size.Y += WindowSave_txtFile->Size.Y + 8;

	WindowSave_chkSaveOutVals = new ui::UIEdit_Check(this);
	WindowSave_chkSaveOutVals->Position.X = 10;
	WindowSave_chkSaveOutVals->Position.Y = 72;
	WindowSave_chkSaveOutVals->SetText(GD_WINDOW_SAVE_SAVEOUTDATA);
	WindowSave->AddChild(WindowSave_chkSaveOutVals);
	WindowSave->Size.Y += WindowSave_chkSaveOutVals->Size.Y + 8 + 12;

	/*WindowLoad_txtFile = new ui::UIEdit_String(this);
	WindowLoad_txtFile->Position.X = 10;
	WindowLoad_txtFile->Position.Y = 32;
	WindowLoad_txtFile->SetText(GD_WINDOW_LOAD_MESSAGE);
	WindowLoad_txtFile->SetString(GD_WINDOW_LOAD_DEFUALT);
	WindowLoad->AddChild(WindowLoad_txtFile);
	WindowLoad->Size.Y += WindowLoad_txtFile->Size.Y + 16;*/

	WindowLoad_exp = new ui::UIEdit_Explorer(this);
	WindowLoad_exp->Position.X = (WindowLoad->Size.X - WindowLoad_exp->Size.X)/2;
	WindowLoad_exp->Position.Y = 32;
	WindowLoad->AddChild(WindowLoad_exp);
	WindowLoad->Size.Y += WindowLoad_exp->Size.Y + 12;

	WindowContinue = new ui::UIWindow<GameState>(this);
	WindowContinue->OnResult_Obj = this;
	WindowContinue->OnResult = &GameState::WindowContinue_OnResult;
	WindowContinue->SetTitle(GD_WINDOW_CONTINUE_TITLE);
	WindowContinue->Position.X = 24;
	WindowContinue->Position.Y = 32;
	WindowContinue_State = 0;
	WindowContinue_Label = new ui::UIEdit_Label(this);
	WindowContinue_Label->Position.X = 10;
	WindowContinue_Label->Position.Y = 32;
	WindowContinue->AddChild(WindowContinue_Label);
	WindowContinue->Size.Y = WindowContinue_Label->Size.Y + 32;

	WindowOptions = new ui::UIWindow<GameState>(this);
	WindowOptions->OnResult_Obj = this;
	WindowOptions->OnResult = &GameState::WindowOptions_OnResult;
	WindowOptions->Position.X = 24;
	WindowOptions->Position.Y = 32;
	WindowOptions->SetTitle( GD_WINDOW_OPTIONS_TITLE );
	
	WindowOptions_BatterySaver = new ui::UIEdit_Check(this);
	WindowOptions_BatterySaver->Position.X = 28;
	WindowOptions_BatterySaver->Position.Y = 24;
	WindowOptions_BatterySaver->SetText( GD_WINDOW_OPTIONS_BATTERYSAVER );
	WindowOptions_BatterySaver->SetCheck( BatteryMode != BATTERYMODE_OFF );

	WindowOptions_BatterySaver_iconimg = new sf::Image();
	WindowOptions_BatterySaver_iconimg->LoadFromFile( GD_RES_ZING );

	WindowOptions_BatterySaver_icon = new ui::UIEdit_Sprite(this);
	WindowOptions_BatterySaver_icon->Position.X = 10;
	WindowOptions_BatterySaver_icon->Position.Y = 24;
	WindowOptions_BatterySaver_icon->Load( * WindowOptions_BatterySaver_iconimg );

	WindowOptions_BatterySaver_Auto = new ui::UIEdit_Radio(this, "options_batterysaver");
	WindowOptions_BatterySaver_AlwaysOn = new ui::UIEdit_Radio(this, "options_batterysaver", WindowOptions_BatterySaver_Auto);
	WindowOptions_BatterySaver_Auto->SetCheck(true);
	WindowOptions_BatterySaver_AlwaysOn->SetCheck(false);
	WindowOptions_BatterySaver_Auto->SetText( GD_WINDOW_OPTIONS_BATTERYSAVER_AUTO );
	WindowOptions_BatterySaver_AlwaysOn->SetText( GD_WINDOW_OPTIONS_BATTERYSAVER_ALWAYSON );
	WindowOptions_BatterySaver_Auto->Position.X = 28;
	WindowOptions_BatterySaver_Auto->Position.Y = 42;
	WindowOptions_BatterySaver_AlwaysOn->Position.X = 72;
	WindowOptions_BatterySaver_AlwaysOn->Position.Y = 42;

	WindowOptions_GridSize = new ui::UIEdit_Number(this);
	WindowOptions_GridSize->Position.X = 28;
	WindowOptions_GridSize->Position.Y = 60;
	WindowOptions_GridSize->min = 1;
	WindowOptions_GridSize->max = 512;
	WindowOptions_GridSize->SetText( GD_WINDOW_OPTIONS_GRIDSIZE );
	WindowOptions_GridSize->SetString( sys::StringHelper::ToStr(GridSize).c_str() );

	WindowOptions_RefreshAddons = new ui::UIButton(this);
	WindowOptions_RefreshAddons->Position.X = 32;
	WindowOptions_RefreshAddons->Position.Y = 104;
	WindowOptions_RefreshAddons->Size.X = 92;
	WindowOptions_RefreshAddons->Size.Y = 24;
	WindowOptions_RefreshAddons->SetText("Refresh Addons");

	WindowOptions->Size.Y = 48;
	WindowOptions->AddChild(WindowOptions_BatterySaver);
	WindowOptions->AddChild(WindowOptions_BatterySaver_icon);
	WindowOptions->Size.Y += WindowOptions_BatterySaver->Size.Y;
	WindowOptions->AddChild(WindowOptions_BatterySaver_Auto);
	WindowOptions->AddChild(WindowOptions_BatterySaver_AlwaysOn);
	WindowOptions->Size.Y += WindowOptions_BatterySaver_Auto->Size.Y;
	WindowOptions->AddChild(WindowOptions_GridSize);
	WindowOptions->Size.Y += WindowOptions_GridSize->Size.Y;
	WindowOptions->AddChild(WindowOptions_RefreshAddons);
	WindowOptions->Size.Y += WindowOptions_RefreshAddons->Size.Y;

	WindowAbout = new ui::UIWindow<GameState>(this);
	WindowAbout->OnResult_Obj = this;
	WindowAbout->OnResult = &GameState::WindowAbout_OnResult;
	WindowAbout->SetTitle(GD_WINDOW_ABOUT_TITLE);
	WindowAbout->Position.X = 24;
	WindowAbout->Position.Y = 32;
	WindowAbout_Label = new ui::UIEdit_Label(this);
	{
		sys::String str = GD_NULLSTR;
		str << GD_NAME << GD_NEWLINE;
		str << GD_NEWLINE;
		str << "By: " << GD_AUTHOR << GD_NEWLINE;
		str << "Version: " << GD_VERSION << GD_NEWLINE;
		str << "Email/Paypal: " << GD_EMAIL << GD_NEWLINE;
		str << "Donations are welcome! :)" << GD_NEWLINE;
		str << GD_NEWLINE;
		str << GD_COPYRIGHT << GD_NEWLINE;
		str << GD_NEWLINE << GD_NEWLINE;
		str << GD_SPECIAL_THANKS << GD_NEWLINE;
		str << GD_NEWLINE;
		str << GD_SPECIAL_THANKS_WIREMOD << GD_NEWLINE;
		str << GD_SPECIAL_THANKS_JAMIEPOOLE << GD_NEWLINE;
		str << GD_SPECIAL_THANKS_DECODAMAN << GD_NEWLINE;

		WindowAbout_Label->SetString(str.C_Str());
	}
	WindowAbout_Label->Position.X = 10;
	WindowAbout_Label->Position.Y = 32;
	WindowAbout->AddChild(WindowAbout_Label);
	WindowAbout->Size.X = WindowAbout_Label->Size.X + 22;
	WindowAbout->Size.Y = WindowAbout_Label->Size.Y + 32;

	//Mouse
	Renderer::Ref()->ShowMouseCursor(false);
	m_img = new sf::Image();
	m_img->LoadFromFile(GD_RES_MOUSE);
	m_spr = new sf::Sprite(*m_img);
	mw_img = new sf::Image();
	mw_img->LoadFromFile(GD_RES_WRENCHORANGE);
	mw_spr = new sf::Sprite(*mw_img);
	mw_spr->SetPosition(12.0, 12.0f);

	//Tooltip
	ToolTipText = sf::Text(GD_NULLSTR, Fonts::font_BOGSTD, 10);

	//IO Tip
	IOInfoIText = sf::Text(GD_NULLSTR, Fonts::font_COMMS, 10);
	IOInfoOText = sf::Text(GD_NULLSTR, Fonts::font_COMMS, 10);
	IOInfoNText = sf::Text(GD_NULLSTR, Fonts::font_COMMS, 11);

	//Finishing touches to graphics
	RecalibrateUI(ScrW, ScrH);

	//Debug
	txt_info = new sf::Text();
	txt_info->SetColor(sf::Color(240, 240, 240, 196));
	txt_info->SetCharacterSize(12);
	txt_info->SetPosition(2.0f, 22.0f);
}

void GameState::RecalibrateUI(int w, int h)
{
	//Backdrop
	shp_bg_top = sf::Shape::Rectangle( sf::FloatRect( 0.0f,0.0f , ScrW+0.0f,48.0f ), sf::Color(0,0,0,0) );
	shp_bg_top.SetPointColor(0, sf::Color(0,0,0,48));
	shp_bg_top.SetPointColor(1, sf::Color(0,0,0,48));
	shp_bg_top.SetPointColor(2, sf::Color(0,0,0,0));
	shp_bg_top.SetPointColor(3, sf::Color(0,0,0,0));
	shp_bg_btm = sf::Shape::Rectangle( sf::FloatRect( 0.0f,ScrH-48.0f , ScrW+0.0f,48.0f ), sf::Color(0,0,0,0) );
	shp_bg_btm.SetPointColor(0, sf::Color(0,0,0,0));
	shp_bg_btm.SetPointColor(1, sf::Color(0,0,0,0));
	shp_bg_btm.SetPointColor(2, sf::Color(0,0,0,48));
	shp_bg_btm.SetPointColor(3, sf::Color(0,0,0,48));
	shp_bg_lft = sf::Shape::Rectangle( sf::FloatRect( 0.0f,0.0f , 48.0f,ScrH+0.0f ), sf::Color(0,0,0,0) );
	shp_bg_lft.SetPointColor(0, sf::Color(0,0,0,48));
	shp_bg_lft.SetPointColor(1, sf::Color(0,0,0,0));
	shp_bg_lft.SetPointColor(2, sf::Color(0,0,0,0));
	shp_bg_lft.SetPointColor(3, sf::Color(0,0,0,48));
	shp_bg_rgt = sf::Shape::Rectangle( sf::FloatRect( ScrW-48.0f,0.0f , 48.0f,ScrH+0.0f ), sf::Color(0,0,0,0) );
	shp_bg_rgt.SetPointColor(0, sf::Color(0,0,0,0));
	shp_bg_rgt.SetPointColor(1, sf::Color(0,0,0,48));
	shp_bg_rgt.SetPointColor(2, sf::Color(0,0,0,48));
	shp_bg_rgt.SetPointColor(3, sf::Color(0,0,0,0));

	//Console
	Console->Resize(w, h);

	//Menu
	Menu->Resize(w, h);
}

void GameState::OnExit()
{
	AddonReader::KillSingleton();

	delete m_img;
	delete m_spr;
	delete mw_img;
	delete mw_spr;
	delete txt_info;

	RemoveComponent(TreeContext);
	RemoveComponent(ContextComponent);
	RemoveComponent(ContextInputs);
	RemoveComponent(ContextOutputs);
	RemoveComponent(ContextInputs_Unwire);
	RemoveComponent(ContextOutputs_Unwire);
	RemoveComponent(EditWindow);

	delete TreeContext;
	delete ContextComponent;
	delete ContextInputs;
	delete ContextOutputs;
	delete ContextInputs_Unwire;
	delete ContextOutputs_Unwire;
	delete EditWindow;

	RemoveComponent(WindowLoad);
	delete WindowLoad;
	//delete WindowLoad_exp;
	
	RemoveComponent(WindowSave);
	delete WindowSave;
	//delete WindowSave_txtFile;
	//delete WindowSave_chkSaveOutVals;

	RemoveComponent(WindowContinue);
	delete WindowContinue;
	//delete WindowContinue_Label;

	RemoveComponent(WindowOptions);
	delete WindowOptions;
	//delete WindowOptions_BatterySaver_icon;
	delete WindowOptions_BatterySaver_iconimg;
	//delete WindowOptions_GridSize;

	RemoveComponent(WindowAbout);
	delete WindowAbout;
	//delete WindowAbout_Label;
}

void GameState::DrawLoading()
{
#define rw Renderer::Ref()
	rw->Clear();
	
	sf::Image img;
	img.LoadFromFile(GD_RES_SPLASH);
	
	sf::Sprite spr(img);
	spr.SetScale(static_cast<float>(rw->GetWidth())/img.GetWidth(), static_cast<float>(rw->GetHeight())/img.GetHeight());

	rw->Draw(spr);

	rw->Display();
#undef rw
}

void GameState::LoadUserSettings(const char* filename)
{
	TiXmlDocument doc;
	doc.LoadFile(filename);

	if(!doc.Error())
	{
		TiXmlElement* settings = doc.FirstChildElement(GD_XML_SETTINGS_ROOT);

		if(settings)
		{
			TiXmlElement* set = settings->FirstChildElement(GD_XML_SETTINGS_ITEM);

			if(set) do
			{
				std::string name = set->Attribute(GD_XML_SETTINGS_ATT_NAME);

				if(name == GD_XML_SETTINGS_ITEM_BATTERYSAVER)
				{
					int val = 0;
					set->Attribute(GD_XML_SETTINGS_ATT_VALUE, &val);
					switch (val)
					{
						default: BatteryMode = BATTERYMODE_OFF; break;
						case 1: BatteryMode = BATTERYMODE_ON; break;
						case 2: BatteryMode = BATTERYMODE_AUTO; break;
					}
				}
				else if(name == GD_XML_SETTINGS_ITEM_GRIDSIZE)
				{
					set->Attribute(GD_XML_SETTINGS_ATT_VALUE, &GridSize);

					if(GridSize > 64)
						GridSize = 64;
					else if(GridSize < 2)
						GridSize = 2;
				}
				else if(name == GD_XML_SETTINGS_ITEM_SCREENW)
				{
					int val = 0;
					set->Attribute(GD_XML_SETTINGS_ATT_VALUE, &val);
					ScrW = val;

					if( ScrW < 0 ) ScrW = 0;
				}
				else if(name == GD_XML_SETTINGS_ITEM_SCREENH)
				{
					int val = 0;
					set->Attribute(GD_XML_SETTINGS_ATT_VALUE, &val);
					ScrH = val;

					if( ScrH < 0 ) ScrH = 0;
				}
				else if(name == GD_XML_SETTINGS_ITEM_SCREENFULL)
				{
					int val = 0;
					set->Attribute(GD_XML_SETTINGS_ATT_VALUE, &val);
					ScrFullscreen = (val!=0);
				}
			}
			while((set = set->NextSiblingElement(GD_XML_SETTINGS_ITEM)) != NULL);
		}
	}
}

// #define USE_OLD_WIRING_ENGINE

#ifdef USE_OLD_WIRING_ENGINE

void GameState::OnTick(float dt)
{
	// Garbage collection
	for(int i = 0; i < queue_comp_delete.size(); ++i)
	{
		queue_comp_delete[i]->luaCallObjectFunction(GD_LUA_FNAME_ONREMOVE);
		lua_close(queue_comp_delete[i]->L);
		queue_comp_delete.erase(queue_comp_delete.begin() + i);
	}

	bool finished, callevent;
	LuaComponent* c;

	// Reset update state
	for(int i = 0, sz = luacomps.size(); i < sz; ++i)
	{
		luacomps[i]->_doneupdating = false;
	}

	// Send updated output ports to inputs
	do
	{
		finished = true;

		// Check for updated outports
		for(int i = 0, sz = luacomps.size(); i < sz; ++i)
		{
			c = luacomps[i];

			// Collect all ports that need updating
			for(int iport = 0, iport_sz = c->InputPorts.size(); iport < iport_sz; ++iport)
			{
				Opposition& opp = c->InputPorts[iport].opposing;

				if(opp.comp == NULL)
					continue; // Not connected...

				c->_inputneedsupdating[iport] = opp.comp->OutputPorts[opp.port]._outputtriggered;
			}
		}

		// Reset output triggered states
		for(int i = 0, sz = luacomps.size(); i < sz; ++i)
		{
			c = luacomps[i];

			for(int oport = 0, oport_sz = c->OutputPorts.size(); oport < oport_sz; ++oport)
			{
				c->OutputPorts[oport]._outputtriggered = false;
			}
		}

		// See if components can call onInputsTriggered()
		for(int i = 0, sz = luacomps.size(); i < sz; ++i)
		{
			c = luacomps[i];

			// Check if we need to call input event
			callevent = false;
			for(int iport = 0, iport_sz = c->_inputneedsupdating.Size(); iport < iport_sz; ++iport)
			{
				if(c->_inputneedsupdating[iport])
				{
					callevent = true;
					if(!c->_doneupdating)
						finished = false;
					break;
				}
			}
			
			// Perform onInputsTriggered() event
			if(callevent)
			{
				c->luaCallObjectFunction(GD_LUA_FNAME_ONINPUTSTRIGGER);

				for(int iport = 0, iport_sz = c->_inputneedsupdating.Size(); iport < iport_sz; ++iport)
				{
					// Reset input "needs update" state
					c->_inputneedsupdating[iport] = false;
				}

				c->_doneupdating = true;
			}
		}

		// Loop back and retry because some objects probably need another update because of input triggers
	}
	while(!finished);

	++ITER;
	TICK = dt;

	if(BatteryMode)
		Sleep(16);
}

#else

#pragma region Unedited engine
/*
void GameState::OnTick(float dt)
{
	LuaComponent* c;


	// Delete any components queued for deletion

	for(int i = 0, sz = queue_comp_delete.size(); i < sz; ++i)
	{
		c = queue_comp_delete[i];

		// Call remove Lua event
		c->luaCallObjectFunction(GD_LUA_FNAME_ONREMOVE);

		// Terminate Lua VM for the component
		lua_close(c->L);

		queue_comp_delete.erase(queue_comp_delete.begin() + i);
	}


	// Reset some states

	for(int i = 0, sz = luacomps.size(); i < sz; ++i)
	{
		c = luacomps[i];

		// Reset update state
		c->_doneupdating = false;
		c->_ischanged = false;
	}
	

	// Notify components if they are going to be changed (and find dead-ends that need updating)

	if(updatestack.Size())
		updatestack.Clear();

	for (int i = 0, sz = luacomps.size(); i < sz; ++i) // Collect components that triggered any outputs
	{
		c = luacomps[i];

		if (c->_outputtriggered)
		{
			updatestack.Push( Opposition(c) );
			c->_outputtriggered = false;
		}
	}
	while (!updatestack.Empty()) // Set "changed" flags and look for dead-end components
	{
		c = updatestack.Top().comp;
		updatestack.Pop();

		c->_ischanged = true;
		c->_deadend = true;

		for (int op = 0, op_sz = c->OutputPorts.size(); op < op_sz; ++op)
		{
			OutPort& outp = c->OutputPorts[op];

			if (int sz = outp.opposing_list.size())
			{
				c->_deadend = false;

				for (int i = 0; i < sz; ++i)
				{
					updatestack.Push( Opposition(outp.opposing_list[i].comp) );
				}
			}
		}
	}


	// Collect dead-ends

	if(deadendlist.size())
		deadendlist.clear();

	for (int i = 0, sz = luacomps.size(); i < sz; ++i)
	{
		c = luacomps[i];

		if (c->_deadend || c->_forcedeadend )
		{
			deadendlist.push_back(c);

			c->_deadend = false;
			c->_forcedeadend = false;
		}
	}


	// Update/Execute components

	for (int i = 0, sz = deadendlist.size(); i < sz; ++i)
	{
		// Dead-End is last of the stack
		updatestack.Push( Opposition(deadendlist[i], 0) );

		while_wiresys_stack:  // Oh no, a goto!!!!!! sue me
		while (!updatestack.Empty())
		{

			Opposition& top = updatestack.Top();

			if (top.comp->_doneupdating)
			{
				updatestack.Pop();
				continue;
			}

			// Push and recurse components that are connect to our input(s)

			for (int ip_sz = top.comp->InputPorts.size(); top.port < ip_sz; ++top.port)
			{
				InPort& inp = top.comp->InputPorts[top.port];

				if (inp.opposing.comp) // Check if we have a connection in this input
				{
					if (!inp.opposing.comp->_doneupdating) // Check if it already updated... No need to update again if it already did this tick
					{
						// Found a connection, push component to top of stack and recurse
						++top.port;
						updatestack.Push( Opposition(inp.opposing.comp, 0) );
						goto while_wiresys_stack;
					}
				}
			}

			// Execute component

			top.comp->luaCallObjectFunction(GD_LUA_FNAME_ONINPUTSTRIGGER);
			for(int iport = 0, iport_sz = top.comp->_inputneedsupdating.Size(); iport < iport_sz; ++iport)
			{
				// Reset input "needs update" state
				top.comp->_inputneedsupdating[iport] = false;
			}
			top.comp->_doneupdating = true;
			top.comp->_ischanged = false;
			top.comp->_outputtriggered = false;

			// We're done with this component!
			updatestack.Pop();

		} // updatestack while

	} // for each dead-end


	++ITER;
	TICK = dt;

	if(BatteryMode)
		Sleep(16);
}
*/
#pragma endregion

void GameState::OnTick(float dt)
{
	// Do UI checks
	PerformUIChecks();

	LuaComponent* c;
	bool autosleep = true;

	/* *** Free any components queued for deletion *** */

	for(int i = 0, sz = queue_comp_delete.size(); i < sz; ++i)
	{
		c = queue_comp_delete[i];

		// Call remove Lua event
		c->luaCallObjectFunction(GD_LUA_FNAME_ONREMOVE);

		// Terminate Lua VM for the component.
		// This also frees the component, so there is no need for "delete c;"
		lua_close(c->L);

		queue_comp_delete.erase(queue_comp_delete.begin() + i);
	}

	/* *** Notify components if they are going to be changed (and find dead-ends that need updating) *** */

	if(pathstack.Size())
		pathstack.Clear();

	for (int i = 0, sz = luacomps.size(); i < sz; ++i)
	{
		c = luacomps[i];

		// Reset update states
		c->_doneupdating = false;
		c->_ischanged = false;
		c->_startend = false;
		c->_forcedeadend = false;
		c->_loopgate = false;
		c->_pathstackinit = false;

		// Collect components that triggered any outputs
		if (c->_outputtriggered)
		{
			pathstack.Push( pathstack_s(c) );
			c->_outputtriggered = false;
			c->_startend = true;
			autosleep = false;
		}
	}

	// Set "changed" flags and look for dead-end components
while_wiresys_pathstack:
	while (!pathstack.Empty())
	{
		pathstack_s& top = pathstack.Top();

		if (!top.comp->_pathstackinit)
		{
			top.comp->_ischanged = true;
			top.comp->_deadend = true;

			top.comp->_pathstackinit = true;
		}
		
		for (int op_sz = top.comp->OutputPorts.size(); top.out_p < op_sz; ++top.out_p)
		{
			OutPort& outp = top.comp->OutputPorts[top.out_p];

			// If it has outputs connected, it's not a dead end
			if (int sz = outp.opposing_list.size())
			{
				if (!top.comp->_forcedeadend)
					top.comp->_deadend = false;

				// Send all outputs to the stack
				for (; top.opp_p < sz; ++top.opp_p)
				{
					Opposition& opp = outp.opposing_list[top.opp_p];
					
					if (!opp.comp->_ischanged) // Check if the change state was already set this tick
					{
						++top.opp_p;
						pathstack.Push( pathstack_s(opp.comp) );
						goto while_wiresys_pathstack;
					}
					else
					{
						// Check if opposing component is already in the path
						//if (opp.comp->_looptest)
						//if (pathstack.CountOf(top.comp) > 1)
						for (int i = 0, i_sz = pathstack.Size(); i < i_sz; ++i)
						{
							pathstack_s& i_ref = pathstack.Get(i);

							if (!i_ref.comp->_pathstackinit)
								continue;

							if (i_ref.comp == opp.comp) // If opposing component is already in path
							{
								top.comp->_loopgate = true;
								top.comp->_deadend = true;
								top.comp->_forcedeadend = true;
								//top.comp->_needsupdatenexttick = true;
								opp.comp->_needsupdatenexttick = true;
								break;
							}
						}
						
					}

				}
			}

		}

		pathstack.Pop();
	}


	/* *** Collect dead-ends *** */

	if(deadendlist.size())
		deadendlist.clear();

	for (int i = 0, sz = luacomps.size(); i < sz; ++i)
	{
		c = luacomps[i];

		if (c->_deadend)
		{
			// Add to list
			deadendlist.push_back(c);

			// Reset dead-end state for next tick
			c->_deadend = false;
		}
	}

	/* *** Update/Execute components *** */

	if(updatestack.Size())
		updatestack.Clear();

	for (int i = 0, sz = deadendlist.size(); i < sz; ++i) // (DFS)
	{
		// Dead-End is last of the stack
		updatestack.Push( Opposition(deadendlist[i], 0) );

while_wiresys_updatestack:

		while (!updatestack.Empty())
		{
			Opposition& top = updatestack.Top();

			if (top.comp->_doneupdating)
			{
				updatestack.Pop();
				continue;
			}

			// Push and recurse components that are connect to our input(s) (DFS)

			for (int ip_sz = top.comp->InputPorts.size(); top.port < ip_sz; ++top.port)
			{
				InPort& inp = top.comp->InputPorts[top.port];
				
				if (inp.opposing.comp) // Check if we have a connection in this input
				{
					/*if (top.comp == inp.opposing.comp)
					{
						top.comp->_outputtriggered = true;
						break;
					}*/

					if (!inp.opposing.comp->_doneupdating)  // Check if it already updated... No need to update again if it already did this tick
					{
						if (!inp.opposing.comp->_loopgate)
						{
							// Found a connection, push component to top of stack and recurse
							++top.port;
							updatestack.Push( Opposition(inp.opposing.comp, 0) );
							goto while_wiresys_updatestack;
						}
					}
				}
			}
			// No [more] input components that need to update

			// Execute component

			top.comp->luaCallObjectFunction(GD_LUA_FNAME_ONINPUTSTRIGGER);

			for(int iport = 0, iport_sz = top.comp->_inputneedsupdating.Size(); iport < iport_sz; ++iport)
			{
				// Reset input "needs update" state
				top.comp->_inputneedsupdating[iport] = false;
			}

			if (!top.comp->_needsupdatenexttick)
			{
				top.comp->_outputtriggered = false;
			}
			else
			{
				top.comp->_needsupdatenexttick = false;
			}

			top.comp->_doneupdating = true;

			// We're done with this component!
			updatestack.Pop();

		} // updatestack while

	} // for each dead-end


	++ITER;
	TICK = dt;

	if (BatteryMode == BATTERYMODE_AUTO)
		BatterySaver = autosleep;
	else if (BatteryMode == BATTERYMODE_ON)
		BatterySaver = true;
	else
		BatterySaver = false;
}

#endif


#define rw Renderer::Ref()
void GameState::OnDraw()
{
	rw->SetActive();
	rw->Clear(sf::Color( 64, 64, 64 ));

	// Draw backdrop
	rw->Draw(shp_bg_top);
	rw->Draw(shp_bg_btm);
	rw->Draw(shp_bg_lft);
	rw->Draw(shp_bg_rgt);

	// Draw tools
	if(EditMode)
	{
		if(Wiring)
		{
			// Draw wiring points
			glPushMatrix();
			glTranslatef(static_cast<float>(Pan.X), static_cast<float>(Pan.Y), 0.0);
			glBegin(GL_QUADS);
			glColor3f(0.95f, 0.95f, 0.06f);
			for(int i = 0, sz = WiringPoints.size(); i < sz; ++i)
			{
				float x, y;
				x = WiringPoints[i].X;
				y = WiringPoints[i].Y;

				glVertex2f(x - 3.0f, y);
				glVertex2f(x, y - 3.0f);
				glVertex2f(x + 3.0f, y);
				glVertex2f(x, y + 3.0f);
			}
			glEnd();
			glPopMatrix();
		}
	}

	glPushMatrix();
	if(GridMode)
	{
		// Draw grid

		int mx = Renderer::Ref()->GetInput().GetMouseX()-GridPan.X,
			my = Renderer::Ref()->GetInput().GetMouseY()-GridPan.Y;
		ConvertToGridPos(mx, my);

		glLineWidth(1.0f);
		glTranslatef(static_cast<float>(GridPan.X), static_cast<float>(GridPan.Y), 0.0);
		glBegin(GL_LINES);

			if(EditMode)
				glColor4f(0.0f, 0.0f, 0.0f, 0.45f);
			else
				glColor4f(0.0f, 0.0f, 0.0f, 0.3f);

			for(int x = 0, width = Renderer::Ref()->GetWidth(); x < width; x += GridSize)
			{
				glVertex2i(x, -GridSize);
				glVertex2i(x, Renderer::Ref()->GetHeight());
			}
			for(int y = 0, height = Renderer::Ref()->GetHeight(); y < height; y += GridSize)
			{
				glVertex2i(-GridSize, y);
				glVertex2i(Renderer::Ref()->GetWidth(), y);
			}

			if(EditMode)
			{
				glColor4f(0.6667f, 0.85f, 0.925f, 0.75f);
				glVertex2i(mx - GridSize, my);
				glVertex2i(mx + GridSize, my);
				glVertex2i(mx, my - GridSize);
				glVertex2i(mx, my + GridSize);
			}
		glEnd();
	}
	glPopMatrix();

	// Draw wiring
	glPushMatrix();
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	for(int i = 0, sz = luacomps.size(); i < sz; ++i)
	{
		LuaComponent* c = luacomps[i];

		for(int w = 0, w_sz = c->InputPorts.size(); w < w_sz; ++w)
		{
			InPort& myinport = c->InputPorts[w];
			if(myinport.opposing.port != -1)
			{
				LuaComponent* opp_c = c->InputPorts[w].opposing.comp;
				OutPort& oppoutport = opp_c->OutputPorts[myinport.opposing.port];

				switch(oppoutport.draw_state)
				{
				default:
				case DRAW_STATE_IDLE:
					glColor3f(0.06f, 0.85f, 0.06f);
					break;
				case DRAW_STATE_TRIGGERED:
					glColor3f(0.9f, 1.0f, 0.9f);
					break;
				case DRAW_STATE_ERROR:
					glColor3f(0.9f, 0.15f, 0.06f);
					break;
				}

				// Wire beginning
				sys::Vectori* point;
				glVertex2i(static_cast<int>(myinport.x + c->Position.X), static_cast<int>(myinport.y + c->Position.Y));
				for(int p = c->InputWirePoints[w].Size() - 1; p >= 0; --p)
				{
					// Wire path point (needs duplicate glVertex2i() to connect them)
					point = &c->InputWirePoints[w][p];
					glVertex2i( point->X + Pan.X, point->Y + Pan.Y );
					glVertex2i( point->X + Pan.X, point->Y + Pan.Y );
				}
				glVertex2i(static_cast<int>(oppoutport.x + opp_c->Position.X), static_cast<int>(oppoutport.y + opp_c->Position.Y));
				// Wire end

				/*rw->Draw(sf::Shape::Line(
					static_cast<float>(myinport.x + c->Position.X), static_cast<float>(myinport.y + c->Position.Y),
					static_cast<float>(oppoutport.x + opp_c->Position.X), static_cast<float>(oppoutport.y + opp_c->Position.Y),
					1.0f,
					sf::Color(16, 215, 16)
				));*/
			}
		}
	}
	glEnd();
	glPopMatrix();

	//Reset wire draw states
	for(int i = 0, sz = luacomps.size(); i < sz; ++i)
	{
		LuaComponent* c = luacomps[i];

		for(int j = 0, jsz = c->OutputPorts.size(); j < jsz; ++j)
		{
			OutPort& outport = c->OutputPorts[j];
			switch(outport.draw_state)
			{
				case DRAW_STATE_TRIGGERED:
					outport.draw_state = DRAW_STATE_IDLE;
					break;
				case DRAW_STATE_ERROR:
					// oppoutport.draw_state = DRAW_STATE_IDLE;
					break;
			}
		}
	}

	//Debug
	if(ShowDebug)
	{
		std::stringstream info;
		info << GD_DBG_FPS << 1000.0 / (Renderer::Ref()->GetFrameTime() * 1000.0) << GD_NEWLINE;
		info << GD_DBG_TICKMS << TICK << GD_NEWLINE;
		info << GD_DBG_TICKNUM << ITER << GD_NEWLINE;
		PROCESS_MEMORY_COUNTERS pmc; GetProcessMemoryInfo( GetCurrentProcess(), &pmc, sizeof(pmc) );
		info << GD_DBG_WORKINGSET << (pmc.WorkingSetSize / 1024.0) / 1024.0 << GD_NEWLINE;
		info << GD_DBG_SCREENPOS << "X:" << Pan.X << " Y:" << Pan.Y << GD_NEWLINE;
		txt_info->SetString(info.str());
		rw->Draw(*txt_info);
	}
}

void GameState::OnPostFXDraw()
{
	if(HoverComponent)
	{
		//Draw tooltip
		if(!HoverComponent->Tooltip.empty())
		{
			ToolTipText.SetString(HoverComponent->Tooltip);
			ToolTipText.SetPosition( static_cast<float>(sys::Engine::Ref().GetMouseX()), static_cast<float>(sys::Engine::Ref().GetMouseY() - 4) );

			sf::FloatRect r = ToolTipText.GetRect();
			ToolTipText.Move(0.0, -r.Height);

			r.Left -= 2;
			r.Top -= 2 + r.Height;
			r.Width += 4;
			r.Height += 4;
			rw->Draw(sf::Shape::Rectangle( r, sf::Color(96, 96, 96, 225), 1.0f, sf::Color(16, 16, 16, 128) ));
			rw->Draw(ToolTipText);
		}

		//Draw IO Info (Sampler)
		if(EditMode)
		{
			// this code is rushed... it looks awful
			float offsetx = 0.0f, offsety = 22.0f;
			float height = max(HoverComponent->InputPorts.size(), HoverComponent->OutputPorts.size())*11.0f + 34.0f;
			if(HoverComponent->Position.X < rw->GetWidth()/2)
			{
				offsetx = rw->GetWidth() - 256.0f;
			}
			if(HoverComponent->Position.Y < rw->GetHeight()/2)
			{
				offsety = rw->GetHeight() - height - 22.0f;
			}
			rw->Draw(sf::Shape::Rectangle(sf::FloatRect(offsetx, offsety, 256.0f, height), sf::Color(0, 0, 0, 164), 1.0f, sf::Color(0, 0, 0, 196)));

			sys::String s;

			IOInfoNText.SetPosition(2.0f + offsetx, 2.0f + offsety);
			s << HoverComponent->myaddonitem->p_category->name << " - " << HoverComponent->myaddonitem->name;
			IOInfoNText.SetString(s.C_Str());
			rw->Draw(IOInfoNText);

			IOInfoIText.SetPosition(2.0f + offsetx, 18.0f + offsety);
			s.Clear();
			s << "Inputs: " << HoverComponent->InputPorts.size();
			IOInfoIText.SetString(s.C_Str());
			IOInfoIText.SetColor(sf::Color(196, 196, 196));
			IOInfoIText.SetStyle(sf::Text::Underlined);
			rw->Draw(IOInfoIText);
			IOInfoIText.SetPosition(2.0f + offsetx, 20.0f + offsety);
			IOInfoIText.SetColor(sf::Color::White);
			IOInfoIText.SetStyle(sf::Text::Regular);
			s = GD_NULLSTR;
			for(int i = 0; i < HoverComponent->InputPorts.size(); ++i)
			{
				InPort& inp = HoverComponent->InputPorts[i];
				s << GD_NEWLINE << inp.port_name;
				if(inp.opposing.comp)
				{
					s << ": ";
					if(inp.opposing.comp->OutputPorts[inp.opposing.port].val_type == TYPE_NUMBER)
						s << inp.opposing.comp->OutputPorts[inp.opposing.port].val_num;
					else
						s << inp.opposing.comp->OutputPorts[inp.opposing.port].val_str;
				}
			}
			IOInfoIText.SetString(s.C_Str());
			rw->Draw(IOInfoIText);

			IOInfoOText.SetPosition(130.0f + offsetx, 18.0f + offsety);
			s.Clear();
			s << "Outputs: " << HoverComponent->OutputPorts.size();
			IOInfoOText.SetString(s.C_Str());
			IOInfoOText.SetColor(sf::Color(196, 196, 196));
			IOInfoOText.SetStyle(sf::Text::Underlined);
			rw->Draw(IOInfoOText);
			IOInfoOText.SetPosition(130.0f + offsetx, 20.0f + offsety);
			IOInfoOText.SetColor(sf::Color::White);
			IOInfoOText.SetStyle(sf::Text::Regular);
			s = GD_NULLSTR;
			for(int i = 0; i < HoverComponent->OutputPorts.size(); ++i)
			{
				OutPort& outp = HoverComponent->OutputPorts[i];
				s << GD_NEWLINE << outp.port_name << ": ";
				if(outp.val_type == TYPE_NUMBER)
					s << outp.val_num;
				else
					s << outp.val_str;
			}
			IOInfoOText.SetString(s.C_Str());
			rw->Draw(IOInfoOText);
		}
	}

	//Draw Mouse
	rw->Draw(*m_spr);

	//Draw mouse wrench in edit mode
	if(EditMode)
		rw->Draw(*mw_spr);
}

void GameState::OnResize(int w, int h)
{
	ScrW = w;
	ScrH = h;

	rw->SetView( sf::View( sf::FloatRect(0.0f, 0.0f, (float)w, (float)h) ) );
	
	RecalibrateUI(w, h);
}
#undef rw

void GameState::OnMouseMove(int x, int y, int dx, int dy)
{
	//update mouse sprite position
	m_spr->Move(static_cast<float>(dx), static_cast<float>(dy));
	mw_spr->Move(static_cast<float>(dx), static_cast<float>(dy));

	if(EditMode)
	{
		if(DragComponent)
		{
			// move dragging component
			int mx = x, my = y;
			ConvertToGridPos(mx, my);
			DragComponent->Position.X = mx - DragOffset.X;
			DragComponent->Position.Y = my - DragOffset.Y;
		}
	}

	if(Panning)
	{
		// pan
		
		Pan.X += dx;
		Pan.Y += dy;
		GridPan.X += dx;
		GridPan.Y += dy;
		while(GridPan.X > GridSize) GridPan.X -= GridSize;
		while(GridPan.Y > GridSize) GridPan.Y -= GridSize;
		while(GridPan.X <= 0) GridPan.X += GridSize;
		while(GridPan.Y <= 0) GridPan.Y += GridSize;
		for(int i = 0; i < luacomps.size(); ++i)
		{
			LuaComponent* c = luacomps[i];
			c->Position.X += dx;
			c->Position.Y += dy;
		}
	}
}

void GameState::OnMouseDown(int x, int y, unsigned button)
{
}

void GameState::OnMouseUp(int x, int y, unsigned button)
{
	if(button == MOUSEBUTTON_LEFT)
	{
		if(DragComponent)
		{
			// End drag
			DragComponent = NULL;
		}

		if(Panning)
		{
			// End panning
			Panning = false;
		}
	}
}

void GameState::OnMouseClicked(int x, int y, unsigned button)
{
	if(EditMode)
	{
		// On right click, open Tree
		if(button == MOUSEBUTTON_RIGHT)
		{
			if(Wiring)
			{
				// create wiring point
				int gx = x - Pan.X,
					gy = y - Pan.Y;
				ConvertToGridPos(gx, gy);
				WiringPoints.push_back( sys::Vectori(gx, gy) );
			}
			else
			{
				// Set the tree's positioning
				TreeContext->Position.X = x;
				TreeContext->Position.Y = y;
				TreeContext->PlaceComponentPos.X = x;
				TreeContext->PlaceComponentPos.Y = y;

				// Check if the tree falls outside of the screen
				if(TreeContext->Position.X + TreeContext->Size.X >= Renderer::Ref()->GetWidth())
				{
					TreeContext->Position.X = Renderer::Ref()->GetWidth()-1 - TreeContext->Size.X;
				}
				if(TreeContext->Position.Y + TreeContext->Size.Y >= Renderer::Ref()->GetHeight())
				{
					TreeContext->Position.Y = Renderer::Ref()->GetHeight()-1 - TreeContext->Size.Y;
				}

				// Open tree component
				if(!TreeContextOpened)
				{
					TreeContext->ReadyToClose = false;
					AddComponent(TreeContext);

					TreeContextOpened = true;

					// Remove context components if they're open
					RemoveComponent(ContextComponent);
					RemoveComponent(ContextInputs);
					RemoveComponent(ContextOutputs);
					RemoveComponent(ContextInputs_Unwire);
					RemoveComponent(ContextOutputs_Unwire);
				}
			}
		}
		// Close tree if left click
		else if(button == MOUSEBUTTON_LEFT)
		{
			TreeContextOpened = false;
			//context_state = 0;
			RemoveComponent(TreeContext);
			RemoveComponent(ContextComponent);
			RemoveComponent(ContextInputs);
			RemoveComponent(ContextOutputs);
			RemoveComponent(ContextInputs_Unwire);
			RemoveComponent(ContextOutputs_Unwire);
		}
	}

	if(button == MOUSEBUTTON_LEFT)
	{
		Panning = true;
	}
}

void GameState::OnMouseUnclicked(int x, int y, unsigned button)
{
	
}

void GameState::OnMouseClickedComponent(int x, int y, unsigned button, sys::Component* comp)
{
	if(EditMode)
	{
		if(button == MOUSEBUTTON_LEFT)
		{
			if(*(ui::UI_DATATYPE*)(comp->UserData) == ui::UI_LUACOMPONENT)
			{
				// Start drag
				DragComponent = (LuaComponent*)comp;

				DragOffset.X = (x - comp->Position.X);
				DragOffset.Y = (y - comp->Position.Y);
				ConvertToGridPos(DragOffset.X, DragOffset.Y);
				DragOffset.X -= GridPan.X;
				DragOffset.Y -= GridPan.Y;
			}
			else
			{
				// An item was clicked
				if(comp == TreeContext)
				{
					if(TreeContext->ReadyToClose && TreeContext->ItemSelected!=NULL)
					{
						this->RemoveComponent(TreeContext);

						int gx = TreeContext->PlaceComponentPos.X,
							gy = TreeContext->PlaceComponentPos.Y;
						ConvertToGridPos(gx, gy);
						if(GridMode)
						{
							gx += GridPan.X;
							gy += GridPan.Y;
						}

						// Create new component
						LuaComponent* c = CreateComponent(TreeContext->ItemSelected, gx, gy);
						this->InsertComponent(c, 0);
						luacomps.push_back(c);

						if(!c->error)
							Console->Print(ui::UIConsole::MSG_TINFO, GD_MSG_CREATEDCOMPONENT, c->myaddonitem->name.c_str());

						TreeContextOpened = false;
					}
				}

				else if(comp == ContextComponent)
				{
					bool closeContext = true;

					if(ContextComponent->ItemClicked == ui::UICONTEXTITEM_INIT_WIREINPUT)
					{
						//wire in->out
						context_state = 0;

						if(ContextComponent->comp_ref->InputPorts.size() != 0)
						{
							if(ContextComponent->comp_ref->InputPorts.size() == 1)
							{
								context_state = 2;
								StartWiring();
								ContextInputs->comp_ref = ContextComponent->comp_ref;
								ContextInputs->ItemClicked = 0;
							}
							else
							{
								ContextInputs->UpdateList(ContextComponent->comp_ref);
								ContextInputs->Position = ContextComponent->Position;

								// Check if the context falls outside of the screen
								if(ContextInputs->Position.X + ContextInputs->Size.X >= Renderer::Ref()->GetWidth())
								{
									ContextInputs->Position.X = Renderer::Ref()->GetWidth()-1 - ContextInputs->Size.X;
								}
								if(ContextInputs->Position.Y + ContextInputs->Size.Y >= Renderer::Ref()->GetHeight())
								{
									ContextInputs->Position.Y = Renderer::Ref()->GetHeight()-1 - ContextInputs->Size.Y;
								}

								this->AddComponent(ContextInputs);
								ContextInputs->selected_item = 0;
								ContextInputs->mouse_inside = false;
							}
						}
						else closeContext = false;
					}
					else if(ContextComponent->ItemClicked == ui::UICONTEXTITEM_INIT_WIREOUTPUT)
					{
						//wire out->in
						context_state = 0;

						if(ContextComponent->comp_ref->OutputPorts.size() != 0)
						{
							if(ContextComponent->comp_ref->OutputPorts.size() == 1)
							{
								context_state = 1;
								StartWiring();
								ContextOutputs->comp_ref = ContextComponent->comp_ref;
								ContextOutputs->ItemClicked = 0;
							}
							else
							{
								ContextOutputs->UpdateList(ContextComponent->comp_ref);
								ContextOutputs->Position = ContextComponent->Position;

								// Check if the context falls outside of the screen
								if(ContextOutputs->Position.X + ContextOutputs->Size.X >= Renderer::Ref()->GetWidth())
								{
									ContextOutputs->Position.X = Renderer::Ref()->GetWidth()-1 - ContextOutputs->Size.X;
								}
								if(ContextOutputs->Position.Y + ContextOutputs->Size.Y >= Renderer::Ref()->GetHeight())
								{
									ContextOutputs->Position.Y = Renderer::Ref()->GetHeight()-1 - ContextOutputs->Size.Y;
								}

								this->AddComponent(ContextOutputs);
								ContextOutputs->selected_item = 0;
								ContextOutputs->mouse_inside = false;
							}
						}
						else closeContext = false;
					}
					else if(ContextComponent->ItemClicked == ui::UICONTEXTITEM_INIT_EDIT)
					{
						//edit box
						context_state = 0;

						EditWindow->Position = ContextComponent->Position;
						EditWindow->Show(ContextComponent->comp_ref);

						//check if the box falls outside of the screen
						if(EditWindow->Position.X + EditWindow->Size.X >= Renderer::Ref()->GetWidth())
						{
							//EditWindow->Position.X = Renderer::Ref()->GetWidth() - EditWindow->Size.X;
							EditWindow->Move(Renderer::Ref()->GetWidth()-1 - (EditWindow->Position.X + EditWindow->Size.X), 0);
						}
						if(EditWindow->Position.Y + EditWindow->Size.Y >= Renderer::Ref()->GetHeight())
						{
							//EditWindow->Position.Y = Renderer::Ref()->GetHeight() - EditWindow->Size.Y;
							EditWindow->Move(0, Renderer::Ref()->GetHeight()-1 - (EditWindow->Position.Y + EditWindow->Size.Y));
						}
					
						ContextComponent->comp_ref = NULL;
						ContextComponent->selected_item = 0;
						ContextComponent->mouse_inside = false;
					}
					else if(ContextComponent->ItemClicked == ui::UICONTEXTITEM_INIT_UNWIREINPUT)
					{
						context_state = 0;

						if(ContextComponent->comp_ref->InputPorts.size() != 0)
						{
							ContextInputs_Unwire->UpdateList(ContextComponent->comp_ref);
							ContextInputs_Unwire->Position = ContextComponent->Position;

							this->AddComponent(ContextInputs_Unwire);

							ContextInputs_Unwire->selected_item = 0;
							ContextInputs_Unwire->mouse_inside = false;
						}
						else closeContext = false;
					}
					else if(ContextComponent->ItemClicked == ui::UICONTEXTITEM_INIT_UNWIREOUTPUT)
					{
						context_state = 0;

						if(ContextComponent->comp_ref->OutputPorts.size() != 0)
						{
							ContextOutputs_Unwire->UpdateList(ContextComponent->comp_ref);
							ContextOutputs_Unwire->Position = ContextComponent->Position;

							this->AddComponent(ContextOutputs_Unwire);

							ContextOutputs_Unwire->selected_item = 0;
							ContextOutputs_Unwire->mouse_inside = false;
						}
						else closeContext = false;
					}
					else if(ContextComponent->ItemClicked == ui::UICONTEXTITEM_INIT_REMOVE)
					{
						context_state = 0;

						LuaComponent* c = ContextComponent->comp_ref;

						for(int i = 0; i < c->InputPorts.size(); ++i)
						{
							if(c->InputPorts[i].opposing.comp)
								c->UnconnectInput(i);
						}

						for(int i = 0; i < c->OutputPorts.size(); ++i)
						{
							c->UnconnectOutput(i);
						}

						for(int i = 0; i < luacomps.size(); ++i)
						{
							if(luacomps[i] == c)
								luacomps.erase(luacomps.begin() + i);
						}

						Console->Print(ui::UIConsole::MSG_TINFO, GD_MSG_REMOVEDCOMPONENT, c->myaddonitem->name.c_str());

						RemoveComponent(c);
						queue_comp_delete.push_back(c);

						ContextComponent->comp_ref = NULL;
						ContextComponent->selected_item = 0;
						ContextComponent->mouse_inside = false;
					}

					if(closeContext)
						this->RemoveComponent(ContextComponent);
				}

				else if(comp == ContextInputs)
				{
					this->RemoveComponent(ContextInputs);

					if(context_state == 1) //we selected an output, and now we selected an input. wire them together.
					{
						//wire out->in
						ContextOutputs->comp_ref->ConnectOutput(ContextInputs->comp_ref, ContextOutputs->ItemClicked, ContextInputs->ItemClicked);
						context_state = 0;

						Console->Print( ui::UIConsole::MSG_TINFO, GD_MSG_WIREDOUTPUT, //wired %s's output %s to %s's input %s
							ContextOutputs->comp_ref->myaddonitem->name.c_str(),
							ContextOutputs->comp_ref->OutputPorts[ContextOutputs->ItemClicked].port_name.c_str(),
							ContextInputs->comp_ref->myaddonitem->name.c_str(),
							ContextInputs->comp_ref->InputPorts[ContextInputs->ItemClicked].port_name.c_str() );

						sys::ArrayOf<sys::Vectori>& arr = ContextInputs->comp_ref->InputWirePoints[ContextInputs->ItemClicked];
						arr.Create(WiringPoints.size());
						for(int i = 0; i < WiringPoints.size(); ++i)
							arr[i] = WiringPoints[i];
						WiringPoints.clear();

						Wiring = false;

						lua_pushinteger(ContextInputs->comp_ref->L, ContextInputs->ItemClicked+1);
						ContextInputs->comp_ref->luaCallObjectFunction(GD_LUA_FNAME_ONINPUTCONNECT, 1, 0);
					}
					else
					{
						context_state = 2;
						StartWiring();
					}
				}

				else if(comp == ContextOutputs)
				{
					this->RemoveComponent(ContextOutputs);

					if(context_state == 2) //we selected an input, and now we selected an output. wire them together.
					{
						//wire in->out
						ContextInputs->comp_ref->ConnectInput(ContextOutputs->comp_ref, ContextInputs->ItemClicked, ContextOutputs->ItemClicked);
						context_state = 0;

						Console->Print( ui::UIConsole::MSG_TINFO, GD_MSG_WIREDINPUT, //wired %s's input %s to %s's output %s
							ContextInputs->comp_ref->myaddonitem->name.c_str(),
							ContextInputs->comp_ref->InputPorts[ContextInputs->ItemClicked].port_name.c_str(),
							ContextOutputs->comp_ref->myaddonitem->name.c_str(),
							ContextOutputs->comp_ref->OutputPorts[ContextOutputs->ItemClicked].port_name.c_str() );

						sys::ArrayOf<sys::Vectori>& arr = ContextInputs->comp_ref->InputWirePoints[ContextInputs->ItemClicked];
						arr.Create(WiringPoints.size());
						for(int i = WiringPoints.size()-1, j = 0; i >= 0; --i, ++j)
							arr[j] = WiringPoints[i];
						WiringPoints.clear();

						Wiring = false;

						lua_pushinteger(ContextInputs->comp_ref->L, ContextInputs->ItemClicked+1);
						ContextInputs->comp_ref->luaCallObjectFunction(GD_LUA_FNAME_ONINPUTCONNECT, 1, 0);
					}
					else
					{
						context_state = 1;
						StartWiring();
					}
				}

				else if(comp == ContextInputs_Unwire)
				{
					this->RemoveComponent(ContextInputs_Unwire);

					if(ContextInputs_Unwire->ItemClicked == 0)
					{
						LuaComponent* c = ContextInputs_Unwire->comp_ref;
						for(int i = 0; i < c->InputPorts.size(); ++i)
						{
							if(c->InputPorts[i].opposing.comp)
							{
								lua_pushinteger(c->L, i+1);
								c->luaCallObjectFunction(GD_LUA_FNAME_ONINPUTUNCONNECT, 1, 0);

								c->UnconnectInput(i);
							}
						}

						Console->Print(ui::UIConsole::MSG_TINFO, GD_MSG_UNWIREDINPUTALL, c->myaddonitem->name.c_str());
					}
					else
					{
						int port = ContextInputs_Unwire->ItemClicked - 1; // first item is "All", so subtract one.
						LuaComponent* c = ContextInputs_Unwire->comp_ref;

						if(c->InputPorts[port].opposing.comp)
						{
							lua_pushinteger(c->L, port+1);
							c->luaCallObjectFunction(GD_LUA_FNAME_ONINPUTUNCONNECT, 1, 0);

							c->UnconnectInput( port );

							Console->Print( ui::UIConsole::MSG_TINFO, GD_MSG_UNWIREDINPUT,
								c->myaddonitem->name.c_str(),
								c->InputPorts[port].port_name.c_str() );
						}
					}
				}

				else if(comp == ContextOutputs_Unwire)
				{
					this->RemoveComponent(ContextOutputs_Unwire);

					if(ContextOutputs_Unwire->ItemClicked == 0)
					{
						LuaComponent* c = ContextOutputs_Unwire->comp_ref;
						for(int i = 0; i < c->OutputPorts.size(); ++i)
						{
							for(int j = 0; j < c->OutputPorts[i].opposing_list.size(); ++j)
							{
								Opposition& opp = c->OutputPorts[i].opposing_list[j];

								if(opp.comp)
								{
									lua_pushinteger(opp.comp->L, opp.port+1);
									opp.comp->luaCallObjectFunction(GD_LUA_FNAME_ONINPUTUNCONNECT, 1, 0);
								}
							}

							c->UnconnectOutput(i);
						}

						Console->Print(ui::UIConsole::MSG_TINFO, GD_MSG_UNWIREDOUTPUTALL, c->myaddonitem->name.c_str());
					}
					else
					{
						int port = ContextOutputs_Unwire->ItemClicked - 1; // first item is "All", so subtract one.
						LuaComponent* c = ContextOutputs_Unwire->comp_ref;

						for(int i = 0; i < c->OutputPorts[port].opposing_list.size(); ++i)
						{
							Opposition& opp = c->OutputPorts[port].opposing_list[i];

							if(opp.comp)
							{
								lua_pushinteger(opp.comp->L, opp.port+1);
								opp.comp->luaCallObjectFunction(GD_LUA_FNAME_ONINPUTUNCONNECT, 1, 0);
							}
						}

						c->UnconnectOutput( port );

						Console->Print( ui::UIConsole::MSG_TINFO, GD_MSG_UNWIREDOUTPUT,
							c->myaddonitem->name.c_str(),
							c->OutputPorts[port].port_name.c_str() );
					}
				}
			}
		}
		else if(button == MOUSEBUTTON_RIGHT)
		{
			if(*(ui::UI_DATATYPE*)(comp->UserData) == ui::UI_LUACOMPONENT)
			{
				LuaComponent* c = (LuaComponent*)comp;

				// Make sure no context menus are already open
				RemoveComponent(ContextComponent);
				RemoveComponent(ContextInputs);
				RemoveComponent(ContextOutputs);
				RemoveComponent(ContextInputs_Unwire);
				RemoveComponent(ContextOutputs_Unwire);

				//open context menu
				if(context_state == 0)
				{
					//init
					ContextComponent->Position.X = x;
					ContextComponent->Position.Y = y;
					ContextComponent->UpdateList(c);

					// Check if the context falls outside of the screen
					if(ContextComponent->Position.X + ContextComponent->Size.X >= Renderer::Ref()->GetWidth())
					{
						ContextComponent->Position.X = Renderer::Ref()->GetWidth()-1 - ContextComponent->Size.X;
					}
					if(ContextComponent->Position.Y + ContextComponent->Size.Y >= Renderer::Ref()->GetHeight())
					{
						ContextComponent->Position.Y = Renderer::Ref()->GetHeight()-1 - ContextComponent->Size.Y;
					}

					AddComponent(ContextComponent);

					RemoveComponent(TreeContext);
				}
				else if(context_state == 1)
				{
					//wire out->in
					if(c->InputPorts.size() > 0)
					{
						ContextInputs->Position.X = x;
						ContextInputs->Position.Y = y;
						ContextInputs->UpdateList(c);

						// Check if the context falls outside of the screen
						if(ContextInputs->Position.X + ContextInputs->Size.X >= Renderer::Ref()->GetWidth())
						{
							ContextInputs->Position.X = Renderer::Ref()->GetWidth()-1 - ContextInputs->Size.X;
						}
						if(ContextInputs->Position.Y + ContextInputs->Size.Y >= Renderer::Ref()->GetHeight())
						{
							ContextInputs->Position.Y = Renderer::Ref()->GetHeight()-1 - ContextInputs->Size.Y;
						}

						AddComponent(ContextInputs);

						RemoveComponent(TreeContext);
					}
				}
				else if(context_state == 2)
				{
					//wire in->out
					if(c->OutputPorts.size() > 0)
					{
						ContextOutputs->Position.X = x;
						ContextOutputs->Position.Y = y;
						ContextOutputs->UpdateList(c);

						// Check if the context falls outside of the screen
						if(ContextOutputs->Position.X + ContextOutputs->Size.X >= Renderer::Ref()->GetWidth())
						{
							ContextOutputs->Position.X = Renderer::Ref()->GetWidth()-1 - ContextOutputs->Size.X;
						}
						if(ContextOutputs->Position.Y + ContextOutputs->Size.Y >= Renderer::Ref()->GetHeight())
						{
							ContextOutputs->Position.Y = Renderer::Ref()->GetHeight()-1 - ContextOutputs->Size.Y;
						}

						AddComponent(ContextOutputs);

						RemoveComponent(TreeContext);
					}
				}
			}
		}
	}
}

void GameState::OnMouseUnclickedComponent(int x, int y, unsigned button, sys::Component* comp)
{
}

void GameState::OnMouseWheel(int x, int y, int d)
{
}

void GameState::OnKeyPress(int key, bool shift, bool ctrl, bool alt)
{
	switch(key)
	{
	case 'r':
		if(EditMode)
		{
			// Cancel wiring
			if(Wiring)
			{
				context_state = 0;
				Wiring = false;
				WiringPoints.clear();

				Console->Print(ui::UIConsole::MSG_TINFO, GD_MSG_WIRINGCANCEL);
			}
		}
		break;

	case 279: //backspace
		if(EditMode)
		{
			// Remove last point placed during wiring
			if(Wiring)
			{
				if(WiringPoints.size() > 0)
					WiringPoints.pop_back();
			}
		}
		break;
	case 274: //tilda ~
		ShowDebug = !ShowDebug;
		if(ShowDebug)
		{
			Console->Print(ui::UIConsole::MSG_TINFO, GD_MSG_DISPLAYDBG);
		}
		else
		{
			Console->Print(ui::UIConsole::MSG_TINFO, GD_MSG_HIDEDBG);
		}
		break;
	}
}

void GameState::OnText(int key)
{
}

void GameState::OnKeyRelease(int key, bool shift, bool ctrl, bool alt)
{
}

/****************************************/
/************ GAME INTERFACE ************/
/****************************************/

void GameState::WindowLoad_OnResult(ui::WINDOW_RESULT res)
{
	if(res == ui::RESULT_ACCEPT)
	{
		std::string file = WindowLoad_exp->ItemSelected().c_str();
		if(file != GD_NULLSTR)
		{
			Console->Print(ui::UIConsole::MSG_TINFO, GD_MSG_LOADING, file.c_str());

			int err = LoadFile(file.c_str());
			if(err <= 0)
			{
				if(err == -1)
					Console->Print(ui::UIConsole::MSG_TWARNING, GD_MSG_LOADING_FINISHED_WITH_UNLOADEDITEMS);
				else
					Console->Print(ui::UIConsole::MSG_TINFO, GD_MSG_LOADING_FINISHED);

				WindowSave_txtFile->SetString(file.c_str());
			}
			else
			{
				const char* errmsg;
				switch(err)
				{
				case 1: errmsg = GD_MSG_LOADING_ERR_OPEN; break;
				case 2: errmsg = GD_MSG_LOADING_ERR_INVALID; break;
				default: errmsg = GD_MSG_LOADING_ERR_UNKNOWN; break;
				}
				Console->Print(ui::UIConsole::MSG_TERROR, GD_MSG_LOADING_ERR, err, errmsg);
			}
		}
	}

	RemoveComponent(WindowLoad);
	LockMenu = false;
}

void GameState::WindowSave_OnResult(ui::WINDOW_RESULT res)
{
	if(res == ui::RESULT_ACCEPT)
	{
		Console->Print(ui::UIConsole::MSG_TINFO, GD_MSG_SAVING, WindowSave_txtFile->GetString());

		int err = SaveToFile(WindowSave_txtFile->GetString(), WindowSave_chkSaveOutVals->Checked());
		if(!err)
			Console->Print(ui::UIConsole::MSG_TINFO, GD_MSG_SAVING_FINISHED);
		else
		{
			const char* errmsg;
			switch(err)
			{
			case 1: errmsg = GD_MSG_SAVING_ERR_WRITE; break;
			default: errmsg = GD_MSG_SAVING_ERR_UNKNOWN; break;
			}
			Console->Print(ui::UIConsole::MSG_TERROR, GD_MSG_SAVING_ERR, err, errmsg);
		}
	}

	RemoveComponent(WindowSave);
	LockMenu = false;
}

void GameState::WindowContinue_OnResult(ui::WINDOW_RESULT res)
{
	if(res == ui::RESULT_ACCEPT)
	{
		switch(WindowContinue_State)
		{
		case 1: //New
			ResetGame();
			Console->Print(ui::UIConsole::MSG_TINFO, GD_MSG_NEWGAME);
			break;
		case 2: //Exit
			sys::Engine::Ref().Exit();
			break;
		}
	}

	RemoveComponent(WindowContinue);
	LockMenu = false;
}

void GameState::WindowOptions_OnResult(ui::WINDOW_RESULT res)
{
	if (res == ui::RESULT_ACCEPT)
	{
		// Set battery saver mode
		if (!WindowOptions_BatterySaver->Checked())
		{
			BatteryMode = BATTERYMODE_OFF;
		}
		else
		{
			if (WindowOptions_BatterySaver_AlwaysOn->Checked())
				BatteryMode = BATTERYMODE_ON;
			else
				BatteryMode = BATTERYMODE_AUTO;
		}

		// Set grid size
		GridSize = (int)WindowOptions_GridSize->GetNumber();
	}
	else
	{
		// Reset BatteryMode back to original state
		WindowOptions_BatterySaver->SetCheck( BatteryMode != BATTERYMODE_OFF );
		if (BatteryMode = BATTERYMODE_ON)
		{
			WindowOptions_BatterySaver_Auto->SetCheck(false);
			WindowOptions_BatterySaver_AlwaysOn->SetCheck(true);
		}
		else
		{
			WindowOptions_BatterySaver_Auto->SetCheck(true);
			WindowOptions_BatterySaver_AlwaysOn->SetCheck(false);
		}
		
	}

	// Change the text to new GridSize (to get rid of any float numbers, this should be an integer)
	WindowOptions_GridSize->SetString( sys::StringHelper::ToStr(GridSize).c_str() );

	RemoveComponent(WindowOptions);
}

void GameState::WindowAbout_OnResult(ui::WINDOW_RESULT res)
{
	RemoveComponent(WindowAbout);
}

int GameState::SaveToFile(const char* filename, bool save_outdata)
{
	TiXmlDocument doc;

	TiXmlElement savefile(GD_XML_SAVEFILE_ROOT);
	savefile.SetAttribute(GD_XML_SAVEFILE_ROOT_ATT_SCRX, Pan.X);
	savefile.SetAttribute(GD_XML_SAVEFILE_ROOT_ATT_SCRY, Pan.Y);

	for(int i = 0; i < luacomps.size(); ++i)
	{
		TiXmlElement item(GD_XML_SAVEFILE_ITEM);
		LuaComponent* curcomp = luacomps[i];

		//properties
		item.SetAttribute(GD_XML_SAVEFILE_ITEM_ATT_X, curcomp->Position.X);
		item.SetAttribute(GD_XML_SAVEFILE_ITEM_ATT_Y, curcomp->Position.Y);
		item.SetAttribute(GD_XML_SAVEFILE_ITEM_ATT_ADDON, curcomp->myaddonitem->p_category->p_package->name.c_str());
		item.SetAttribute(GD_XML_SAVEFILE_ITEM_ATT_CATEGORY, curcomp->myaddonitem->p_category->name.c_str());
		item.SetAttribute(GD_XML_SAVEFILE_ITEM_ATT_ITEM, curcomp->myaddonitem->name.c_str());

		//save inputs
		for(int j = 0; j < curcomp->InputPorts.size(); ++j)
		{
			if(curcomp->InputPorts[j].opposing.comp != NULL)
			{
				//properties for wiring
				TiXmlElement input(GD_XML_SAVEFILE_INPUT);
				input.SetAttribute(GD_XML_SAVEFILE_INPUT_ATT_PORT, j);
				input.SetAttribute(GD_XML_SAVEFILE_INPUT_ATT_OTHERCOMPPORT, curcomp->InputPorts[j].opposing.port);

				int compid;
				for(compid = 0; compid < luacomps.size(); ++compid)
				{
					if(luacomps[compid] == curcomp->InputPorts[j].opposing.comp)
						break;
				}
				input.SetAttribute(GD_XML_SAVEFILE_INPUT_ATT_OTHERCOMPID, compid);

				//wire points
				for(int k = 0; k < curcomp->InputWirePoints[j].Size(); ++k)
				{
					TiXmlElement point(GD_XML_SAVEFILE_POINT);
					point.SetAttribute(GD_XML_SAVEFILE_POINT_ATT_X, curcomp->InputWirePoints[j][k].X);
					point.SetAttribute(GD_XML_SAVEFILE_POINT_ATT_Y, curcomp->InputWirePoints[j][k].Y);
					input.InsertEndChild(point);
				}

				item.InsertEndChild(input);
			}
		}

		//save outputs
		if(save_outdata)
		{
			for(int j = 0; j < curcomp->OutputPorts.size(); ++j)
			{
				TiXmlElement output(GD_XML_SAVEFILE_OUTPUT);
				output.SetAttribute(GD_XML_SAVEFILE_OUTPUT_ATT_PORT, j);
				switch(curcomp->OutputPorts[j].val_type)
				{
				case TYPE_NUMBER:
					output.SetAttribute(GD_XML_SAVEFILE_OUTPUT_ATT_VALUE, sys::StringHelper::ToStr(curcomp->OutputPorts[j].val_num).c_str());
					break;
				case TYPE_STRING:
					output.SetAttribute(GD_XML_SAVEFILE_OUTPUT_ATT_VALUE, curcomp->OutputPorts[j].val_str.c_str());
					break;
				}
				item.InsertEndChild(output);
			}
		}

		//lua persistant data collection
		curcomp->luaCallObjectFunction(GD_LUA_FNAME_ONGAMESAVE, 0, 1);
		if(lua_istable(curcomp->L, -1))
		{
			lua_pushnil(curcomp->L);
			while(lua_next(curcomp->L, -2))
			{
				TiXmlElement data(GD_XML_SAVEFILE_DATA);

				//save key
				if(lua_isnumber(curcomp->L, -2))
				{
					data.SetAttribute(GD_XML_SAVEFILE_DATA_ATT_KEYTYPE, GD_XML_SAVEFILE_DATA_TYPE_NUMBER);
					data.SetAttribute(GD_XML_SAVEFILE_DATA_ATT_KEY, lua_tointeger(curcomp->L, -2));
				}
				else if(lua_isstring(curcomp->L, -2))
				{
					data.SetAttribute(GD_XML_SAVEFILE_DATA_ATT_KEYTYPE, GD_XML_SAVEFILE_DATA_TYPE_STRING);
					data.SetAttribute(GD_XML_SAVEFILE_DATA_ATT_KEY, lua_tostring(curcomp->L, -2));
				}
				else
				{
					lua_pop(curcomp->L, 1);
					continue;
				}

				//save data
				switch(lua_type(curcomp->L, -1))
				{
				case LUA_TNUMBER:
					data.SetAttribute(GD_XML_SAVEFILE_DATA_ATT_VALTYPE, GD_XML_SAVEFILE_DATA_TYPE_NUMBER);
					data.SetDoubleAttribute(GD_XML_SAVEFILE_DATA_ATT_VALUE, lua_tonumber(curcomp->L, -1));
					break;
				case LUA_TSTRING:
					data.SetAttribute(GD_XML_SAVEFILE_DATA_ATT_VALTYPE, GD_XML_SAVEFILE_DATA_TYPE_STRING);
					data.SetAttribute(GD_XML_SAVEFILE_DATA_ATT_VALUE, lua_tostring(curcomp->L, -1));
					break;
				case LUA_TBOOLEAN:
					data.SetAttribute(GD_XML_SAVEFILE_DATA_ATT_VALTYPE, GD_XML_SAVEFILE_DATA_TYPE_BOOLEAN);
					data.SetAttribute(GD_XML_SAVEFILE_DATA_ATT_VALUE, lua_toboolean(curcomp->L, -1));
					break;
				case LUA_TNIL:
					data.SetAttribute(GD_XML_SAVEFILE_DATA_ATT_VALTYPE, GD_XML_SAVEFILE_DATA_TYPE_NIL);
					break;
				default:
					//don't add data, invalid type
					lua_pop(curcomp->L, 1);
					continue;
					break;
				}

				lua_pop(curcomp->L, 1);

				item.InsertEndChild(data);
			}

			lua_pop(curcomp->L, 2);

		}

		savefile.InsertEndChild(item);
	}

	doc.InsertEndChild(savefile);

	// write to file
	sys::io::Path path_saves(GD_PATH_SAVES);
	if(doc.SaveFile((std::string(path_saves.GetFullPath())+GD_PATHSEP+filename+GD_EXT_SAVEFILE).c_str()))
		return 0;
	else
		return 1;
}

int GameState::LoadFile(const char* filename)
{
	sys::io::Path path_saves(GD_PATH_SAVES);

	TiXmlDocument doc;
	doc.LoadFile((std::string(path_saves.GetFullPath())+GD_PATHSEP+filename+GD_EXT_SAVEFILE).c_str());

	bool items_not_loaded = false;

	if(!doc.Error())
	{
		const TiXmlElement* n_savefile = doc.FirstChildElement(GD_XML_SAVEFILE_ROOT);
		if(n_savefile)
		{
			//we're ready to load, clear the game to make way for the save
			ResetGame();

			//scrx and scry
			int px = 0, py = 0;
			n_savefile->Attribute(GD_XML_SAVEFILE_ROOT_ATT_SCRX, &px);
			n_savefile->Attribute(GD_XML_SAVEFILE_ROOT_ATT_SCRY, &py);
			Pan.X = px;
			Pan.Y = py;
			GridPan.X = Pan.X;
			GridPan.Y = Pan.Y;
			while(GridPan.X > GridSize) GridPan.X -= GridSize;
			while(GridPan.Y > GridSize) GridPan.Y -= GridSize;
			while(GridPan.X <= 0) GridPan.X += GridSize;
			while(GridPan.Y <= 0) GridPan.Y += GridSize;

			std::vector<bool> itemsloaded;

			//load all components into game
			const TiXmlElement* n_item = n_savefile->FirstChildElement(GD_XML_SAVEFILE_ITEM);
			if(n_item) do
			{
				const char* addon_name = n_item->Attribute(GD_XML_SAVEFILE_ITEM_ATT_ADDON);
				const char* category_name = n_item->Attribute(GD_XML_SAVEFILE_ITEM_ATT_CATEGORY);
				const char* item_name = n_item->Attribute(GD_XML_SAVEFILE_ITEM_ATT_ITEM);
				AddonPackage* package = NULL;
				AddonCategory* category = NULL;
				AddonItem* item = NULL;

				// create all components

				// get package
				for(int i = 0; i < AddonReader::Ref().Packages.size(); ++i)
				{
					if(AddonReader::Ref().Packages[i]->name == addon_name)
					{
						package = AddonReader::Ref().Packages[i];
						break;
					}
				}

				if(package != NULL)
				{
					// get category
					for(int i = 0; i < package->categories.size(); ++i)
					{
						if(package->categories[i]->name == category_name)
						{
							category = package->categories[i];
							break;
						}
					}

					if(category != NULL)
					{
						// get item
						for(int i = 0; i < category->items.size(); ++i)
						{
							if(category->items[i]->name == item_name)
							{
								item = category->items[i];
								break;
							}
						}

						if(item != NULL)
						{
							// we've found it! Create the component

							int x = 0, y = 0;
							n_item->Attribute(GD_XML_SAVEFILE_ITEM_ATT_X, &x);
							n_item->Attribute(GD_XML_SAVEFILE_ITEM_ATT_Y, &y);

							LuaComponent* c = CreateComponent(item, x, y);
							this->InsertComponent(c, 0);
							luacomps.push_back(c);
							itemsloaded.push_back(true);
						}
						else
						{
							sys::Dbg::Printf(GD_MSG_LOADING_WARNING_ITEMNOTFOUND, item_name);
							itemsloaded.push_back(false);
							items_not_loaded = true;
						}
					}
					else
					{
						sys::Dbg::Printf(GD_MSG_LOADING_WARNING_CATEGORYNOTFOUND, category_name);
						itemsloaded.push_back(false);
						items_not_loaded = true;
					}
				}
				else
				{
					sys::Dbg::Printf(GD_MSG_LOADING_WARNING_PACKAGENOTFOUND, addon_name);
					itemsloaded.push_back(false);
					items_not_loaded = true;
				}

			} while((n_item = n_item->NextSiblingElement(GD_XML_SAVEFILE_ITEM)) != NULL);

			// now that all components are loaded into the game, let's set them up
			n_item = n_savefile->FirstChildElement(GD_XML_SAVEFILE_ITEM);
			int id = 0, __fails = 0;
			if(n_item) do
			{
				if(!itemsloaded[id])
				{
					++__fails;
					continue;
				}

				LuaComponent* cur_comp = luacomps[id-__fails];

				//load persistant data
				lua_newtable(cur_comp->L);
				const TiXmlElement* n_data = n_item->FirstChildElement(GD_XML_SAVEFILE_DATA);
				if(n_data) do
				{
					//push key
					const char* ktype = n_data->Attribute(GD_XML_SAVEFILE_DATA_ATT_KEYTYPE);
					if(strcmp(ktype, GD_XML_SAVEFILE_DATA_TYPE_STRING)==0)
					{
						lua_pushstring(cur_comp->L, n_data->Attribute(GD_XML_SAVEFILE_DATA_ATT_KEY));
					}
					else
					{
						int key = 0.0;
						n_data->Attribute(GD_XML_SAVEFILE_DATA_ATT_KEY, &key);
						lua_pushinteger(cur_comp->L, key);
					}

					//push value
					const char* type = n_data->Attribute(GD_XML_SAVEFILE_DATA_ATT_VALTYPE);
					if(strcmp(type, GD_XML_SAVEFILE_DATA_TYPE_NUMBER)==0)
					{
						double val = 0.0;
						n_data->Attribute(GD_XML_SAVEFILE_DATA_ATT_VALUE, &val);
						lua_pushnumber(cur_comp->L, val);
					}
					else if(strcmp(type, GD_XML_SAVEFILE_DATA_TYPE_STRING)==0)
					{
						lua_pushstring(cur_comp->L, n_data->Attribute(GD_XML_SAVEFILE_DATA_ATT_VALUE));
					}
					else if(strcmp(type, GD_XML_SAVEFILE_DATA_TYPE_BOOLEAN)==0)
					{
						int val = 0;
						n_data->Attribute(GD_XML_SAVEFILE_DATA_ATT_VALUE, &val);
						lua_pushboolean(cur_comp->L, val);
					}
					else
					{
						lua_pushnil(cur_comp->L);
					}

					lua_settable(cur_comp->L, -3);

				} while((n_data = n_data->NextSiblingElement(GD_XML_SAVEFILE_DATA)) != NULL);
				cur_comp->luaCallObjectFunction(GD_LUA_FNAME_ONGAMELOAD, 1, 0);

				//set up wiring
				const TiXmlElement* n_input = n_item->FirstChildElement(GD_XML_SAVEFILE_INPUT);
				if(n_input) do
				{
					int port = 0;
					int compid = 0;
					int compport = 0;
					n_input->Attribute(GD_XML_SAVEFILE_INPUT_ATT_PORT, &port);
					n_input->Attribute(GD_XML_SAVEFILE_INPUT_ATT_OTHERCOMPID, &compid);
					n_input->Attribute(GD_XML_SAVEFILE_INPUT_ATT_OTHERCOMPPORT, &compport);

					if(!itemsloaded[compid])
						continue;

					if(port >= cur_comp->InputPorts.size() || compport >= luacomps[compid]->OutputPorts.size())
						continue; // Error: Could not wire to non-existant port!

					cur_comp->ConnectInput(luacomps[compid], port, compport);

					//extract wire points
					const TiXmlElement* n_point = n_input->FirstChildElement(GD_XML_SAVEFILE_POINT);
					std::vector< sys::Vectori > points;
					if(n_point) do
					{
						sys::Vectori vec;
						n_point->Attribute(GD_XML_SAVEFILE_POINT_ATT_X, &vec.X);
						n_point->Attribute(GD_XML_SAVEFILE_POINT_ATT_Y, &vec.Y);
						points.push_back(vec);
					} while((n_point = n_point->NextSiblingElement(GD_XML_SAVEFILE_POINT)) != NULL);

					//apply wire points
					cur_comp->InputWirePoints[port].Create(points.size());
					for(int i = 0; i < points.size(); ++i)
					{
						cur_comp->InputWirePoints[port][i] = points[i];
					}
					points.clear();

				} while((n_input = n_input->NextSiblingElement(GD_XML_SAVEFILE_INPUT)) != NULL);

				//set up output values
				const TiXmlElement* n_output = n_item->FirstChildElement(GD_XML_SAVEFILE_OUTPUT);
				if(n_output) do
				{
					int port = 0;
					n_output->Attribute(GD_XML_SAVEFILE_OUTPUT_ATT_PORT, &port);

					switch(cur_comp->OutputPorts[port].val_type)
					{
					case TYPE_NUMBER:
						n_output->Attribute(GD_XML_SAVEFILE_OUTPUT_ATT_VALUE, &cur_comp->OutputPorts[port].val_num);
						break;
					case TYPE_STRING:
						cur_comp->OutputPorts[port].val_str = n_output->Attribute(GD_XML_SAVEFILE_OUTPUT_ATT_VALUE);
						break;
					}

				} while((n_output = n_output->NextSiblingElement(GD_XML_SAVEFILE_OUTPUT)) != NULL);

				++id;
			} while((n_item = n_item->NextSiblingElement(GD_XML_SAVEFILE_ITEM)) != NULL);
		}
		else return 2;
	}
	else return 1;

	if(items_not_loaded)
		return -1;
	return 0;
}

LuaComponent* GameState::CreateComponent(AddonItem* item, int x, int y)
{
	// Create new component
	LuaComponent* c = new LuaComponent(this);
	c->myaddonitem = item;
	c->Position.X = x;
	c->Position.Y = y;

	sys::String filepath = GD_NULLSTR;
	filepath << GD_PATH_ADDONS << GD_PATHSEP
		<< item->p_category->p_package->path.c_str() << GD_PATHSEP
		<< GD_PATH_ADDONS_LUA << GD_PATHSEP
		<< item->p_category->path.c_str() << GD_PATHSEP
		<< item->path.c_str() ;

	c->luaLoadScript(filepath.C_Str());
	return c;
}

void GameState::PerformUIChecks()
{
	if (WindowOptions_RefreshAddons->Poll())
	{
		Console->Print(ui::UIConsole::MSG_TINFO, GD_MSG_LOADING_REFRESHADDONS);
		AddonReader::Ref().RefreshAddons();
	}
}

void GameState::MenuButtonSelected(int selected)
{
	if(LockMenu)
		return;

	switch(selected)
	{
	case 0: //new
		WindowContinue_State = 1;
		WindowContinue_Label->SetString(game::GD_WINDOW_CONTINUE_MESSAGE_NEW);
		WindowContinue->Size.X = 20 + WindowContinue_Label->txt.GetRect().Width;
		WindowContinue->Size.Y = WindowContinue_Label->Size.Y + 32;
		WindowContinue->Show();
		AddComponent(WindowContinue);

		LockMenu = true;
		break;
	case 1: //load
		WindowLoad_exp->OpenFolder(GD_PATH_SAVES);
		WindowLoad->Show();
		AddComponent(WindowLoad);
		LockMenu = true;
		break;
	case 2: //save
		WindowSave->Show();
		AddComponent(WindowSave);
		LockMenu = true;
		break;
	case 3: //toggle mode
		EditMode = !EditMode;
		if(EditMode)
		{
			Menu->EditItem(3, GD_GAME_MENU_GAMEMODE, GD_RES_CONTROLLER);
			context_state = 0;
		}
		else
		{
			Menu->EditItem(3, GD_GAME_MENU_EDITMODE, GD_RES_WRENCHBLUE);
			context_state = 0;

			//close edit components
			TreeContextOpened = false;
			RemoveComponent(TreeContext);
			RemoveComponent(ContextComponent);
			RemoveComponent(ContextInputs);
			RemoveComponent(ContextOutputs);
			RemoveComponent(ContextInputs_Unwire);
			RemoveComponent(ContextOutputs_Unwire);

			Wiring = false;
			WiringPoints.clear();
		}

		Menu->Update();

		break;
	case 4: //toggle grid

		GridMode = !GridMode;

		if (GridMode)
		{
			Menu->EditItem(4, GD_GAME_MENU_DISABLEGRID, GD_RES_GRID);
		}
		else
		{
			Menu->EditItem(4, GD_GAME_MENU_ENABLEGRID, GD_RES_GRID);
		}

		Menu->Update();

		break;

	case 5: //options menu
		
		if (WindowOptions->IsOpened()) break;

		WindowOptions->Show(false);
		AddComponent(WindowOptions);

		break;

	case 6: //about

		if (WindowAbout->IsOpened()) break;

		WindowAbout->Show(true);
		AddComponent(WindowAbout);

		break;

	case 7: //exit

		if (WindowContinue->IsOpened()) break;

		WindowContinue_State = 2;
		WindowContinue_Label->SetString(game::GD_WINDOW_CONTINUE_MESSAGE_EXIT);
		WindowContinue->Size.X = 20 + WindowContinue_Label->txt.GetRect().Width;
		WindowContinue->Size.Y = WindowContinue_Label->Size.Y + 32;
		WindowContinue->Show();
		AddComponent(WindowContinue);

		LockMenu = true;
		break;

	}
}

void GameState::StartWiring()
{
	this->WiringPoints.clear();
	Wiring = true;
	Console->Print(ui::UIConsole::MSG_TINFO_NOCONSOLE, GD_MSG_WIRINGSTARTED,
		GD_MSG_WIRINGSTARTED_RIGHTCLICK,
		GD_MSG_WIRINGSTARTED_BACKSPACE,
		GD_MSG_WIRINGSTARTED_R );
}

void GameState::ResetGame()
{
	for(int i = 0; i < this->GetComponentCount(); ++i)
	{
		if(*(ui::UI_DATATYPE*)(this->GetComponent(i)->UserData) == ui::UI_LUACOMPONENT)
		{
			//this component is a lua component
			LuaComponent* c = (LuaComponent*)this->GetComponent(i);

			c->luaCallObjectFunction(GD_LUA_FNAME_ONREMOVE);
			lua_close(c->L);

			RemoveComponent(c);
			
			--i;
		}
	}
	
	luacomps.clear();

	Pan.X = 0;
	Pan.Y = 0;
	GridPan.X = 0;
	GridPan.Y = 0;
	WiringPoints.clear();
	Wiring = false;
	context_state = 0;
}

inline void GameState::ConvertToGridPos(int& x, int& y)
{
	if(GridMode)
	{
		//inaccurate
		//x = (x + GridSize/2)/GridSize * GridSize;
		//y = (y + GridSize/2)/GridSize * GridSize;

		x = static_cast<int>(sys::Math::Round( static_cast<float>(x)/GridSize ) * GridSize);
		y = static_cast<int>(sys::Math::Round( static_cast<float>(y)/GridSize ) * GridSize);
	}
}

lua_State* GameState::NewLuaState()
{
	//Create a new state
	lua_State* l = luaL_newstate();

	//Load libraries to state
	luaL_openlibs(l);

	return l;
}

void GameState::CloseLuaState(lua_State* l)
{
	lua_close(l);
}
