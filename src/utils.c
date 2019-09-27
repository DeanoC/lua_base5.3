#include "al2o3_platform/platform.h"
#include "lua_base5.3/utils.h"
#include "lua_base5.3/lua.h"
#include "lua_base5.3/lualib.h"
#include "lua_base5.3/lauxlib.h"
#include "al2o3_vfile/vfile.h"

static AL2O3_THREAD_LOCAL void *readBuffer = NULL;
static AL2O3_THREAD_LOCAL size_t readBufferSize =  (10 * 1024);

static char const *readerFunc(lua_State *L, void *ud, size_t *sz) {
	VFile_Handle handle = (VFile_Handle) ud;
	if (readBuffer == NULL) {
		LuaBase_SetLoadBufferSize(L, readBufferSize);
	}
	*sz = VFile_Read(handle, readBuffer, readBufferSize);
	return (char const *) readBuffer;
}

static void openStdLibs (lua_State *L) {
	luaL_requiref(L, "_G", luaopen_base, 1);
	luaL_requiref(L, "package", luaopen_package, 1);
	lua_pop(L, 2);  /* remove results from previous calls */
	LuaBase_RegisterLib(L, "coroutine", luaopen_coroutine);
	LuaBase_RegisterLib(L, "table", luaopen_table);
	LuaBase_RegisterLib(L, "io", luaopen_io);
	LuaBase_RegisterLib(L, "os", luaopen_os);
	LuaBase_RegisterLib(L, "string", luaopen_string);
	LuaBase_RegisterLib(L, "math", luaopen_math);
	LuaBase_RegisterLib(L, "utf8", luaopen_utf8);
	LuaBase_RegisterLib(L, "debug", luaopen_debug);
}

static void *allocFunc(void *ud, void *ptr, size_t osize, size_t nsize) {
	Memory_Allocator const* allocator = (Memory_Allocator const *)ud;

	if (ptr == NULL) {
		// alloc
		if (nsize != 0) {
			return MEMORY_ALLOCATOR_CALLOC(allocator, 1, nsize);
		} else {
			return NULL;
		}
	} else {
		// resize or free
		if (nsize == 0) {
			// free
			MEMORY_ALLOCATOR_FREE(allocator, ptr);
			return NULL;
		} else {
			// resize
			return MEMORY_ALLOCATOR_REALLOC(allocator, ptr, nsize);
		}
	}
}


AL2O3_EXTERN_C void LuaBase_RegisterLib(lua_State *L, const char *name, lua_CFunction f) {
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");  /* get 'package.preload' */
	lua_pushcfunction(L, f);
	lua_setfield(L, -2, name);  /* package.preload[name] = f */
	lua_pop(L, 2);  /* pop 'package' and 'preload' tables */
}

AL2O3_EXTERN_C lua_State* LuaBase_Create() {
	return LuaBase_CreateWithAllocator(&Memory_GlobalAllocator);
}

AL2O3_EXTERN_C lua_State* LuaBase_CreateWithAllocator(Memory_Allocator const* allocator) {
	lua_State *L = lua_newstate(&allocFunc, (void*)allocator);
	openStdLibs(L);
	return L;
}

AL2O3_EXTERN_C void LuaBase_Destroy(lua_State* L) {
	void* ud = NULL;
	lua_Alloc func = lua_getallocf(L, &ud);
	if(ud != NULL) {
		Memory_Allocator const* allocator = (Memory_Allocator const *)ud;
		MEMORY_ALLOCATOR_FREE(allocator, readBuffer);
		readBuffer = NULL;
	}
	lua_close(L);
}

AL2O3_EXTERN_C void LuaBase_SetLoadBufferSize(lua_State* L, size_t size) {
	void* ud = NULL;
	lua_Alloc func = lua_getallocf(L, &ud);
	if(ud != NULL) {
		Memory_Allocator const* allocator = (Memory_Allocator const *)ud;
		readBuffer = MEMORY_ALLOCATOR_MALLOC(allocator, size);
		readBufferSize = size;
	}
}

AL2O3_EXTERN_C bool LuaBase_ExecuteScript(lua_State *L, VFile_Handle handle) {
	if (lua_load(L, &readerFunc, handle, VFile_GetName(handle), NULL) ||
			lua_pcall(L, 0, 0, 0)) {
		LOGERROR("Lua script loading error %s", lua_tostring(L, -1));
		return false;
	};
	return true;
}
