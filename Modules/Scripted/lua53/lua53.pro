#-------------------------------------------------
#
# Project created by QtCreator 2015-10-24T09:00:41
#
#-------------------------------------------------

QT       -= core gui
CONFIG += release
TARGET = lua53
TEMPLATE = lib

DEFINES += LUA53_LIBRARY LUA_COMPAT_5_2
unix: DEFINES += LUA_USE_LINUX
win32:DEFINES += LUA_BUILD_AS_DLL
unix:VERSION = 5.3
SOURCES += \
    lapi.c \
    lauxlib.c \
    lbaselib.c \
    lbitlib.c \
    lcode.c \
    lcorolib.c \
    lctype.c \
    ldblib.c \
    ldebug.c \
    ldo.c \
    ldump.c \
    lfunc.c \
    lgc.c \
    linit.c \
    llex.c \
    liolib.c \
    lmathlib.c \
    lmem.c \
    loadlib.c \
    lobject.c \
    lopcodes.c \
    loslib.c \
    lparser.c \
    lstate.c \
    lstring.c \
    lstrlib.c \
    ltable.c \
    ltablib.c \
    ltm.c \
    lundump.c \
    lutf8lib.c \
    lvm.c \
    lzio.c

HEADERS +=\
    lapi.h \
    lauxlib.h \
    lcode.h \
    lctype.h \
    ldebug.h \
    ldo.h \
    lfunc.h \
    lgc.h \
    llex.h \
    llimits.h \
    lmem.h \
    lobject.h \
    lopcodes.h \
    lparser.h \
    lprefix.h \
    lstate.h \
    lstring.h \
    ltable.h \
    ltm.h \
    lua.h \
    lua.hpp \
    luaconf.h \
    lualib.h \
    lundump.h \
    lvm.h \
    lzio.h


unix {

    options = $$find(QMAKESPEC, "rasp-pi")
    count(options,1) {
        target.path =  $$[QT_SYSROOT]/usr/local/lib
        headers.path = $$[QT_SYSROOT]/usr/local/include/lua

    } else {
        target.path =  /usr/local/lib
        headers.path = /usr/local/include/lua
    }

}

# Set the output path to match build environment
#
win32 {

        target.path =  C:/usr/local/lib
        headers.path = C:/usr/local/include/lua
}


headers.files = $$HEADERS
INSTALLS += headers target
