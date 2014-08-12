#pragma once

#include "Renderer.h"
#include <sys/sys.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "UI/UITree.h"
#include "UI/UICompContext.h"
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

#include "Port.h"

#include "lua/lua.hpp"

namespace game
{

namespace ui
{
	class UIMenu;
	class UIEdit;
}

class GameState : public sys::State
{

/* **** ENGINE **** */

public:
	GameState();
	~GameState();

protected:
	void OnInitialized();
	void OnExit();

	void OnTick(float dt);
	void OnDraw();
	void OnPostFXDraw();
	void OnResize(int w, int h);

	void OnMouseMove(int x, int y, int dx, int dy);
	void OnMouseDown(int x, int y, unsigned button);
	void OnMouseUp(int x, int y, unsigned button);
	void OnMouseClicked(int x, int y, unsigned button);
	void OnMouseUnclicked(int x, int y, unsigned button);
	void OnMouseClickedComponent(int x, int y, unsigned button, sys::Component* comp);
	void OnMouseUnclickedComponent(int x, int y, unsigned button, sys::Component* comp);
	void OnMouseWheel(int x, int y, int d);
	void OnKeyPress(int key, bool shift, bool ctrl, bool alt);
	void OnKeyRelease(int key, bool shift, bool ctrl, bool alt);
	void OnText(int key);

/* **** GAME **** */

public:
	lua_State* NewLuaState();
	void DrawLoading();
	void CloseLuaState(lua_State* l);
	void ResetGame();
	int SaveToFile(const char* filename, bool save_outdata = false);
	int LoadFile(const char* filename);
	LuaComponent* CreateComponent(AddonItem* item, int x = 0, int y = 0);
	void LoadUserSettings(const char* filename);
	void StartWiring();
	void PerformUIChecks();

	void RecalibrateUI(int w, int h);

	sf::Image* m_img;
	sf::Sprite* m_spr;
	sf::Image* mw_img;
	sf::Sprite* mw_spr;

	sf::Shape shp_bg_top;
	sf::Shape shp_bg_btm;
	sf::Shape shp_bg_lft;
	sf::Shape shp_bg_rgt;

	std::vector<LuaComponent*> queue_comp_delete;

	sf::Text ToolTipText;
	
	sf::Text IOInfoIText;
	sf::Text IOInfoOText;
	sf::Text IOInfoNText;

	sf::Text* txt_info;
	//sys::HRTimer timerTICK;
	double TICK;
	unsigned long ITER;
	bool ShowDebug;
	void MenuButtonSelected(int selected);
	inline void ConvertToGridPos(int& x, int& y);

	void WindowLoad_OnResult(ui::WINDOW_RESULT res);
	void WindowSave_OnResult(ui::WINDOW_RESULT res);
	void WindowContinue_OnResult(ui::WINDOW_RESULT res);
	void WindowOptions_OnResult(ui::WINDOW_RESULT res);
	void WindowAbout_OnResult(ui::WINDOW_RESULT res);

	std::vector<LuaComponent*> deadendlist;
	sys::Stack<Opposition> updatestack;
	struct pathstack_s
	{
		pathstack_s(LuaComponent* c = NULL, int out = 0, int opp = 0)
		: comp(c)
		, out_p(out)
		, opp_p(opp)
		{}

		LuaComponent* comp;
		int out_p, opp_p;
	};
	sys::Stack<pathstack_s> pathstack;

	enum
	{
		BATTERYMODE_OFF = 0,
		BATTERYMODE_ON,
		BATTERYMODE_AUTO
	} BatteryMode;

	int ScrW, ScrH;
	bool ScrFullscreen;
	bool EditMode;
	bool GridMode;
	int GridSize;
	bool Panning;
	sys::Point Pan;
	sys::Point GridPan;
	bool Wiring;
	LuaComponent* DragComponent;
	LuaComponent* HoverComponent;
	sys::Point DragOffset;
	std::vector<sys::Vectori> WiringPoints;

	ui::UIEdit* EditWindow;

	bool TreeContextOpened;
	ui::UITree* TreeContext;
	ui::UICompContext* ContextComponent;
	ui::UICompContext* ContextInputs;
	ui::UICompContext* ContextOutputs;
	ui::UICompContext* ContextInputs_Unwire;
	ui::UICompContext* ContextOutputs_Unwire;
	ui::UIMenu* Menu;
	bool LockMenu;
	ui::UIConsole* Console;
	unsigned char context_state;
	std::vector<LuaComponent*> luacomps;


	ui::UIWindow<GameState>* WindowLoad;
	//ui::UIEdit_String* WindowLoad_txtFile;

	ui::UIEdit_Explorer* WindowLoad_exp;

	ui::UIWindow<GameState>* WindowSave;
	ui::UIEdit_String* WindowSave_txtFile;
	ui::UIEdit_Check* WindowSave_chkSaveOutVals;

	ui::UIWindow<GameState>* WindowContinue;
	int WindowContinue_State;
	ui::UIEdit_Label* WindowContinue_Label;

	ui::UIWindow<GameState>* WindowOptions;
	ui::UIEdit_Check* WindowOptions_BatterySaver;
	ui::UIEdit_Sprite* WindowOptions_BatterySaver_icon;
	ui::UIEdit_Radio* WindowOptions_BatterySaver_Auto;
	ui::UIEdit_Radio* WindowOptions_BatterySaver_AlwaysOn;
	ui::UIEdit_Number* WindowOptions_GridSize;
	sf::Image* WindowOptions_BatterySaver_iconimg;
	ui::UIButton* WindowOptions_RefreshAddons;

	ui::UIWindow<GameState>* WindowAbout;
	ui::UIEdit_Label* WindowAbout_Label;
};

}
