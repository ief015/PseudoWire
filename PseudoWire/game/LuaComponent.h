#pragma once

#include <sys/sys.h>
#include "lua/lua.hpp"
#include "luna.h"
#include "LuaAPI.h"

#include "GameState.h"
#include "GameLoader.h"

#include <SFML/Graphics.hpp>

#include "Port.h"

namespace game
{
class GameState;

class LuaComponent : public sys::Component
{
public:

	/***************************************************************/
	//UI Events

	LuaComponent(sys::State* parent);
	LuaComponent(lua_State* l);
	virtual ~LuaComponent();

	void Tick(float dt);
	void Draw(const sys::Point& screenPos);

	void OnMouseHover(int localx, int localy);
	void OnMouseMoved(int localx, int localy, int dx, int dy);
	void OnMouseMovedInside(int localx, int localy, int dx, int dy);
	void OnMouseEnter(int localx, int localy);
	void OnMouseLeave(int localx, int localy);
	void OnMouseDown(int x, int y, unsigned int button);
	void OnMouseUp(int x, int y, unsigned int button);
	void OnMouseClick(int localx, int localy, unsigned int button);
	void OnMouseUnclick(int localx, int localy, unsigned int button);
	void OnMouseWheel(int localx, int localy, int d);
	void OnMouseWheelInside(int localx, int localy, int d);
	void OnKeyPress(int key, bool shift, bool ctrl, bool alt);
	void OnKeyRelease(int key, bool shift, bool ctrl, bool alt);
	void OnKeyDown(int key, bool shift, bool ctrl, bool alt);
	void OnKeyUp(int key, bool shift, bool ctrl, bool alt);

	/***************************************************************/
	//Game Data
private:
	void Init();

public:
	void ConnectInput(LuaComponent* opposingComp, int our_inport, int their_outport);
	void ConnectOutput(LuaComponent* opposingComp, int our_outport, int their_inport);
	void UnconnectInput(int port);
	void UnconnectOutput(int port);

	static void StaticInit();
	static sf::Image DefaultImage;

	GameState* gamestate;
	AddonItem* myaddonitem;

	std::vector<InPort> InputPorts;
	std::vector<OutPort> OutputPorts;
	std::vector< sys::ArrayOf<sys::Vectori> > InputWirePoints;
	std::string Tooltip;
	sf::Sprite MySprite;
	bool ShowSprite;
	std::string scriptpath;
	bool _hovering;

	bool error;

	//tick helpers
	bool _ischanged;
	bool _forceischanged;
	sys::ArrayOf<bool> _inputneedsupdating;
	bool _doneupdating;
	bool _needsupdatenexttick;
	bool _forcedeadend;
	bool _deadend;
	bool _outputtriggered;
	bool _loopgate;
	bool _startend;
	bool _pathstackinit;
	
	bool allowNewEditComps;
	struct EditComponent
	{
		// 1 = check, 2 = number, 3 = radio, 4 = string
		int type;

		std::string ref;
		Component* c;
	};
	std::vector<EditComponent> editComponents;
	struct LastRadioItem
	{
		void* last;
		std::string group;
	};
	std::vector<LastRadioItem> lastRadios;

	//lua functions
	LUA_FUNC(SetImage);
	LUA_FUNC(GetWidth);
	LUA_FUNC(GetHeight);

	LUA_FUNC(ToLocal);
	LUA_FUNC(ToGlobal);

	LUA_FUNC(SetToolTip);

	LUA_FUNC(SetInputs);
	LUA_FUNC(SetOutputs);
	LUA_FUNC(SetBoundsSize);
	LUA_FUNC(GetBoundsSize);
	LUA_FUNC(GetNumInputs);
	LUA_FUNC(GetNumOutputs);
	LUA_FUNC(TriggerOutput);
	LUA_FUNC(GetInputValue);
	LUA_FUNC(GetAllInputValues);
	LUA_FUNC(GetInputsTable);
	LUA_FUNC(GetInputType);
	LUA_FUNC(IsConnected);
	LUA_FUNC(IsInputTriggered);

	LUA_FUNC(AddUINumber);
	LUA_FUNC(AddUIString);
	LUA_FUNC(AddUICheck);
	LUA_FUNC(AddUIRadio);
	LUA_FUNC(AddUIKey);
	LUA_FUNC(AddUISeparator);
	LUA_FUNC(AddUILabel);

	LUA_FUNC(Draw);

	//registry
	static Luna<LuaComponent>::RegType registerMethods[];
	static const char className[];

	/***************************************************************/
	//Lua Control

	lua_State* L;
	void luaLoadScript(const char* filename);

	/* 
	*	Call a lua function. Values at the top of the stack will be pushed as arguments.
	*	Returns:
	*	-1 if function was not found
	*	0 successfully ran function
	*	1 function encountered an error
	*/
	int luaCallFunction(const char* func, int num_args = 0, int num_results = 0);

	/* 
	*	Call a lua function on variable. Value at top of the stack is the object used to call the method. Anything below are the arguments.
	*	Returns:
	*	-1 if function was not found
	*	0 successfully ran function
	*	1 function encountered an error
	*/
	int luaCallObjectFunction(const char* func, int num_args = 0, int num_results = 0);

};

}
