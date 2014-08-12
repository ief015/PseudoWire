#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "Platform.h"

namespace sys
{

class StringHelper
{
	public:
	
		#define IEF_TOSTR_SIZE 12
		#define IEF_TOSTR_SIZE_LONG 24
		
		//Compare C-Style strings.
		inline static bool Equals(const char * str1, const char * str2)
		{
			return strcmp(str1, str2) == 0;
		}

		//Split the string into a vector.
		static void Split(const std::string& str, char delim, std::vector<std::string> &elems, bool ignoreEmpties = false)
		{
			std::stringstream ss(str);
			std::string item;
			while(std::getline(ss, item, delim)) {
				if(ignoreEmpties && item.empty()) continue;
				elems.push_back(item);
			}
		}

		//Replace all "replaceThis" characters in "src" with "withThis".
		static std::string Replace(const std::string& src, char replaceThis, char withThis)
		{
			std::string ret = src;
			for(u32 i = 0; i < ret.length(); ++i)
			{
				if(ret[i] == replaceThis)
				{
					ret[i] = withThis;
				}
			}
			return ret;
		}



		static void ToStr(char* dest, s8 val)
		{
			sprintf(dest, "%c", val);
		}
		static std::string ToStr(s8 val)
		{
			char str[IEF_TOSTR_SIZE];
			sprintf(str, "%c", val);
			return std::string(str);
		}

		static void ToStr(char* dest, u8 val)
		{
			sprintf(dest, "%i", val);
		}
		static std::string ToStr(u8 val)
		{
			char str[IEF_TOSTR_SIZE];
			sprintf(str, "%i", val);
			return std::string(str);
		}

		static void ToStr(char* dest, s16 val)
		{
			sprintf(dest, "%i", val);
		}
		static std::string ToStr(s16 val)
		{
			char str[IEF_TOSTR_SIZE];
			sprintf(str, "%i", val);
			return std::string(str);
		}

		static void ToStr(char* dest, u16 val)
		{
			sprintf(dest, "%i", val);
		}
		static std::string ToStr(u16 val)
		{
			char str[IEF_TOSTR_SIZE];
			sprintf(str, "%i", val);
			return std::string(str);
		}

		static void ToStr(char* dest, s32 val)
		{
			sprintf(dest, "%i", val);
		}
		static std::string ToStr(s32 val)
		{
			char str[IEF_TOSTR_SIZE];
			sprintf(str, "%i", val);
			return std::string(str);
		}

		static void ToStr(char* dest, u32 val)
		{
			sprintf(dest, "%i", val);
		}
		static std::string ToStr(u32 val)
		{
			char str[IEF_TOSTR_SIZE];
			sprintf(str, "%i", val);
			return std::string(str);
		}

		static void ToStr(char* dest, s64 val)
		{
			sprintf(dest, "%i", val);
		}
		static std::string ToStr(s64 val)
		{
			char str[IEF_TOSTR_SIZE];
			sprintf(str, "%i", val);
			return std::string(str);
		}

		static void ToStr(char* dest, u64 val)
		{
			sprintf(dest, "%i", val);
		}
		static std::string ToStr(u64 val)
		{
			char str[IEF_TOSTR_SIZE];
			sprintf(str, "%i", val);
			return std::string(str);
		}

		static void ToStr(char* dest, f32 val)
		{
			sprintf(dest, "%f", val);
		}
		static std::string ToStr(f32 val)
		{
			char str[IEF_TOSTR_SIZE];
			sprintf(str, "%f", val);
			return std::string(str);
		}
		
		static void ToStr(char* dest, f64 val)
		{
			sprintf(dest, "%.16g", val);
		}
		static std::string ToStr(f64 val)
		{
			char str[IEF_TOSTR_SIZE_LONG];
			sprintf(str, "%.16g", val);
			return std::string(str);
		}
		
		static void ToStr(char* dest, void* ptr)
		{
			sprintf(dest, "%p", ptr);
		}
		static std::string ToStr(void* ptr)
		{
			char str[IEF_TOSTR_SIZE];
			sprintf(str, "%p", ptr);
			return std::string(str);
		}

};

}