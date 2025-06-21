cmake_minimum_required(VERSION 3.5)
project (SlotProject VERSION 1.0.0 LANGUAGES CXX )

# Currently CMake (since 2.8.5) supports generation of compilation databases
# for Unix Makefile builds (Ninja builds in the works) with the option
# CMAKE_EXPORT_COMPILE_COMMANDS.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#=== SETTING VARIABLES ===#
# Appending to existing flags the correct way (two methods)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
# string(APPEND CMAKE_CXX_FLAGS " -Wall -Werror")

# Add include dir
include_directories(src/include)

#=== Main App ===
set( APP_NAME "zombiedice" )
add_executable( ${APP_NAME}
  src/dice_bag.cpp
  src/game_controller.cpp
  src/ini_parser.cpp
  src/test.cpp
  src/zdie.cpp
)
target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib )
target_compile_features( ${APP_NAME}  PUBLIC cxx_std_17 )
