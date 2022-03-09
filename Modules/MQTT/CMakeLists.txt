cmake_minimum_required(VERSION 3.10)
set (MODULE MQTT)
project(${MODULE})

set(CMAKE_CXX_STANDARD 17)
file (GLOB HeaderFiles *.h)
file (GLOB SourceFiles *.cpp)
add_library(${MODULE} SHARED ${SourceFiles} )
#
set(PROJECT_LIB_FLAGS NodeFlow MrlLib)
target_link_libraries(${MODULE} ${wxWidgets_LIBRARIES}  ${Boost_LIBRARIES} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS})
install(TARGETS ${MODULE} DESTINATION ${PLUGINS_DIR})
