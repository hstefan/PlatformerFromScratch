﻿cmake_minimum_required(VERSION 3.0)

project(PlatformerFromScratch)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_ARCHIVE_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_LIBRARY_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set(EXTERNAL_DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/external)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})

include_directories(src)

set(SOURCES
  src/main.cpp
)

set(HEADERS
)

add_executable(PlatformerFromScratch ${SOURCES} ${HEADERS})

target_link_libraries(PlatformerFromScratch ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})

if (MSVC)
  set(CONFIG_FILES ${SDL2_DYNAMIC_LIBRARIES})
  add_custom_target(CopyBinaries)
  foreach(ConfigFile ${CONFIG_FILES})
    add_custom_command(TARGET CopyBinaries
                       POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E
                       copy_if_different ${ConfigFile} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$\(ConfigurationName\)/)
  endforeach()

  add_dependencies(PlatformerFromScratch CopyBinaries)
endif()