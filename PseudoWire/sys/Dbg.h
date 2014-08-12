#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <crtdbg.h>
#include <time.h>

#include "Stopwatch.h"
#include "Platform.h"

#ifdef IEF_PLATFORM_WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

namespace sys
{
class Dbg
{
public:

	static inline bool DEBUGMODE()
	{
#ifdef IEF_DEBUG
		return true;
#else
		return false;
#endif
	}

	//Print to console.
	static /*inline*/ void Printf( const char* format, ... )
	{
		va_list argptr;
		va_start(argptr, format);
		vprintf(format, argptr);
	}
	
	//Print to console.
	static /*inline*/ void PrintfDBG( const char* format, ... )
	{
#ifdef IEF_DEBUG
		va_list argptr;
		va_start(argptr, format);
		vprintf(format, argptr);
#endif
	}

	//Clear console.
	static void Cls()
	{
#ifdef IEF_PLATFORM_WIN32
		
		// std::system("cls");

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coordScreen = { 0, 0 };    /* here's where we'll home the
											cursor */ 
		DWORD cCharsWritten;
		CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
		DWORD dwConSize;                 /* number of character cells in
											the current buffer */

		/* get the number of character cells in the current buffer */
		GetConsoleScreenBufferInfo( hConsole, &csbi );
		dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

		/* fill the entire screen with blanks */
		FillConsoleOutputCharacter( hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten );

		/* get the current text attribute */
		GetConsoleScreenBufferInfo( hConsole, &csbi );

		/* now set the buffer's attributes accordingly */
		FillConsoleOutputAttribute( hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten );

		/* put the cursor at (0, 0) */
		SetConsoleCursorPosition( hConsole, coordScreen );
#else
		//TODO assume posix
		std::system("clear");
#endif
	}
	
	//Simple assert test.
	static inline void Assert( bool test )
	{
#ifdef IEF_DEBUG
		assert(test);
#endif
	}
	
	//Print to console and perform assert test.
	static inline void Assert( bool test, const char* format, ... )
	{
		if(!test)
		{
			va_list argptr;
			va_start(argptr, format);
			vprintf(format, argptr);
		}
#ifdef IEF_DEBUG
		assert(test);
#endif
	}

	//Pause console, only enabled during DEBUG.
	static inline void PauseDBG()
	{
#ifdef IEF_DEBUG
		getchar();
#endif
	}

	//Pause console.
	static inline void Pause()
	{
		getchar();
	}
	
	//Sleep thread. (Windows Precise)
	static inline void Halt(f64 milliseconds)
	{
#ifdef IEF_PLATFORM_WIN32
		//Windows' Sleep() resolution is 1MS (or even 15.625ms), so we (unfortunately) have to hang execution if less than 1MS
		sys::HRTimer sw;
		f64 a;
		sw.Start();
		while((a=sw.ElapsedMS()) < milliseconds)
		{
			if(milliseconds-a > 15.625)
				Sleep(1); //let thread sleep a little
		}
#else
		//milliseconds -> microseconds
		usleep(static_cast<useconds_t>(milliseconds*1000.0));
#endif
	}
	
	//Sleep thread.
	static inline void Halt(u32 milliseconds)
	{
#ifdef IEF_PLATFORM_WIN32
/*
		//Windows' Sleep() resolution is 1MS (or even 15.625ms), so we (unfortunately) have to hang execution if less than 1MS
		sys::HRTimer sw;
		u32 a;
		sw.Start();
		while((a=static_cast<u32>(sw.ElapsedMS())) < milliseconds)
		{
			if(milliseconds-a > 15)
				Sleep(1); //let thread sleep a little
		}
*/
		Sleep(milliseconds);
#else
		usleep(static_cast<useconds_t>(milliseconds*1000));
#endif
	}

	//Hang execution for a certain amount of time, only enabled during DEBUG.
	static inline void HangDBG(f64 milliseconds)
	{
#ifdef IEF_DEBUG
		sys::HRTimer sw;
		sw.Start();
		while(sw.ElapsedMS() < milliseconds) ;
#endif
	}

	//Hang execution for a certain amount of time.
	static inline void Hang(f64 milliseconds)
	{
		sys::HRTimer sw;
		sw.Start();
		while(sw.ElapsedMS() < milliseconds)
		{
			//nothing...
		}
	}

	//Get the OS Time.
	inline static s64 OSTime() 
	{ 
		return time(NULL); 
	}
};
}