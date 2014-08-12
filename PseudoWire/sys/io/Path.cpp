#include "Path.h"
#include "../StringHelper.h"
#include <vector>
#include "../Platform.h"

#ifdef IEF_PLATFORM_WIN32
	#ifdef _MSC_VER //Microsoft Visual C++ does not have dirent.h
		#include "../externs/dirent.h"
	#else
		#include <dirent.h>
	#endif
	#include <windows.h>
#else
	#include <dirent.h>
#endif
#include <io.h> //not sys::io... gotta be careful with this

using namespace sys;
using namespace io;
// ==========================================
// ------------------------------ class Path:
// ==========================================

Path::Path()
{
}

Path::Path(const char* directory)
{
	ChangeDirectory(directory);
}

s32 Path::ChangeDirectory(const char* directory)
{
	DIR* d = opendir(directory);
	if(d == NULL)
	{
		return 1;
	}
	closedir(d);

#ifdef IEF_PLATFORM_WIN32
	GetFullPathName(directory, IEF_MAX_PATH, fullpath, NULL);
#else
#error not implemented
#endif

	strcpy(fullpath, StringHelper::Replace(fullpath, '\\', '/').c_str());

	return 0;
}


s32 Path::GetAllFiles(std::vector<File> & files)
{
	DIR* dp = opendir(fullpath);
	if(dp == NULL) return 1; //probably doesn't exist
	
	dirent* dirp;

	while((dirp = readdir(dp)) != NULL)
	{
		if(dirp->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue; //not a directory

		File f(dirp->d_name, Path(fullpath), (dirp->data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0, (dirp->data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0);
		files.push_back(f);
	}

	closedir(dp);
	return 0;
}

s32 Path::GetAllDirs(std::vector<Path> & dirs)
{
	DIR* dp = opendir(fullpath);
	if(dp == NULL) return 1; //probably doesn't exist
	
	dirent* dirp;

	while((dirp = readdir(dp)) != NULL)
	{
		if( sys::StringHelper::Equals(dirp->d_name, ".") || sys::StringHelper::Equals(dirp->d_name, "..") ) continue;
		if(!(dirp->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) continue; //not a directory
		
		Path p( (std::string(fullpath) + "/" + dirp->d_name).c_str() );
		dirs.push_back(p);
	}

	closedir(dp);
	return 0;
}

std::string Path::GetName() const
{
	std::vector<std::string> s;
	sys::StringHelper::Split( fullpath, '/', s );
	return s[s.size()-1];
}

const char* Path::GetFullPath() const
{
	return fullpath;
}

// ==========================================
// ------------------------------ class File:
// ==========================================

File::File(const char* fullname, Path& path, bool hidden, bool readOnly)
{
	this->fullName = fullname;
	this->fileLoc = path;
	this->fileHidden = hidden;
	this->fileReadOnly = readOnly;
}

std::string File::GetName()
{
	std::vector<std::string> p; sys::StringHelper::Split(fullName, '.', p);

	if(p.size() > 1) p.pop_back();
	
	std::stringstream name;
	for(u32 i = 0; i < p.size(); ++i)
	{
		name << p[i];
		if(i != p.size()-1)
			name << '.';
	}

	return name.str();
}

std::string File::GetExtension()
{
	std::vector<std::string> p; sys::StringHelper::Split(fullName, '.', p);
	
	if(p.size() > 1)
	{
		return p[p.size()-1];
	}

	return std::string(""); //no extension found
}

std::string File::GetFullPath() const
{
	return std::string(fileLoc.GetFullPath()) + "/" + fullName;
}