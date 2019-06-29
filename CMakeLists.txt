cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
get_directory_property(hasParent PARENT_DIRECTORY)
if(NOT hasParent)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out_libs)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out_bin)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out_bin)
	set(LIB_BASE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/libs/)
	include(FetchContent)
	FetchContent_Declare( al2o3 GIT_REPOSITORY https://github.com/DeanoC/al2o3 GIT_TAG master )
	FetchContent_GetProperties(al2o3)
	if(NOT al2o3_POPULATED)
		FetchContent_Populate(al2o3)
		add_subdirectory(${al2o3_SOURCE_DIR} ${al2o3_BINARY_DIR})
	endif()
endif ()

SET_MIN_VERSIONS()
set(LibName lua_base5.3)
project(${LibName})

set(Interface
		luaconf.h
		lua.h
		lua.hpp
		lualib.h
		lauxlib.h
		utils.h
		)

set(Src
		utils.c
		lapi.c
		lauxlib.c
		lbaselib.c
		lbitlib.c
		lcode.c
		lcorolib.c
		lctype.c
		ldblib.c
		ldebug.c
		ldo.c
		ldump.c
		lfunc.c
		lgc.c
		linit.c
		liolib.c
		llex.c
		lmathlib.c
		lmem.c
		loadlib.c
		lobject.c
		lopcodes.c
		loslib.c
		lparser.c
		lstate.c
		lstring.c
		lstrlib.c
		ltable.c
		ltablib.c
		ltm.c
		lua.c
		luac.c
		lundump.c
		lutf8lib.c
		lvm.c
		lzio.c
		lapi.h
		lcode.h
		lctype.h
		ldebug.h
		ldo.h
		lfunc.h
		lgc.h
		llex.h
		llimits.h
		lmem.h
		lobject.h
		lopcodes.h
		lparser.h
		lprefix.h
		lstate.h
		lstring.h
		ltable.h
		ltm.h
		lundump.h
		lvm.h
		lzio.h
		)

set(Deps
		al2o3_platform
		al2o3_memory
		al2o3_vfile
		)
ADD_LIB(${LibName} "${Interface}" "${Src}" "${Deps}")


