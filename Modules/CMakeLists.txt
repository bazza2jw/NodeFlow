cmake_minimum_required(VERSION 3.10)
project(Modules)

set(CMAKE_CXX_STANDARD 17)
#
# Add each of the modules
#
add_subdirectory(FlowBase)
add_subdirectory(MQTT)

# LUA scripting is a possibility - but implimenting new nodes in C++ is easier
# Scripting is work in progress
#add_subdirectory(Scripted)




