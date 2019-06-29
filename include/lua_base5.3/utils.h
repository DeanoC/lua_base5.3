#pragma once
#ifndef LUA_BASE5_3_UTILS_H_
#define LUA_BASE5_3_UTILS_H_

#include "lua_base5.3/lua.h"
#include "al2o3_memory/memory.h"
#include "al2o3_vfile/vfile.h"

AL2O3_EXTERN_C void LuaBase_RegisterLib(lua_State *L, const char *name, lua_CFunction f);
AL2O3_EXTERN_C lua_State* LuaBase_Create();
AL2O3_EXTERN_C lua_State* LuaBase_CreateWithAllocator(Memory_Allocator const* allocator);
AL2O3_EXTERN_C void LuaBase_Destroy(lua_State* L);

AL2O3_EXTERN_C void LuaBase_SetLoadBufferSize(lua_State* L, size_t size);
AL2O3_EXTERN_C bool LuaBase_ExecuteScript(lua_State *L, VFile_Handle handle);

#endif