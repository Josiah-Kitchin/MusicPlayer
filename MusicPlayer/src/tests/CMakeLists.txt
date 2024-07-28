cmake_minimum_required(VERSION 3.10)
project(Testing CXX)

# Find packages using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

# Find other libraries
find_library(SNDFILE_LIBRARIES sndfile)
find_library(MPG123_LIBRARIES mpg123)
find_library(TAG_LIBRARIES tag)
find_library(AO_LIBRARIES ao)


# Check if libraries are found
if(NOT SNDFILE_LIBRARIES)
    message(FATAL_ERROR "libsndfile library not found")
endif()

if(NOT MPG123_LIBRARIES)
    message(FATAL_ERROR "mpg123 library not found")
endif()

if(NOT TAG_LIBRARIES)
    message(FATAL_ERROR "tag library not found")
endif()

if(NOT AO_LIBRARIES)
    message(FATAL_ERROR "ao library not found")
endif()

# Specify source files
file(GLOB SOURCES "../../src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "main.cpp$")


set(GTEST_DIR ./googletest)  # Specify the path to Google Test
add_subdirectory(${GTEST_DIR} gtest)


# Create executable
add_executable(mptest ${SOURCES} test_audio_file_funcs.cpp)

# Include directories
include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(/usr/include)  # Adjust this path based on where headers are located


# Link libraries
target_link_libraries(mptest PRIVATE 
    ${SNDFILE_LIBRARIES}
    ${MPG123_LIBRARIES}
    ${TAG_LIBRARIES}
    ${AO_LIBRARIES}
    ${GTKMM_LIBRARIES}  # Link gtkmm libraries
    gtest
    gtest_main
)

# Compiler options
target_compile_options(mptest PRIVATE -Wall -Wextra -g) 

# Build configuration
set(CMAKE_BUILD_TYPE Debug)

# Install command (optional)
install(TARGETS mptest DESTINATION bin)


enable_testing()

add_test(NAME SongQueue COMMAND mptest)
