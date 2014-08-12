#pragma once
#include "Platform.h"

namespace sys
{

//Resizable array.
template<typename T>
class List
{
public:
	List()
	{
		size = 0;
		cap = 1;
		arr = reinterpret_cast<T*>( calloc(cap, sizeof(T)) );
	}
	List(u32 Size)
	{
		size = 0;
		Resize(Size);
	}
	~List()
	{
		free(arr);
	}

	//Get the count of added elements.
	inline u32 Count() { return size; }

	//Get the maximum capacity of elements allocated.
	inline u32 Capacity() { return cap; }

	//Add an item to the end of the list.
	void Add(const T &item)
	{
		if(size >= cap)
			upsize(); //reached capacity, double size...

		arr[size] = item;

		++size;
	}

	//Remove the first instance of an item. (use removeDuplicates to delete all instances)
	void Remove(const T &item, bool removeDuplicates = false)
	{
		for(u32 i = 0; i < size; ++i)
		{
			if(item == arr[i])
			{
				for(u32 j = i; j < size; ++j)
				{
					if(j+1 >= size)
						arr[j] = NULL;
					else
						arr[j] = arr[j+1];
				}
				--size;
				if(!removeDuplicates)
					break;
			}
		}
	}
	
	//Retrieve reference to item in array
	inline T& At(u32 spot)
	{
		return arr[spot];
	}

	//Add an item to a specific spot. Pushes other above items up the list.
	void Insert(const T &item, u32 spot)
	{
		if(spot >= size)
		{
			Add(item);
			return;
		}

		if(size >= cap)
			upsize(); //reached capacity, double size...

		for(u32 i = size; i > spot; --i)
			arr[i] = arr[i-1];
		
		arr[spot] = item;
		++size;
	}

	//Remove an item at a specific spot. Pushes above items down.
	void RemoveAt(u32 spot)
	{
		if(spot >= size)
			return;

		for(u32 i = spot; i < size; ++i)
		{
			if(i+1 >= size)
				arr[i] = NULL;
			else
				arr[i] = arr[i+1];
		}
		--size;
	}

	//Remove all items from the list and free memory.
	void Clear()
	{
		free(arr);
		size = 0;
		cap = 1;
		arr = reinterpret_cast<T*>( calloc(cap, sizeof(T)) );
	}

	//Removes excess allocated memory that is not in use. (This will force Capacity down to Count, freeing unused memory)
	inline void Compact()
	{
		Resize(size);
	}

	//Get the address of the raw array. Use Capacity() to get the size of the array.
	inline T* const GetArray()
	{
		return arr;
	}

	//Resizes the list capacity. This will remove any items that are beyond the new size.
	void Resize(u32 newSize)
	{
		T* newarr = reinterpret_cast<T*>( calloc(newSize, sizeof(T)) );
		for(u32 i = 0; i < size && i < newSize; ++i)
			newarr[i] = arr[i];
		free(arr);
		arr = newarr;
		cap = newSize;
	}

	//Is the list empty?
	inline bool IsEmpty()
	{
		return (arr == NULL || size == 0);
	}
	
	//Syntax-Candy for At()
	inline T& operator[](u32 spot)
	{
		return arr[spot];
	}

private:
	u32 size;
	u32 cap;

	T* arr;

	inline void upsize()
	{
		Resize(cap+cap);
	}
};

}