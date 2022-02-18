# This is the cross compile target definition file
#
# cmake .. -DCMAKE_TOOLCHAIN_FILE=rpi.cmake -DCMAKE_BUILD_TYPE=Debug
#
# Define our host system
set(BOOST_DEBUG OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CROSSBUILD ON)
set(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)
# Adjust to suit actual environment
SET(DEVROOT ${CMAKE_CURRENT_SOURCE_DIR})
# where the sysroot is
SET(PIROOT /Development/sysroot)
# where the development tools are
SET(PITOOLS /usr/bin)
#
# Define the cross compiler locations
SET(CMAKE_C_COMPILER   ${PITOOLS}/arm-linux-gnueabihf-gcc-8)
SET(CMAKE_CXX_COMPILER ${PITOOLS}/arm-linux-gnueabihf-g++-8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_C_FLAGS "-march=armv8-a -mtune=cortex-a53 -mfpu=crypto-neon-fp-armv8 ${CMAKE_C_FLAGS}"  )
SET(CMAKE_CXX_FLAGS "-march=armv8-a -mtune=cortex-a53 -mfpu=crypto-neon-fp-armv8 ${CMAKE_CXX_FLAGS}")

# Define the sysroot path for the RaspberryPi distribution in our tools folder
SET(CMAKE_SYSROOT ${PIROOT}) 
SET(CMAKE_FIND_ROOT_PATH ${PIROOT})
SET(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-isystem")
#
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Search for libraries and headers in the target directories only
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# Search for the headers in the target and host directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
#
 
