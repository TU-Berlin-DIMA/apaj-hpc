cmake_minimum_required(VERSION 3.17)
project(apaj_hpc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Custom CMake find instructions and macros
include(cmake/macros.cmake)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

# Log4cc
find_package(Log4cxx REQUIRED)
include_directories(${Log4cxx_INCLUDE_DIR})
set(LIBRARIES ${LIBRARIES} ${Log4cxx_LIBRARY})

# PAPI
find_package(PAPI REQUIRED)
message(STATUS "PAPI library found at ${PAPI_LIBRARIES}")
message(STATUS "PAPI headers found at ${PAPI_INCLUDE_DIRS}")
set(LIBRARIES ${LIBRARIES} ${PAPI_LIBRARIES})

# Libtensorflow C
find_library(TENSORFLOW_LIB tensorflow)


include_directories("include/")
include_directories("include/papi/")
include_directories(${PAPI_INCLUDE_DIRS})

if (LOG4CXX_FOUND)
    message(STATUS "LOG4CXX found")
else (LOG4CXX_FOUND)
    message(STATUS "LOG4CXX not found")
    message(STATUS "On Ubuntu, install the package by the following command!")
    message(STATUS "sudo apt-get install liblog4cxx*")
endif (LOG4CXX_FOUND)

set(CMAKE_CXX_FLAGS "-Wall -Werror=return-type -Werror=unused-variable -Werror=unused-parameter -lpapi -fopenmp -no-pie")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DNO_RACE_CHECK -fopenmp -no-pie")
set(CMAKE_CXX_FLAGS_RELEASE "-Wno-ignored-qualifiers -Wno-sign-compare -O3 -march=native -mtune=native -fopenmp -no-pie")

message(STATUS "CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE}")

# Add source code for project
add_subdirectory(src)

get_source_apaj(apaj_SOURCE_FILES)

message("Source files: ${apaj_SOURCE_FILES}")

add_executable(apaj main.cpp ${apaj_SOURCE_FILES})
target_link_libraries(apaj PUBLIC ${LIBRARIES} ${TENSORFLOW_LIB})