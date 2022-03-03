#Common CMAKE definitons for 
# where to install
# Get the common definitons at the higher level - BOOST
#
# Common CMAKE definitions
set(CMAKE_CXX_STANDARD 17)
# Set project vaiables
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})  # Set the project source code location (location of this file)
# If the install dir changes check Common/common.h
set(CMAKE_INSTALL_PREFIX ${PIROOT}/usr/local/NodeFlow)
message("Install Dir: ${CMAKE_INSTALL_PREFIX}")

# where the components go
set(EXE_DIR bin)
set(LIB_DIR lib)
set(PLUGINS_DIR modules)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)
#
# Debug and trace options
# enable this to insert breakpoints into throws and catches
add_definitions(-DTRAP_ENABLE)
# Enable debug trace in build
#add_definitions(-DTRACE_ON)
# NB: may on module by module basis use - target_compile_options(my_library PRIVATE -Werror -Wall -Wextra)
SET(CMAKE_CXX_FLAGS "-Wno-psabi ${CMAKE_CXX_FLAGS}")
#
IF( DEFINED CROSSBUILD )
#when building for cross compiling wxWidgets cannot be auto detected
    include_directories(${PIROOT}/usr/local/include)
    include_directories(${PIROOT}/usr/local/lib/wx/include/gtk3-unicode-3.1)
    include_directories(${PIROOT}/usr/local/include/wx-3.1)
     add_definitions(-DWXUSINGDLL -D__WXGTK3__ -D__WXGTK__)
    set(wxWidgets_LIBRARIES wx_gtk3u_xrc-3.1 wx_gtk3u_html-3.1 wx_gtk3u_qa-3.1
        wx_gtk3u_core-3.1 wx_baseu_xml-3.1 wx_baseu_net-3.1 wx_baseu-3.1
        wx_gtk3u_propgrid-3.1 wx_gtk3u_richtext-3.1)


    # Find BOOST seems to have problem with sysroots so explicitly set the libraries
    set(Boost_LIBRARIES -lboost_system -lboost_thread -lboost_program_options)
    SET(BOOST_ROOT ${PIROOT}/usr)
    SET(BOOST_DIR ${BOOST_ROOT})
    SET(BOOST_PREFIX ${BOOST_DIR})
    SET(BOOST_LIBRARYDIR ${PIROOT}/usr/lib/arm-linux-gnueabihf )
    SET(BOOST_INCLUDEDIR ${PIROOT}/usr/include/boost)

    # Force the linker to look in the sysroot where the libraries actually are - needed for some of the cmake tests
    SET (PI_LINKER_FLAGS "-Wl,--no-as-needed -lcrypt" )
    SET(CMAKE_EXE_LINKER_FLAGS "${PI_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS} " )
    SET(CMAKE_SHARED_LINKER_FLAGS "${PI_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS} ")

    
    add_definitions(-Wall )
    add_definitions(-DwxSIZE_T_IS_UINT)
    add_definitions(-DHAVE_VSNPRINTF_DECL)
    add_definitions(-DHAVE_VSSCANF_DECL)
    add_definitions(-DHAVE_SNPRINTF_DECL)
    add_definitions(-D_FILE_OFFSET_BITS=64)
    add_definitions(-DRASPBERRY_PI_BUILD)
    add_definitions(-DMYSQLPP_MYSQL_HEADERS_BURIED)

ELSE( DEFINED CROSSBUILD )
    # Include wxWidgets directory
    set(wxWidgets_USE_UNICODE ON)
    set(wxWidgets_CONFIGURATION gtk3u)
    find_package(wxWidgets REQUIRED html net core base xrc adv xml propgrid richtext)
    include(${wxWidgets_USE_FILE})
    add_definitions(-DMYSQLPP_MYSQL_HEADERS_BURIED)
# Set a variable with all the required wxWdiget library linker flags
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
#
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

find_package(Boost REQUIRED COMPONENTS system thread program_options)

# add boost to target
if(Boost_FOUND)
    include_directories(${BOOST_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
    add_definitions(${Boost_DEFINITIONS})
    include_directories(${Boost_INCLUDE_DIRS})  
endif()

IF( DEFINED RASPBERRY_PI )
    add_definitions(-DRASPBERRY_PI_BUILD)
endif()
    SET (LINKER_FLAGS "-lcrypt" )
    SET(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS} " )
    SET(CMAKE_SHARED_LINKER_FLAGS "${LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS} ")


ENDIF( DEFINED CROSSBUILD )

#enable debug tracing
#add_definitions(-DTRACE_ON)
#
# enable the use of Wt , web UI support
add_definitions(-DUSE_WT)
set(WTLIBS  -lwt -lwthttp)
#
link_directories(/usr/local/MRL/lib)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(/usr/local/MRL/include)
#
# Enable the GUI build
add_definitions(-DUSE_GUI )
set(EXTRA_LIB_FLAGS MrlLib open62541cpp sqlite3 mysqlpp modbus mosquittopp socket++ atomic pthread rt m)


