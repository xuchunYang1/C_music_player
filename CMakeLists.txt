# This is a templet of cmake config file
# Author YangZG 2017-05-10
# TODO:Auto Detect compile environment
# Brief: (sign '#' )
#		(1) Mutiple thread program need have these configures:
#			1) #find_package(Threads)
#			2) #target_link_libraries(FPGA_camera ${CMAKE_THREAD_LIBS_INIT})
#		(2) Debug your executable 
#			1) if you have no idea about ->gdb<-,learn how to use it
#			2) in this project,if you want active debug mode,Use Macro (IDEBUG) or have following configuretions,
#			..Debug configuretions:
#									#SET(CMAKE_BUILD_TYPE "Debug") 
#									#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
#									#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
#
#
#

cmake_minimum_required(VERSION 2.8)

set(CMAKE_SYSTEM_NAME Linux)

# Define project name
project(mplayer)

# gdb mode debug
#SET(CMAKE_BUILD_TYPE "Debug") 
#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -W -Wall -g -ggdb -m32")
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -W -Wall -m32")

SET(CMAKE_C_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -W -Wall -g -ggdb -m32")
SET(CMAKE_C_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -W -Wall -m32")

# set compile flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

#设置包含路径
set(USER_INCLUDES include)
#查找多线程库
find_package(Threads)

# Add headers location to your include paths
include_directories(${USER_INCLUDES})

# Declare the executable target built from your sources
add_executable(
				mplayer
				main.c
				music.c
				manipulate_sqlite3.c
			  )

# Link your application with OpenCV libraries and other necessary libs(such as :libpthread ... libopengl ... libxxx)
target_link_libraries(mplayer sqlite3)#link libsqlite3 and libm(math)
target_link_libraries(mplayer ${CMAKE_THREAD_LIBS_INIT})
