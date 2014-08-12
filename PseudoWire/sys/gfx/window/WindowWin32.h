#pragma once

#include "../VideoSettings.h"
#include "../opengl.h"
#include "../Events.h"
#include "../../Platform.h"

#include <vector>

namespace sys
{
namespace gfx
{

class WindowPlat
{
public:
	WindowPlat(const char* Title, const VideoSettings & Settings, WindowStyle Style, u8 BitsPerPixel = 32);
	virtual ~WindowPlat();

	inline void Clear() { Activate(); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
	inline void Display() { Activate(); glFlush(); }
	inline void Activate() { if(!IsActive()) { wglMakeCurrent(myDeviceContext, myRenderContext); activeRenderContext = myRenderContext; } }
	inline bool IsActive() const { return activeRenderContext == myRenderContext; }

	inline void SetTitle(const char* title) { SetWindowText(myWndHandle, title); }
	inline void Show(bool show = true) { ShowWindow(myWndHandle, (show ? SW_SHOW : SW_HIDE)); }
	void ShowMouseCursor(bool show = true);
	void SetCursorLoc(s32 x, s32 y);
	void SetWindowLocation(s32 x, s32 y);
	void SetSize(s32 width, s32 height);
	bool SetIcon(s32 width, s32 height, const u8* pixels);

	inline s32 Width() const { return myWidth; }
	inline s32 Height() const { return myHeight; }

	bool PollEvent(EventHandle& Event);
	
protected:

	void ProcessEvent(UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK OnEvent(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
	
private:
	HWND myWndHandle;
	HDC myDeviceContext;
	PIXELFORMATDESCRIPTOR myPixelFormat;
	HGLRC myRenderContext;
	HICON myIcon;
	HCURSOR myCursor;

	s32 myWidth;
	s32 myHeight;

	bool cursorInside;

	std::vector<EventHandle*> events;
	
	u8 myBitsPerPixel;
	
	static const char* windowClassName;
	static u32 windowsCount;
	static HGLRC activeRenderContext;
	static WindowPlat* currentFullscreenWindow;
	static Key VK2Key(WPARAM wparam, LPARAM lparam);
};

}
}
