cmake_minimum_required(VERSION 3.10)
set (MODULE Scripted)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
file (GLOB HeaderFiles *.h)
file (GLOB SourceFiles *.cpp)
file (GLOB LuaSourceFiles lua53/*.c)

add_library(${MODULE} SHARED ${SourceFiles} ${LuaSourceFiles} )
#
set(PROJECT_LIB_FLAGS NodeFlow MrlLib)

install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
