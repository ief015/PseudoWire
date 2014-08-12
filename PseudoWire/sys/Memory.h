#pragma once

#include <cstring>

namespace sys
{

class Memory
{
public:
	
	// Reinterpret T_In value as T_Out without type casting.
	// May become unstable or unexpected data may be found if T_In is smaller than T_Out.
	// Loss of data may occur if T_In is larger than T_Out.
	// long i = Reinterpret<long>(10.0f); i == 1092616192
	template<typename T_Out, typename T_In>
	static inline T_Out Reinterpret_Ref(T_In& val)
	{
		return *reinterpret_cast<T_Out*>(&val);
	}

	// Reinterpret T_In value as T_Out without type casting.
	// May become unstable or unexpected data may be found if T_In is smaller than T_Out.
	// Loss of data may occur if T_In is larger than T_Out.
	// long i = Reinterpret<long>(10.0f); i == 1092616192
	template<typename T_Out, typename T_In>
	static inline T_Out Reinterpret(T_In val)
	{
		return *reinterpret_cast<T_Out*>(&val);
	}
	
	//Memory Copy (memcpy)
	static inline void Copy(void* dest, void* src, size_t size)
	{
		memcpy(dest, src, size);
	}
	
	//Memory Set (memset)
	static inline void Set(void* dest, int value, size_t size)
	{
		memset(dest, value, size);
	}
	
	//Memory Compare (memcmp)
	static inline int Compare(void* ptrA, void* ptrB, size_t size)
	{
		return memcmp(ptrA, ptrB, size);
	}
	
	//Memory Compare (memcmp)
	static inline bool Equals(void* ptrA, void* ptrB, size_t size)
	{
		return memcmp(ptrA, ptrB, size)==0;
	}
	
};

}