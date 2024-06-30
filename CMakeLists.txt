cmake_minimum_required(VERSION 3.22)
project(AIM_CPP)

set(CMAKE_CXX_STANDARD 17)



include_directories(/opt/homebrew/Cellar/boost/1.85.0/include)
include_directories(library)

aux_source_directory(mbi MBI_CFILE)


add_executable(AIM_CPP main.cpp
        ${MBI_CFILE}
        AIM.cpp
        AIM.h
)

MATH(EXPR stack_size "100  * 1024 * 1024") # 16 Mb
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-z,stack-size=${stack_size}")
