#pragma once

extern "C" {
#include "lua/lua.h"
#include "lua/lauxlib.h"
}

// prints the whole lua stack to the console
static int lua_stackdump(lua_State *L, const char* usermsg = NULL)
{
	static int dumpcount = 0;
	int size = lua_gettop(L);
	printf(">>> LUA STACK DUMP #%i [LUA:0x%p] Size: %i (%s)\n", ++dumpcount, L, size, usermsg != NULL ? usermsg : "");
	for(int i = -1; -i <= size; --i)
	{
		switch(lua_type(L, i))
		{
		case LUA_TNUMBER: printf("%i:\t[number]\t%f\n", -i, lua_tonumber(L, i)); break;
		case LUA_TBOOLEAN: printf("%i:\t[boolean]\t%s\n", -i, lua_toboolean(L, i) ? "true" : "false"); break;
		case LUA_TSTRING: printf("%i:\t[string]\t%s\n", -i, lua_tostring(L, i)); break;
		case LUA_TTABLE: printf("%i:\t[table]\t\t0x%p\n", -i, lua_topointer(L, i)); break;
		case LUA_TFUNCTION: printf("%i:\t[function]\t0x%i\n", -i, lua_topointer(L, i)); break;
		case LUA_TTHREAD: printf("%i:\t[thread]\t0x%p\n", -i, lua_tothread(L, i)); break;
		case LUA_TLIGHTUSERDATA: printf("%i:\t[lightuserdata]\t0x%p\n", -i, lua_touserdata(L, i)); break;
		case LUA_TUSERDATA: printf("%i:\t[userdata]\t0x%p\n", -i, lua_touserdata(L, i)); break;
		case LUA_TNIL: printf("%i:\t[nil]\n", -i); break;
		default: printf("%i:\t[???]\n", -i); break;
		}
	}
	printf("\n");
	return 0;
}

// prints the elements of a table at the top of the stack
static void lua_tabledump(lua_State *L)
{
	printf(">>> LUA TABLE DUMP [LUA:0x%p TBL:0x%p] Size: %i\n", L, lua_topointer(L, -1), lua_objlen(L, -1));

	for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1))
	{
		// -1 is key, -2 is value
		switch(lua_type(L, -1))
		{
		case LUA_TNUMBER:
			if(lua_type(L, -2) == LUA_TNUMBER)
				printf("%i:\t[number]\t%f\n", (int)lua_tonumber(L, -2), lua_tonumber(L, -1));
			else
				printf("%s:\t[number]\t%f\n", lua_tostring(L, -2), lua_tonumber(L, -1));
			break;
		case LUA_TBOOLEAN:
			if(lua_type(L, -2) == LUA_TNUMBER)
				printf("%i:\t[boolean]\t%s\n", (int)lua_tonumber(L, -1), lua_toboolean(L, -1) ? "true" : "false");
			else
				printf("%s:\t[boolean]\t%s\n", lua_tostring(L, -2), lua_toboolean(L, -1) ? "true" : "false");
			break;
		case LUA_TSTRING:
			if(lua_type(L, -2) == LUA_TNUMBER)
				printf("%i:\t[string]\t%s\n", (int)lua_tonumber(L, -2), lua_tostring(L, -1));
			else
				printf("%s:\t[string]\t%s\n", lua_tostring(L, -2), lua_tostring(L, -1));
			break;
		case LUA_TTABLE:
			if(lua_type(L, -2) == LUA_TNUMBER)
				printf("%i:\t[table]\t\t0x%p\n", (int)lua_tonumber(L, -2), lua_topointer(L, -1));
			else
				printf("%s:\t[table]\t\t0x%p\n", lua_tostring(L, -2), lua_topointer(L, -1));
			break;
		case LUA_TFUNCTION:
			if(lua_type(L, -2) == LUA_TNUMBER)
				printf("%i:\t[function]\t0x%p\n", (int)lua_tonumber(L, -2), lua_topointer(L, -1));
			else
				printf("%s:\t[function]\t0x%p\n", lua_tostring(L, -2), lua_topointer(L, -1));
			break;
		case LUA_TTHREAD:
			if(lua_type(L, -2) == LUA_TNUMBER)
				printf("%i:\t[thread]\t0x%p\n", (int)lua_tonumber(L, -2), lua_tothread(L, -1));
			else
				printf("%s:\t[thread]\t0x%p\n", lua_tostring(L, -2), lua_tothread(L, -1));
			break;
		case LUA_TLIGHTUSERDATA:
			if(lua_type(L, -2) == LUA_TNUMBER)
				printf("%i:\t[lightuserdata]\t0x%p\n", (int)lua_tonumber(L, -2), lua_touserdata(L, -1));
			else
				printf("%s:\t[lightuserdata]\t0x%p\n", lua_tostring(L, -2), lua_touserdata(L, -1));
			break;
		case LUA_TUSERDATA:
			if(lua_type(L, -2) == LUA_TNUMBER)
				printf("%i:\t[userdata]\t0x%p\n", (int)lua_tonumber(L, -2), lua_touserdata(L, -1));
			else
				printf("%s:\t[userdata]\t0x%p\n", lua_tostring(L, -2), lua_touserdata(L, -1));
			break;
		case LUA_TNIL:
			if(lua_type(L, -2) == LUA_TNUMBER)
				printf("%i:\t[nil]\n", (int)lua_tonumber(L, -1));
			else
				printf("%s:\t[nil]\n", lua_tostring(L, -1));
			break;
		default:
			if(lua_type(L, -2) == LUA_TNUMBER)
				printf("%i:\t[???]\n", (int)lua_tonumber(L, -1));
			else
				printf("%s:\t[???]\n", lua_tostring(L, -1));
			break;
		}
	}
	
	printf("\n");
}

