cmake_minimum_required(VERSION 3.10)
project(ArtGalleryProblem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create executable
add_executable(art_gallery_solver main.cpp)

# Include directories
target_include_directories(art_gallery_solver PRIVATE ${CMAKE_SOURCE_DIR})

# Compiler flags for different platforms
if(MSVC)
    target_compile_options(art_gallery_solver PRIVATE /W4)
else()
    target_compile_options(art_gallery_solver PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Add optimization flags
if(CMAKE_BUILD_TYPE MATCHES Release)
    if(MSVC)
        target_compile_options(art_gallery_solver PRIVATE /O2)
    else()
        target_compile_options(art_gallery_solver PRIVATE -O3)
    endif()
endif()

# Create test executable for random polygon generation
add_executable(test_random_polygons test_random_polygons.cpp)
target_include_directories(test_random_polygons PRIVATE ${CMAKE_SOURCE_DIR})

if(MSVC)
    target_compile_options(test_random_polygons PRIVATE /W4)
else()
    target_compile_options(test_random_polygons PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
    if(MSVC)
        target_compile_options(test_random_polygons PRIVATE /O2)
    else()
        target_compile_options(test_random_polygons PRIVATE -O3)
    endif()
endif()
