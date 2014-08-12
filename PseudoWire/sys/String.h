#pragma once
#include <string>
#include "StringHelper.h"

#include "RawArray.h"
#include "Platform.h"

namespace sys
{
	// Wrapper for std::string
	class String
	{
	private:
		std::string str;

	public:
		String() { }
		String(const String& s) { str = s; }
		String(const std::string& s) { str = s; }
		String(const char* s) { str = s; }
		String(void* ptr) { str = StringHelper::ToStr(ptr); }
		String(s8 c) { str = StringHelper::ToStr(c); }
		String(u8 v) { str = StringHelper::ToStr(v); }
		String(s16 v) { str = StringHelper::ToStr(v); }
		String(u16 v) { str = StringHelper::ToStr(v); }
		String(s32 v) { str = StringHelper::ToStr(v); }
		String(u32 v) { str = StringHelper::ToStr(v); }
		String(s64 v) { str = StringHelper::ToStr(v); }
		String(u64 v) { str = StringHelper::ToStr(v); }
		String(f32 v) { str = StringHelper::ToStr(v); }
		String(f64 v) { str = StringHelper::ToStr(v); }

		/*
		* Methods
		*/

		// Is the string empty?
		inline bool IsEmpty() const { return str.empty(); }

		// Clear the string.
		inline void Clear() { str.clear(); }

		// Get the beinning iterator.
		inline std::string::iterator Begin() { return str.begin(); }
		// Get the beginning constant iterator.
		inline std::string::const_iterator Begin() const { return str.begin(); }
		// Get the ending iterator.
		inline std::string::iterator End() { return str.end(); }
		// Get the ending constant iterator.
		inline std::string::const_iterator End() const { return str.end(); }

		// Get the length of the string.
		inline size_t Length() { return str.length(); }

		// Pop the last character in the string.
		inline void Pop() { str.pop_back(); }

		// Pop the last n characters in the string.
		void Pop(s32 n) { for(int i = 0; i < n; ++i) Pop(); }

		/*
		* void Set
		* Assign the string.
		*/

		inline void Set(const String& s) { str.assign(s.Str_C()); }
		inline void Set(const std::string& s) { str.assign(s); }
		inline void Set(const char* s) { str.assign(s); }
		inline void Set(void* ptr) { str.assign(StringHelper::ToStr(ptr)); }
		inline void Set(s8 c) { str.assign(StringHelper::ToStr(c)); }
		inline void Set(u8 v) { str.assign(StringHelper::ToStr(v)); }
		inline void Set(s16 v) { str.assign(StringHelper::ToStr(v)); }
		inline void Set(u16 v) { str.assign(StringHelper::ToStr(v)); }
		inline void Set(s32 v) { str.assign(StringHelper::ToStr(v)); }
		inline void Set(u32 v) { str.assign(StringHelper::ToStr(v)); }
		inline void Set(s64 v) { str.assign(StringHelper::ToStr(v)); }
		inline void Set(u64 v) { str.assign(StringHelper::ToStr(v)); }
		inline void Set(f32 v) { str.assign(StringHelper::ToStr(v)); }
		inline void Set(f64 v) { str.assign(StringHelper::ToStr(v)); }

		/*
		* void Append
		* Concatenates string.
		*/

		inline void Append(const String& s) { str.append(s.Str_C()); }
		inline void Append(const std::string& s) { str.append(s); }
		inline void Append(const char* s) { str.append(s); }
		inline void Append(void* ptr) { str.append(StringHelper::ToStr(ptr)); }
		inline void Append(s8 c) { str.append(StringHelper::ToStr(c)); }
		inline void Append(u8 v) { str.append(StringHelper::ToStr(v)); }
		inline void Append(s16 v) { str.append(StringHelper::ToStr(v)); }
		inline void Append(u16 v) { str.append(StringHelper::ToStr(v)); }
		inline void Append(s32 v) { str.append(StringHelper::ToStr(v)); }
		inline void Append(u32 v) { str.append(StringHelper::ToStr(v)); }
		inline void Append(s64 v) { str.append(StringHelper::ToStr(v)); }
		inline void Append(u64 v) { str.append(StringHelper::ToStr(v)); }
		inline void Append(f32 v) { str.append(StringHelper::ToStr(v)); }
		inline void Append(f64 v) { str.append(StringHelper::ToStr(v)); }

		/*
		* void Insert
		* Insert a value at a specific spot in the string.
		*/

		inline void Insert(const String& s, s32 spot) { str.insert(spot, s.Str_C()); }
		inline void Insert(const std::string& s, s32 spot) { str.insert(spot, s); }
		inline void Insert(const char* s, s32 spot) { str.insert(spot, s); }
		inline void Insert(void* ptr, s32 spot) { str.insert(spot, StringHelper::ToStr(ptr)); }
		inline void Insert(s8 c, s32 spot) { str.insert(spot, StringHelper::ToStr(c)); }
		inline void Insert(u8 v, s32 spot) { str.insert(spot, StringHelper::ToStr(v)); }
		inline void Insert(s16 v, s32 spot) { str.insert(spot, StringHelper::ToStr(v)); }
		inline void Insert(u16 v, s32 spot) { str.insert(spot, StringHelper::ToStr(v)); }
		inline void Insert(s32 v, s32 spot) { str.insert(spot, StringHelper::ToStr(v)); }
		inline void Insert(u32 v, s32 spot) { str.insert(spot, StringHelper::ToStr(v)); }
		inline void Insert(s64 v, s32 spot) { str.insert(spot, StringHelper::ToStr(v)); }
		inline void Insert(u64 v, s32 spot) { str.insert(spot, StringHelper::ToStr(v)); }
		inline void Insert(f32 v, s32 spot) { str.insert(spot, StringHelper::ToStr(v)); }
		inline void Insert(f64 v, s32 spot) { str.insert(spot, StringHelper::ToStr(v)); }

