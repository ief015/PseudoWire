#pragma once
#include "Platform.h"

namespace sys
{

//Simple, lightweight array wrapper class.
template<typename T>
class ArrayOf
{
public:
	ArrayOf()
	: _arr(NULL)
	, _arr_sz(0)
	{
	}
	
	ArrayOf(s32 size)
	: _arr(NULL)
	, _arr_sz(0)
	{
		Create(size);
	}

	~ArrayOf()
	{
		if(_arr)
			delete[] _arr;
	}

	//Create a new array. (Automatically Kills an array if one exists)
	void Create(s32 size)
	{
		if(_arr)
			delete[] _arr;

		_arr = new T[size];

		_arr_sz = size;
	}

	//Delete the array from memory.
	void Kill()
	{
		if(_arr)
		{
			delete[] _arr;
			_arr_sz = 0;
			_arr = NULL;
		}
	}

	//Get constant pointer (to 'non-constant' data) to the array.
	T* const Ptr() const
	{
		return _arr;
	}

	//Get size of the array.
	inline s32 Size() const
	{
		return _arr_sz;
	}

	//Get item reference in array.
	inline T& operator [](s32 idx)
	{
		//TODO: Check if idx is in range (?)
		return _arr[idx];
	}

	//Is the array not valid?
	inline bool operator !() const
	{
		return _arr == NULL;
	}

	//Is the array valid?
	inline operator bool() const
	{
		return _arr != NULL;
	}

	//Is this array valid?
	inline bool IsValid() const
	{
		return _arr != NULL;
	}

private:
	T* _arr;
	s32 _arr_sz;
};

}
