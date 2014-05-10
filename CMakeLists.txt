cmake_minimum_required (VERSION 2.6)
project (lcs)

include(CheckFunctionExists)
include(CheckIncludeFiles)

set(LCS_VERSION_STRING "4.8-git")

set(lcs_REQUIRED_LIBRARIES ncurses)

find_package(Curses REQUIRED)

add_definitions(-DCMAKE -DHAVE_CONFIG_H)

check_include_files(ncurses.h HAVE_NCURSES_H)
set(HAVE_WIDE_NCURSES 1)
set(NCURSES 1)
set(USE_NCURSES 1)
set(Linux 1)

configure_file(${lcs_SOURCE_DIR}/config.h.cmake ${lcs_SOURCE_DIR}/config.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g") #-Wall -pedantic -Wextra -g")

include_directories(${CURSES_INCLUDE_DIR})
include_directories(${lcs_SOURCE_DIR})
include_directories(${lcs_SOURCE_DIR}/src)
add_subdirectory(src)
