#include "WindowWin32.h"
#include "../VideoSettings.h"
#include "../opengl.h"
#include "../Events.h"
#include "../../Platform.h"

using namespace sys;
using namespace sys::gfx;

const char* WindowPlat::windowClassName = "iefengine_glwindow";
u32 WindowPlat::windowsCount = 0;
HGLRC WindowPlat::activeRenderContext = NULL;
WindowPlat* WindowPlat::currentFullscreenWindow = NULL;

WindowPlat::WindowPlat(const char* Title, const VideoSettings & settings, WindowStyle Style, u8 BitsPerPixel)
{
	if(!windowsCount)
	{
		WNDCLASS ass; //ass
		ass.style = 0;
		ass.lpfnWndProc = &WindowPlat::OnEvent;
		ass.cbClsExtra = 0;
		ass.cbWndExtra = 0;
		ass.hInstance = GetModuleHandle(NULL);
		ass.hIcon = NULL;
		ass.hCursor = 0;
		ass.hbrBackground = 0;
		ass.lpszMenuName = NULL;
		ass.lpszClassName = windowClassName;
		
		RegisterClass(&ass);
	}

	myBitsPerPixel = BitsPerPixel;
	myIcon = NULL;
	myCursor = NULL;
	cursorInside = false;

	s32 _width = myWidth = settings.Width;
	s32 _height = myHeight = settings.Height;
	
	DWORD _style = WS_VISIBLE;
	switch(Style)
	{
	default: case STYLE_NO_BORDER: _style |=	WS_POPUP; break;
	case STYLE_TITLEBAR_ONLY: _style |=			WS_CAPTION; break;
	case STYLE_CLOSE_ONLY: _style |=			WS_CAPTION | WS_SYSMENU; break;
	case STYLE_CLOSE_MINIMIZE: _style |=		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX; break;
	case STYLE_RESIZABLE: _style |=				WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX; break;
	}

	if( ! (_style == STYLE_FULLSCREEN || _style == STYLE_FULLSCREEN_AUTOSIZE) )
	{
		RECT r = {0, 0, _width, _height};
		AdjustWindowRect(&r, _style, false);
		_width  = r.right - r.left;
		_height = r.bottom - r.top;
	}

	myWndHandle = CreateWindow(windowClassName, Title, _style, CW_USEDEFAULT, CW_USEDEFAULT, _width, _height, NULL, NULL, GetModuleHandle(NULL), this);

	if(!myWndHandle)
	{
		if(windowsCount == 0) UnregisterClass(windowClassName, GetModuleHandle(NULL));
		return;
	}

	if(_style == STYLE_FULLSCREEN || _style == STYLE_FULLSCREEN_AUTOSIZE)
	{
		DEVMODE devmode;
		devmode.dmSize = sizeof(devmode);
		devmode.dmPelsWidth = settings.Width;
		devmode.dmPelsHeight = settings.Height;
		devmode.dmBitsPerPel = myBitsPerPixel;
		devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		if(ChangeDisplaySettings(&devmode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			DestroyWindow(myWndHandle);
			if(windowsCount == 0) UnregisterClass(windowClassName, GetModuleHandle(NULL));
			return;
		}

		// Make the window flags compatible with fullscreen mode
		SetWindowLong(myWndHandle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		SetWindowLong(myWndHandle, GWL_EXSTYLE, WS_EX_APPWINDOW);

		// Resize the window so that it fits the entire screen
		SetWindowPos(myWndHandle, HWND_TOP, 0, 0, settings.Width, settings.Height, SWP_FRAMECHANGED);
		ShowWindow(myWndHandle, SW_SHOW);
	}
	
	myDeviceContext = GetDC(myWndHandle);
	
	if(!myDeviceContext)
	{
		DestroyWindow(myWndHandle);
		if(windowsCount == 0) UnregisterClass(windowClassName, GetModuleHandle(NULL));
		return;
	}
	
	ZeroMemory(&myPixelFormat, sizeof(myPixelFormat));
	myPixelFormat.nSize = sizeof(myPixelFormat);
	myPixelFormat.nVersion = 1;
	myPixelFormat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL; 
	if(settings.DoubleBuffer) myPixelFormat.dwFlags |= PFD_DOUBLEBUFFER;
	myPixelFormat.iPixelType = PFD_TYPE_RGBA;
	myPixelFormat.cColorBits = settings.ColourBits;
	myPixelFormat.cStencilBits = settings.StencilBits;
	myPixelFormat.cDepthBits = settings.DepthBits;
	myPixelFormat.iLayerType = PFD_MAIN_PLANE;
	
	if( ! SetPixelFormat(myDeviceContext, ChoosePixelFormat(myDeviceContext, &myPixelFormat), &myPixelFormat))
	{
		ReleaseDC(myWndHandle, myDeviceContext);
		DestroyWindow(myWndHandle);
		if(windowsCount == 0) UnregisterClass(windowClassName, GetModuleHandle(NULL));
		return;
	}
	
	myRenderContext = wglCreateContext(myDeviceContext);

	if(!myRenderContext)
	{
		ReleaseDC(myWndHandle, myDeviceContext);
		DestroyWindow(myWndHandle);
		if(windowsCount == 0) UnregisterClass(windowClassName, GetModuleHandle(NULL));
		return;
	}

	RECT r;
	GetClientRect(myWndHandle, &r);
	myWidth = r.right - r.left;
	myHeight = r.bottom - r.top;

	ShowMouseCursor();
	
	++windowsCount;
	Activate();
}

WindowPlat::~WindowPlat()
{
	--windowsCount;

	CloseWindow(myWndHandle);
	
	if(IsActive())
		wglMakeCurrent(NULL, NULL);

	if(myIcon)
		DestroyIcon(myIcon);

	if(myRenderContext)
		wglDeleteContext(myRenderContext);

	if(myWndHandle)
	{
		if(myDeviceContext)
			ReleaseDC(myWndHandle, myDeviceContext);
		DestroyWindow(myWndHandle);
	}

	if(windowsCount == 0)
		UnregisterClass(windowClassName, GetModuleHandle(NULL));
}

void WindowPlat::ShowMouseCursor(bool show)
{
	if(show)
		myCursor = LoadCursor(NULL, IDC_ARROW);
	else
		myCursor = NULL;

	SetCursor(myCursor);
}

void WindowPlat::SetCursorLoc(s32 x, s32 y)
{
	POINT pos = {x, y};
	ClientToScreen(myWndHandle, &pos);
	SetCursorPos(pos.x, pos.y);
}

void WindowPlat::SetWindowLocation(s32 x, s32 y)
{
	SetWindowPos(myWndHandle, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void WindowPlat::SetSize(s32 width, s32 height)
{
	RECT r = {0, 0, width, height};
	AdjustWindowRect(&r, GetWindowLong(myWndHandle, GWL_STYLE), false);
	width = r.right - r.left;
	height = r.bottom - r.top;
	SetWindowPos(myWndHandle, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

bool WindowPlat::SetIcon(s32 width, s32 height, const u8* pixels)
{
	if(myIcon)
		DestroyIcon(myIcon);

	//Convert pixels {RGBA} to window's pixels {BGRA}
	s32 numPixels = width*height*4;
	u8* bgraPixels = new u8[numPixels];
	for(s32 i = 0; i < numPixels; i += 4)
	{
		bgraPixels[i] = pixels[i+2];	//R -> B
		bgraPixels[i+1] = pixels[i+1];	//G -> G
		bgraPixels[i+2] = pixels[i];	//B -> R
		bgraPixels[i+3] = pixels[i+3];	//A -> A
	}

	//Create the icon
	myIcon = CreateIcon(GetModuleHandle(NULL), width, height, 1, 32, NULL, bgraPixels);

	//Set the icon
	if(myIcon)
	{
		SendMessage(myWndHandle, WM_SETICON, ICON_BIG, (LPARAM)myIcon);
		SendMessage(myWndHandle, WM_SETICON, ICON_SMALL, (LPARAM)myIcon);
	}
	else return false;

	return true;
}

bool WindowPlat::PollEvent(EventHandle& Event)
{
	if(events.empty())
	{
		MSG msg;
		while(PeekMessage(&msg, myWndHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if(!events.empty())
	{
		Event = EventHandle(*events[0]);

		delete events[0];
		events.erase(events.begin());
		return true;
	}

	return false;
}

void WindowPlat::ProcessEvent(UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (myWndHandle == NULL)
		return;

	switch(msg)
	{
		//window destroyed
		case WM_DESTROY :
		{
			if(this == currentFullscreenWindow)
			{
				ChangeDisplaySettings(NULL, 0);
				currentFullscreenWindow = NULL;
			}
			ShowMouseCursor(true);
			break;
		}

		//cursor changed
		case WM_SETCURSOR :
		{
			if(LOWORD(lparam) == HTCLIENT)
				SetCursor(myCursor);
			break;
		}

		//window closed
		case WM_CLOSE :
		{
			EventHandle* e = new EventHandle();
			e->Type = EVENT_CLOSE;
			events.push_back(e);
			break;
		}

		//window resized
		case WM_SIZE :
		{
			if(wparam != SIZE_MINIMIZED)
			{
				RECT r;
				GetClientRect(myWndHandle, &r);
				myWidth = r.right - r.left;
				myHeight = r.bottom - r.top;

				EventHandle* e = new EventHandle();
				e->Type = EVENT_RESIZE;
				e->windowWidth = myWidth;
				e->windowHeight = myHeight;
				events.push_back(e);
			}
			break;
		}

		//gained focus to window
		case WM_SETFOCUS :
		{
			EventHandle* e = new EventHandle();
			e->Type = EVENT_FOCUS_GAINED;
			events.push_back(e);
			break;
		}

		//lost focus to window
		case WM_KILLFOCUS :
		{
			EventHandle* e = new EventHandle();
			e->Type = EVENT_FOCUS_LOST;
			events.push_back(e);
			break;
		}

		//keyboard text entered
		case WM_CHAR :
		{
			if(true || (lparam & (1 << 30)) == 0)
			{
				EventHandle* e = new EventHandle();
				e->Type = EVENT_KEYBOARD;
				e->key = KEY_NULL;
				e->key_charcode = static_cast<u32>(wparam);
				events.push_back(e);
			}
			break;
		}

		//keyboard key down
		case WM_KEYDOWN :
		case WM_SYSKEYDOWN :
		{
			if(true || (HIWORD(lparam) & KF_REPEAT) == 0)
			{
				EventHandle* e = new EventHandle();
				e->Type = EVENT_KEYDOWN;
				e->key_alt = HIWORD(GetAsyncKeyState(VK_MENU)) != 0;
				e->key_ctrl = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
				e->key_shift = HIWORD(GetAsyncKeyState(VK_SHIFT)) != 0;
				e->key_charcode = static_cast<u32>(wparam);
				e->key = VK2Key(wparam, lparam);
				events.push_back(e);
			}
			break;
		}

		//keyboard key up
		case WM_KEYUP :
		case WM_SYSKEYUP :
		{
			EventHandle* e = new EventHandle();
			e->Type = EVENT_KEYUP;
			e->key_alt = HIWORD(GetAsyncKeyState(VK_MENU)) != 0;
			e->key_ctrl = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
			e->key_shift = HIWORD(GetAsyncKeyState(VK_SHIFT)) != 0;
			e->key_charcode = static_cast<u32>(wparam);
			e->key = VK2Key(wparam, lparam);
			events.push_back(e);
			break;
		}

		//mousewheel moved
		case WM_MOUSEWHEEL :
		{
			POINT pos;
			pos.x = static_cast<s16>(LOWORD(lparam));
			pos.y = static_cast<s16>(HIWORD(lparam));
			ScreenToClient(myWndHandle, &pos);

			EventHandle* e = new EventHandle();
			e->Type = EVENT_MOUSEWHEEL;
			e->mouseWheelDelta = static_cast<s16>(HIWORD(wparam))/120;
			e->mouseX = static_cast<s16>(pos.x);
			e->mouseY = static_cast<s16>(pos.y);
			events.push_back(e);
			break;
		}

		//mouse left down
		case WM_LBUTTONDOWN :
		{
			EventHandle* e = new EventHandle();
			e->Type = EVENT_MOUSEDOWN;
			e->mouseButton = MOUSEBUTTON_LEFT;
			e->mouseX = static_cast<s16>(LOWORD(lparam));
			e->mouseY = static_cast<s16>(HIWORD(lparam));
			events.push_back(e);
			break;
		}
		//mouse left ip
		case WM_LBUTTONUP :
		{
			EventHandle* e = new EventHandle();
			e->Type = EVENT_MOUSEUP;
			e->mouseButton = MOUSEBUTTON_LEFT;
			e->mouseX = static_cast<s16>(LOWORD(lparam));
			e->mouseY = static_cast<s16>(HIWORD(lparam));
			events.push_back(e);
			break;
		}

		//mouse right down
		case WM_RBUTTONDOWN :
		{
			EventHandle* e = new EventHandle();
			e->Type = EVENT_MOUSEDOWN;
			e->mouseButton = MOUSEBUTTON_RIGHT;
			e->mouseX = static_cast<s16>(LOWORD(lparam));
			e->mouseY = static_cast<s16>(HIWORD(lparam));
			events.push_back(e);
			break;
		}
		//mouse right ip
		case WM_RBUTTONUP :
		{
			EventHandle* e = new EventHandle();
			e->Type = EVENT_MOUSEUP;
			e->mouseButton = MOUSEBUTTON_RIGHT;
			e->mouseX = static_cast<s16>(LOWORD(lparam));
			e->mouseY = static_cast<s16>(HIWORD(lparam));
			events.push_back(e);
			break;
		}

		//mouse middle down
		case WM_MBUTTONDOWN :
		{
			EventHandle* e = new EventHandle();
			e->Type = EVENT_MOUSEDOWN;
			e->mouseButton = MOUSEBUTTON_MIDDLE;
			e->mouseX = static_cast<s16>(LOWORD(lparam));
			e->mouseY = static_cast<s16>(HIWORD(lparam));
			events.push_back(e);
			break;
		}
		//mouse middle up
		case WM_MBUTTONUP :
		{
			EventHandle* e = new EventHandle();
			e->Type = EVENT_MOUSEUP;
			e->mouseButton = MOUSEBUTTON_MIDDLE;
			e->mouseX = static_cast<s16>(LOWORD(lparam));
			e->mouseY = static_cast<s16>(HIWORD(lparam));
			events.push_back(e);
			break;
		}

		//mouse X down
		case WM_XBUTTONDOWN :
		{
			EventHandle* e = new EventHandle();
			e->Type = EVENT_MOUSEDOWN;
			e->mouseButton = HIWORD(wparam) == XBUTTON1 ? MOUSEBUTTON_X1 : MOUSEBUTTON_X2;
			e->mouseX = static_cast<s16>(LOWORD(lparam));
			e->mouseY = static_cast<s16>(HIWORD(lparam));
			events.push_back(e);
			break;
		}
		//mouse X up
		case WM_XBUTTONUP :
		{
			EventHandle* e = new EventHandle();
			e->Type = EVENT_MOUSEUP;
			e->mouseButton = HIWORD(wparam) == XBUTTON1 ? MOUSEBUTTON_X1 : MOUSEBUTTON_X2;
			e->mouseX = static_cast<s16>(LOWORD(lparam));
			e->mouseY = static_cast<s16>(HIWORD(lparam));
			events.push_back(e);
			break;
		}

		//mouse moved
		case WM_MOUSEMOVE :
		{
			EventHandle* e = NULL;

			if(!cursorInside)
			{
				TRACKMOUSEEVENT mouseEvent;
				mouseEvent.cbSize = sizeof(mouseEvent);
				mouseEvent.dwFlags = TME_LEAVE;
				mouseEvent.hwndTrack = myWndHandle;
				TrackMouseEvent(&mouseEvent);

				cursorInside = true;

				e = new EventHandle();
				e->Type = EVENT_MOUSEENTERED;
				events.push_back(e);
			}

			e = new EventHandle();
			e->Type = EVENT_MOUSEMOVED;
			e->mouseX = static_cast<s16>(LOWORD(lparam));
			e->mouseY = static_cast<s16>(HIWORD(lparam));
			events.push_back(e);
			break;
		}

		//mouse leaved
		case WM_MOUSELEAVE :
		{
			cursorInside = false;

			EventHandle* e = new EventHandle();
			e->Type = EVENT_MOUSELEAVE;
			events.push_back(e);
			break;
		}
	}
}

/* static */ LRESULT CALLBACK WindowPlat::OnEvent(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//Create a link between the Window instance and OS handle on creation.
	if(msg == WM_CREATE)
	{
		SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams );
	}
	
	//Get window instance from handle
	WindowPlat* window = reinterpret_cast<WindowPlat*>(GetWindowLongPtr(handle, GWLP_USERDATA));
	
	//process event
	if(window)
		window->ProcessEvent(msg, wparam, lparam);
	
	//We don't forward the WM_CLOSE message, we want to prevent the OS from automatically destroying the window
	if(msg == WM_CLOSE)
		return 0;
	
	return DefWindowProc(handle, msg, wparam, lparam);
}

Key WindowPlat::VK2Key(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
		case VK_SHIFT		: return (static_cast<UINT>((lparam & (0xFF << 16)) >> 16) == MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC) ? KEY_SHIFT_L : KEY_SHIFT_R);
		case VK_MENU		: return (HIWORD(lparam) & KF_EXTENDED) ? KEY_ALT_R : KEY_ALT_L;
		case VK_CONTROL		: return (HIWORD(lparam) & KF_EXTENDED) ? KEY_CONTROL_R : KEY_CONTROL_L;

		case VK_LWIN		: return KEY_SYSTEM_L;
		case VK_RWIN		: return KEY_SYSTEM_R;

		case VK_APPS		: return KEY_MENU;
		case VK_OEM_1		: return KEY_SEMICOLON;
		case VK_OEM_2		: return KEY_SLASH;
		case VK_OEM_PLUS	: return KEY_EQUAL;
		case VK_OEM_MINUS	: return KEY_DASH;
		case VK_OEM_4		: return KEY_BRACKET_L;
		case VK_OEM_6		: return KEY_BRACKET_R;
		case VK_OEM_COMMA	: return KEY_COMMA;
		case VK_OEM_PERIOD	: return KEY_PERIOD;
		case VK_OEM_7		: return KEY_QUOTE;
		case VK_OEM_5		: return KEY_BACKSLASH;
		case VK_OEM_3		: return KEY_TILDE;
		case VK_ESCAPE		: return KEY_ESCAPE;
		case VK_SPACE		: return KEY_SPACE;
		case VK_RETURN		: return KEY_ENTER;
		case VK_BACK		: return KEY_BACKSPACE;
		case VK_TAB			: return KEY_TAB;
		case VK_PRIOR		: return KEY_PAGE_UP;
		case VK_NEXT		: return KEY_PAGE_DOWN;
		case VK_END			: return KEY_END;
		case VK_HOME		: return KEY_HOME;
		case VK_INSERT		: return KEY_INSERT;
		case VK_DELETE		: return KEY_DELETE;
		case VK_ADD			: return KEY_ADD;
		case VK_SUBTRACT	: return KEY_SUBTRACT;
		case VK_MULTIPLY	: return KEY_MULTIPLY;
		case VK_DIVIDE		: return KEY_DIVIDE;
		case VK_PAUSE		: return KEY_PAUSE;
		case VK_F1		: return KEY_F1;
		case VK_F2		: return KEY_F2;
		case VK_F3		: return KEY_F3;
		case VK_F4		: return KEY_F4;
		case VK_F5		: return KEY_F5;
		case VK_F6		: return KEY_F6;
		case VK_F7		: return KEY_F7;
		case VK_F8		: return KEY_F8;
		case VK_F9		: return KEY_F9;
		case VK_F10		: return KEY_F10;
		case VK_F11		: return KEY_F11;
		case VK_F12		: return KEY_F12;
		case VK_F13		: return KEY_F13;
		case VK_F14		: return KEY_F14;
		case VK_F15		: return KEY_F15;
		case VK_LEFT	: return KEY_LEFT;
		case VK_RIGHT	: return KEY_RIGHT;
		case VK_UP		: return KEY_UP;
		case VK_DOWN	: return KEY_DOWN;
		case VK_NUMPAD0	: return KEY_NUMPAD0;
		case VK_NUMPAD1	: return KEY_NUMPAD1;
		case VK_NUMPAD2	: return KEY_NUMPAD2;
		case VK_NUMPAD3	: return KEY_NUMPAD3;
		case VK_NUMPAD4	: return KEY_NUMPAD4;
		case VK_NUMPAD5	: return KEY_NUMPAD5;
		case VK_NUMPAD6	: return KEY_NUMPAD6;
		case VK_NUMPAD7	: return KEY_NUMPAD7;
		case VK_NUMPAD8	: return KEY_NUMPAD8;
		case VK_NUMPAD9	: return KEY_NUMPAD9;
		case '0'		: return KEY_NUM0;
		case '1'		: return KEY_NUM1;
		case '2'		: return KEY_NUM2;
		case '3'		: return KEY_NUM3;
		case '4'		: return KEY_NUM4;
		case '5'		: return KEY_NUM5;
		case '6'		: return KEY_NUM6;
		case '7'		: return KEY_NUM7;
		case '8'		: return KEY_NUM8;
		case '9'		: return KEY_NUM9;
		case 'A'		: return KEY_A;
		case 'B'		: return KEY_B;
		case 'C'		: return KEY_C;
		case 'D'		: return KEY_D;
		case 'E'		: return KEY_E;
		case 'F'		: return KEY_F;
		case 'G'		: return KEY_G;
		case 'H'		: return KEY_H;
		case 'I'		: return KEY_I;
		case 'J'		: return KEY_J;
		case 'K'		: return KEY_K;
		case 'L'		: return KEY_L;
		case 'M'		: return KEY_M;
		case 'N'		: return KEY_N;
		case 'O'		: return KEY_O;
		case 'P'		: return KEY_P;
		case 'Q'		: return KEY_Q;
		case 'R'		: return KEY_R;
		case 'S'		: return KEY_S;
		case 'T'		: return KEY_T;
		case 'U'		: return KEY_U;
		case 'V'		: return KEY_V;
		case 'W'		: return KEY_W;
		case 'X'		: return KEY_X;
		case 'Y'		: return KEY_Y;
		case 'Z'		: return KEY_Z;
	}
	return KEY_NULL;
}
