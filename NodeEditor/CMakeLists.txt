cmake_minimum_required(VERSION 3.10)
set(MODULE NodeEditor)
project(${MODULE})

set(wxWidgets_USE_UNICODE ON)
set(wxWidgets_CONFIGURATION gtk3u)
find_package(wxWidgets REQUIRED html net core base xrc adv xml propgrid richtext)
include(${wxWidgets_USE_FILE})

set(CMAKE_CXX_STANDARD 17)

file (GLOB SourceFiles *.cpp)

set(LIB_SOURCES
    ${SourceFiles}
)

link_directories(/usr/local/NodeFlow/lib)
add_executable(${MODULE} ${LIB_SOURCES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#
target_link_libraries(${MODULE} NodeFlow ${wxWidgets_LIBRARIES}  ${Boost_LIBRARIES} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS}  )
#
install( TARGETS ${MODULE} DESTINATION bin)
