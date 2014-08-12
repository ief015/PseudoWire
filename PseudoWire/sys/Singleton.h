#pragma once

namespace sys
{

//Singleton class.
template <class T>
class Singleton
{
protected:

	Singleton()
	{
		getPtr();
	}

	static T* & getPtr()
	{
		static T* _instance = NULL;
		return _instance;
	}

public:

	static void CreateSingleton()
	{
		getPtr() = new T();
	}
	static void KillSingleton()
	{
		if(getPtr())
			delete getPtr();
		getPtr() = NULL;
	}

	inline static T& Ref()
	{
		return *getPtr();
	}
	inline static T* Ptr()
	{
		return getPtr();
	}
};

}
