#pragma once

#include "Path.h"



#ifdef _MSC_VER //Microsoft Visual C++ does not have dirent.h
	#include "../externs/dirent.h"
#else
	#include <dirent.h>
#endif
#include <io.h> //not sys::io... gotta be careful with this

namespace sys
{
namespace io
{

	//Check if a path (file or directory) exists.
	static bool Exists(const char* path)
	{
	#ifdef WIN32
		return _access(path, 0) != -1;
	#else
		return access(path, 0) != -1;
	#endif
	}

}
}