		/*
		* void Erase
		* Erase a value(s) at a specific spot in the string.
		*/

		inline void Erase(s32 spot) { str.erase(spot); }
		inline void Erase(s32 spot, s32 count) { str.erase(spot, count); }

		/*
		* void operator +=
		* Append/Assign the string. Syntax candy for Append()
		*/

		inline void operator += (const String& s) { Append(s); }
		inline void operator += (const std::string& s) { Append(s); }
		inline void operator += (const char* s) { Append(s); }
		inline void operator += (void* ptr) { Append(ptr); }
		inline void operator += (s8 c) { Append(c); }
		inline void operator += (u8 v) { Append(v); }
		inline void operator += (s16 v) { Append(v); }
		inline void operator += (u16 v) { Append(v); }
		inline void operator += (s32 v) { Append(v); }
		inline void operator += (u32 v) { Append(v); }
		inline void operator += (s64 v) { Append(v); }
		inline void operator += (u64 v) { Append(v); }
		inline void operator += (f32 v) { Append(v); }
		inline void operator += (f64 v) { Append(v); }
		
		/*
		* void operator +
		* Append the string. Syntax candy for Append()
		*/

		inline String operator + (const String& s) { String ret(*this); ret.Append(s); return ret; }
		inline String operator + (const std::string& s) { String ret(*this); ret.Append(s); return ret; }
		inline String operator + (const char* s) { String ret(*this); ret.Append(s); return ret; }
		inline String operator + (void* ptr) { String ret(*this); ret.Append(ptr); return ret; }
		inline String operator + (s8 c) { String ret(*this); ret.Append(c); return ret; }
		inline String operator + (u8 v) { String ret(*this); ret.Append(v); return ret; }
		inline String operator + (s16 v) { String ret(*this); ret.Append(v); return ret; }
		inline String operator + (u16 v) { String ret(*this); ret.Append(v); return ret; }
		inline String operator + (s32 v) { String ret(*this); ret.Append(v); return ret; }
		inline String operator + (u32 v) { String ret(*this); ret.Append(v); return ret; }
		inline String operator + (s64 v) { String ret(*this); ret.Append(v); return ret; }
		inline String operator + (u64 v) { String ret(*this); ret.Append(v); return ret; }
		inline String operator + (f32 v) { String ret(*this); ret.Append(v); return ret; }
		inline String operator + (f64 v) { String ret(*this); ret.Append(v); return ret; }

		/*
		* void operator +=
		* Append the string as a stream.
		* str << "twenty: " << 20;
		*/

		inline String& operator << (const String& s) { Append(s); return *this; }
		inline String& operator << (const std::string& s) { Append(s); return *this; }
		inline String& operator << (const char* s) { Append(s); return *this; }
		inline String& operator << (void* ptr) { Append(ptr); return *this; }
		inline String& operator << (s8 c) { Append(c); return *this; }
		inline String& operator << (u8 v) { Append(v); return *this; }
		inline String& operator << (s16 v) { Append(v); return *this; }
		inline String& operator << (u16 v) { Append(v); return *this; }
		inline String& operator << (s32 v) { Append(v); return *this; }
		inline String& operator << (u32 v) { Append(v); return *this; }
		inline String& operator << (s64 v) { Append(v); return *this; }
		inline String& operator << (u64 v) { Append(v); return *this; }
		inline String& operator << (f32 v) { Append(v); return *this; }
		inline String& operator << (f64 v) { Append(v); return *this; }

		/*
		* void operator =
		* Set the string. Syntax candy for Set()
		*/

		inline void operator = (const String& s) { Set(s); }
		inline void operator = (const std::string& s) { Set(s); }
		inline void operator = (const char* s) { Set(s); }
		inline void operator = (void* ptr) { Set(ptr); }
		inline void operator = (s8 c) { Set(c); }
		inline void operator = (u8 v) { Set(v); }
		inline void operator = (s16 v) { Set(v); }
		inline void operator = (u16 v) { Set(v); }
		inline void operator = (s32 v) { Set(v); }
		inline void operator = (u32 v) { Set(v); }
		inline void operator = (s64 v) { Set(v); }
		inline void operator = (u64 v) { Set(v); }
		inline void operator = (f32 v) { Set(v); }
		inline void operator = (f64 v) { Set(v); }

		/*
		* Casting operators.
		*/

		inline std::string& Str() { return str; }
		inline const char* C_Str() const { return str.c_str(); }
		inline const std::string& Str_C() const { return str; }

		inline operator std::string&() { return str; }
		inline operator const char*() const { return str.c_str(); }

		inline operator bool() const { return str.empty(); }
		inline bool operator !() const { return !str.empty(); }

		inline std::string::reference operator [](size_t idx) { return str[idx]; }
		inline std::string::const_reference operator [](size_t idx) const { return str[idx]; }
	};
}
