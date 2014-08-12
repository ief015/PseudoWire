#pragma once
#include "../Platform.h"

namespace sys
{
namespace gfx
{

struct VideoSettings
{
public:
	VideoSettings(s32 width, s32 height, bool doubleBuffer = false, u8 colourBits = 24, u8 stencilBits = 8, u8 depthBits = 16)
	: Width(width)
	, Height(height)
	, DoubleBuffer(doubleBuffer)
	, ColourBits(colourBits)
	, StencilBits(stencilBits)
	, DepthBits(depthBits)
	{ }
	
	s32 Width;
	s32 Height;
	u8 ColourBits;
	u8 StencilBits;
	u8 DepthBits;
	bool DoubleBuffer;
};

enum WindowStyle
{
	  STYLE_NO_BORDER = 0
	, STYLE_TITLEBAR_ONLY
	, STYLE_CLOSE_ONLY
	, STYLE_CLOSE_MINIMIZE
	, STYLE_RESIZABLE
	, STYLE_FULLSCREEN
	, STYLE_FULLSCREEN_AUTOSIZE
};

}
}