cmake_minimum_required( VERSION 3.20 )
project ( FermiOwnCommon )
include(CTest)

set( FermiOwnCommon_VERSION_MAJOR 0 )
set( FermiOwnCommon_VERSION_MINOR 1 )

set(CMAKE_CXX_FLAGS "-O3 -march=native -std=c++20 -lm -Wall -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -march=native -std=c++20 -lm -Wall -O0 -g")

include_directories($ENV{EIGEN3_INCLUDE_DIR})
include_directories("src/Actions")
include_directories("src/Algorithms")
include_directories("src/Data")
include_directories("src/Math")
include_directories("src/System")
include_directories("apps")

add_subdirectory("src")
add_subdirectory("test")
add_subdirectory("apps")