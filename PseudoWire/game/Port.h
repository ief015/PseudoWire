#pragma once

#include <sys/sys.h>

namespace game
{
class LuaComponent;

enum PORT_TYPE
{
	TYPE_NUMBER = 0,
	TYPE_STRING,

	TYPE_ANY,

	NUM_PORT_TYPES
};

enum PORT_DRAW_STATE
{
	DRAW_STATE_IDLE,
	DRAW_STATE_TRIGGERED,
	DRAW_STATE_ERROR,

	NUM_PORT_DRAW_STATES
};

struct Opposition
{
	int port;
	LuaComponent* comp;

	Opposition(LuaComponent* c = NULL, int p = -1)
	{
		comp = c;
		port = p;
	}

};

class InPort
{
public:
	InPort()
	{
		port_name = "";
		x = 0.0;
		y = 0.0;
		val_type = TYPE_NUMBER;
	}

	std::string port_name;
	Opposition opposing; //inports only allowed one link.

	//PORT_DRAW_STATE draw_state;

	double x;
	double y;
	PORT_TYPE val_type;
};

class OutPort
{
public:
	OutPort()
	{
		port_name = "";
		x = 0.0;
		y = 0.0;
		val_type = TYPE_NUMBER;
		val_num = 0.0;
		val_str = "";
	}

	std::string port_name;
	std::vector<Opposition> opposing_list; //outports may have multiple links.

	double x;
	double y;
	PORT_TYPE val_type;

	PORT_DRAW_STATE draw_state;

	double val_num;
	std::string val_str;
};

}
