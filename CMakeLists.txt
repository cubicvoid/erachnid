cmake_minimum_required(VERSION 3.17)
cmake_policy(SET CMP0100 NEW) # handle .hh files

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.11 CACHE STRING "Build for 10.11")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE Type Unspecified; picking Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

project(erachnid VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)


# use asan as an option (currently mac only)
option(USE_SANITIZER "Build and link with ASAN" FALSE)

# Copy on mac (could expand to other platforms)
option(COPY_AFTER_BUILD "Copy the clap to ~/Library on MACOS, ~/.clap on linux" FALSE)

add_subdirectory(lib/clap EXCLUDE_FROM_ALL)
add_subdirectory(src)
#add_subdirectory(lib/clap-helpers EXCLUDE_FROM_ALL)

