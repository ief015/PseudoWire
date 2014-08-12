#pragma once

#include <vector>
#include "../Platform.h"

#ifdef IEF_PLATFORM_WIN32 //Microsoft Visual C++ does not have dirent.h
# include <windows.h>
# define IEF_MAX_PATH MAX_PATH+1
#else
# error not implemented
#endif

namespace sys
{
namespace io
{

class File;

class Path
{
protected:
	char fullpath[IEF_MAX_PATH];

public:
	Path();
	Path(const char* directory);

	//Find all files in path. Returns 0 if successful.
	s32 GetAllFiles(std::vector<File> & files);

	//Find all directories in path. Returns 0 if successful.
	s32 GetAllDirs(std::vector<Path> & dirs);

	//Get full path string.
	const char* GetFullPath() const;

	//Get only the name of the path.
	std::string GetName() const;

	//Change directory. Returns 0 if successful.
	s32 ChangeDirectory(const char* directory);

	//TODO: Move this directory Up a level. Returns 0 if successful.
//	s32 MoveUp(unsigned numOfLevels = 1 );

	//TODO: Move this directory into a directory. Returns 0 if successful.
//	s32 MoveInto(const char* dir);
};

class File
{
protected:
	Path fileLoc;
	bool fileHidden;
	bool fileReadOnly;
	std::string fullName;

public:
	File(const char* fullname, Path& path, bool hidden = false, bool readOnly = false);
	
	//The file's full name
	inline std::string GetFullName() { return fullName; }

	//The file's path
	std::string GetFullPath() const;

	//Get the name of the file.
	std::string GetName();

	//Get the extension of the file.
	std::string GetExtension();

	//Path where the file is located.
	inline const Path* const GetPath() { return &fileLoc; }

	//Is the file hidden?
	inline bool IsHidden() { return fileHidden; }

	//Is the file read-only?
	inline bool IsReadOnly() { return fileReadOnly; }
};

}
}