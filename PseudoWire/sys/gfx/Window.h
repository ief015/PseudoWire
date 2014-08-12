#pragma once

#include "../Platform.h"

#ifdef IEF_PLATFORM_WIN32
# include "window/WindowWin32.h"
#else
# error Needs to be implemented.
#endif

#include "VideoSettings.h"
#include "Input.h"

namespace sys
{
namespace gfx
{

class Window
{
public:
	Window(const char* Title, const VideoSettings& Settings, WindowStyle Style, u8 BitsPerPixel = 32)
	: myWindow(NULL)
	{
		input = Input();

		myWindow = new WindowPlat(Title, Settings, Style, BitsPerPixel);
	}

	virtual ~Window()
	{
		if(myWindow)
			delete myWindow;
	}

	inline void Clear()
	{
		if(myWindow) myWindow->Clear();
	}

	inline void Display()
	{
		if(myWindow) myWindow->Display();
	}

	inline void Activate()
	{
		if(myWindow) myWindow->Activate();
	}

	inline bool IsActive() const
	{
		if(myWindow) return myWindow->IsActive();
	}

	inline void Show(bool show = true)
	{
		if(myWindow) myWindow->Show(show);
	}

	inline void SetTitle(const char* title)
	{
		if(myWindow) myWindow->SetTitle(title);
	}

	inline void ShowMouse(bool show = true)
	{
		if(myWindow) myWindow->ShowMouseCursor(show);
	}

	inline void SetMouseLocation(u32 x, u32 y)
	{
		if(myWindow)
			myWindow->SetCursorLoc(x, y);
	}

	inline void SetWindowLocation(u32 x, u32 y)
	{
		if(myWindow) myWindow->SetWindowLocation(x, y);
	}

	inline void SetSize(u32 width, u32 height)
	{
		if(myWindow) myWindow->SetSize(width, height);
	}

	inline void SetIcon(u32 width, u32 height, const u8* pixelsRGBA)
	{
		if(myWindow) myWindow->SetIcon(width, height, pixelsRGBA);
	}

	inline s32 Width() const { if(myWindow) return myWindow->Width(); return 0; }
	inline s32 Height() const { if(myWindow) return myWindow->Height(); return 0; }

	inline bool IsMouseDown(MouseButton btn) const { return input.mouseBtnsDown[btn]; }
	inline bool IsKeyDown(Key key) const { return input.keysDown[key]; }
	inline s16 MouseX() const { return input.mouseX; }
	inline s16 MouseY() const { return input.mouseY; }

	bool PollEvent(EventHandle& Event)
	{
		if(myWindow)
		{
			for(;;)
			{
				if(myWindow->PollEvent(Event))
				{
					//perform input stuff
					switch(Event.Type)
					{
					case EVENT_MOUSEDOWN:
						input.mouseBtnsDown[Event.mouseButton] = true;
						break;
					case EVENT_MOUSEUP:
						input.mouseBtnsDown[Event.mouseButton] = false;
						break;
					case EVENT_KEYDOWN:
						input.keysDown[Event.key] = true;
						break;
					case EVENT_KEYUP:
						input.keysDown[Event.key] = false;
						break;
					case EVENT_MOUSEMOVED:
						input.mouseX = Event.mouseX;
						input.mouseY = Event.mouseY;
						break;
					}

					return true;
				}
				break;
			}
		}
		return false;
	}

	inline bool IsOpen() const { return myWindow != NULL; }
	void Close()
	{
		if(myWindow)
		{
			delete myWindow;
			myWindow = NULL;
		}
	}

protected:

private:
	WindowPlat* myWindow; //platform specific window
	Input input;
};

}
}