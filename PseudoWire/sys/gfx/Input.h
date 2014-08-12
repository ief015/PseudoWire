#pragma once

#include "Events.h"
#include "../Platform.h"

namespace sys
{
namespace gfx
{

struct Input
{
public:
	Input()
	{
		for(u32 i = 0; i < NUM_KEYS; ++i)
			keysDown[i] = false;
		for(u32 i = 0; i < NUM_MOUSEBUTTONS; ++i)
			mouseBtnsDown[i] = false;

		mouseX = 0;
		mouseY = 0;
	}

	bool keysDown[NUM_KEYS];
	bool mouseBtnsDown[NUM_MOUSEBUTTONS];

	s16 mouseX;
	s16 mouseY;
};

}
}
