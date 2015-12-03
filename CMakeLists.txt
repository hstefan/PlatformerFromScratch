cmake_minimum_required(VERSION 3.0)

project(PlatformerFromScratch)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_ARCHIVE_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_LIBRARY_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set(EXTERNAL_DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/external)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

if(MSVC)
  set(GLEW_DIR ${EXTERNAL_DEPENDENCIES_DIR}/glew)
  set(GLEW_INCLUDE_DIR ${GLEW_DIR}/include)
  set(GLEW_LIBRARIES ${GLEW_DIR}/lib/Release/x64/glew32.lib)
  set(GLEW_DYNAMIC_LIBRARIES ${GLEW_DIR}/bin/Release/x64/glew32.dll)
endif()

set(STB_INCLUDE_DIR ${EXTERNAL_DEPENDENCIES_DIR}/stb)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${STB_INCLUDE_DIR})

include_directories(src)

set(SOURCES
  src/main.cpp
  src/gl/Texture.cpp
  src/gl/Shader.cpp
  src/gl/BufferObject.cpp
)

set(HEADERS
  src/gl/Texture.hpp
  src/gl/Shader.hpp
  src/gl/ShaderProgram.hpp
  src/gl/BufferObject.hpp
  src/gl/VertexArray.hpp
  src/NonCopyable.hpp
  src/debug/Debug.hpp
)

add_executable(PlatformerFromScratch
  ${SOURCES}
  ${HEADERS}
)

target_link_libraries(PlatformerFromScratch ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${GLEW_LIBRARIES})

if (MSVC)
  set(CONFIG_FILES ${SDL2_DYNAMIC_LIBRARIES} ${GLEW_DYNAMIC_LIBRARIES})
  add_custom_target(CopyBinaries)
  foreach(ConfigFile ${CONFIG_FILES})
    add_custom_command(TARGET CopyBinaries
                       POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E
                       copy_if_different ${ConfigFile} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$\(ConfigurationName\)/)
  endforeach()

  add_dependencies(PlatformerFromScratch CopyBinaries)
endif()

set(DBG_WORKING_DIR ${PROJECT_SOURCE_DIR})
configure_file(config/debug_dir.vcxproj.user.in
  ${CMAKE_CURRENT_BINARY_DIR}/PlatformerFromScratch.vcxproj.user @ONLY)