/*
   This is a modified version of the Luna Wrapper
   http://lua-users.org/wiki/LunaWrapper
*/

/*
 * All classes that use Luna must include the following public members:
	static const char className[];
	static const Luna<T>::RegType registerMethods[];
 * All classes that use Luna must also contain this constructer:
	constructor(lua_State *L)
 * 
 * class Luna<T>
 * Simplifies the binding of C++ classes into lua.
**/
template <typename T> class Luna {
  typedef struct { T *pT; } userdataType;
public:
  typedef int (T::*mfp)(lua_State *L);
  typedef struct { const char *name; mfp mfunc; } RegType;

  // register the class into the lua state
  static void Register(lua_State *L) {
	lua_newtable(L);
	int methods = lua_gettop(L);

	luaL_newmetatable(L, T::className);
	int metatable = lua_gettop(L);

	// store method table in globals so that
	// scripts can add functions written in Lua.
	lua_pushstring(L, T::className);
	lua_pushvalue(L, methods);
	lua_settable(L, LUA_GLOBALSINDEX);

	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, methods);
	lua_settable(L, metatable);  // hide metatable from Lua getmetatable()

	lua_pushliteral(L, "__index");
	lua_pushvalue(L, methods);
	lua_settable(L, metatable);

	lua_pushliteral(L, "__tostring");
	lua_pushcfunction(L, tostring_T);
	lua_settable(L, metatable);

	lua_pushliteral(L, "__gc");
	lua_pushcfunction(L, gc_T);
	lua_settable(L, metatable);

	lua_newtable(L);                // mt for method table
	int mt = lua_gettop(L);
	lua_pushliteral(L, "__call");
	lua_pushcfunction(L, new_T);
	lua_pushliteral(L, "new");
	lua_pushvalue(L, -2);           // dup new_T function
	lua_settable(L, methods);       // add new_T to method table
	lua_settable(L, mt);            // mt.__call = new_T
	lua_setmetatable(L, methods);

	// fill method table with methods from class T
	for (const RegType *l = T::registerMethods; l->name; ++l) {
	/* edited by Snaily: shouldn't it be const RegType *l ... ? */
	  lua_pushstring(L, l->name);
	  lua_pushlightuserdata(L, (void*)l);
	  lua_pushcclosure(L, thunk, 1);
	  lua_settable(L, methods);
	}

	lua_pop(L, 2);  // drop metatable and method table
  }
  
  // push a lua object of the registered class (you must Register() first)
  static int pushobject(lua_State *L, T* obj) {
	userdataType *ud =  // create new user data
		static_cast<userdataType*>(lua_newuserdata(L, sizeof(userdataType*)));
	ud->pT = obj;  // set pointer to object
	luaL_getmetatable(L, T::className);  // load metatable to userdata
	lua_setmetatable(L, -2);
	return 1;  // push new userdata object
  }

  // get userdata from Lua stack and return pointer to T object. if invalid, error occurs
  static T *check(lua_State *L, int narg) {
	userdataType *ud =
	  static_cast<userdataType*>(luaL_checkudata(L, narg, T::className));
	if(!ud) luaL_typerror(L, narg, T::className);
	return ud->pT;  // pointer to T object
  }

  // get userdata from Lua stack and return pointer to T object. if invalid, returns NULL
  static T *totype(lua_State* L, int narg) {
	userdataType *ud =
	  static_cast<userdataType*>(luaL_toudata(L, narg, T::className));
	if(ud) return ud->pT;  // pointer to T object
	else return NULL;  // return NULL if incorrect userdata
  }

private:
  Luna();  // hide default constructor

  // member function dispatcher
  static int thunk(lua_State *L) {
	// stack has userdata, followed by method args
	T *obj = check(L, 1);  // get 'self', or if you prefer, 'this'
	lua_remove(L, 1);  // remove self so member function args start at index 1
	// get member function from upvalue
	RegType *l = static_cast<RegType*>(lua_touserdata(L, lua_upvalueindex(1)));
	return (obj->*(l->mfunc))(L);  // call member function
  }

  // create a new T object and
  // push onto the Lua stack a userdata containing a pointer to T object
  static int new_T(lua_State *L) {
	lua_remove(L, 1);   // use classname:new(), instead of classname.new()
	T *obj = new T(L);  // call constructor for T objects
	userdataType *ud =
	  static_cast<userdataType*>(lua_newuserdata(L, sizeof(userdataType)));
	ud->pT = obj;  // store pointer to object in userdata
	luaL_getmetatable(L, T::className);  // lookup metatable in Lua registry
	lua_setmetatable(L, -2);
	return 1;  // userdata containing pointer to T object
  }

  // garbage collection metamethod
  static int gc_T(lua_State *L) {
	userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
	T *obj = ud->pT;
	delete obj;  // call destructor for T objects
	return 0;
  }

  static int tostring_T (lua_State *L) {
	char buff[32];
	userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
	T *obj = ud->pT;
	sprintf(buff, "%p", obj);
	lua_pushfstring(L, "%s (%s)", T::className, buff);
	return 1;
  }
};
