cmake_policy(SET CMP0048 NEW)

project(sia_td1)

cmake_minimum_required(VERSION 3.2.0)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "")
set(GLFW_INSTALL        OFF CACHE BOOL "")
add_subdirectory(external/glfw)

set(BUILD_SHARED_LIBS              OFF CACHE BOOL "")
set(OPTION_BUILD_TOOLS             OFF CACHE BOOL "")
set(OPTION_BUILD_EXAMPLES          OFF CACHE BOOL "")
add_subdirectory(external/glbinding)
add_subdirectory(external/SOIL)

include_directories(external/glfw/include)
include_directories(external/glbinding/source/glbinding/include)
include_directories(external/glbinding/source/glbinding-aux/include)
include_directories(external/eigen3)
include_directories(external/SOIL/src)
include_directories(include)

if (APPLE)
  set(CMAKE_MACOSX_RPATH ON)
  set(GL_SILENCE_DEPRECATION ON)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  CHECK_CXX_COMPILER_FLAG("-std=c++14" HAS_CPP14_FLAG)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP11_FLAG)

  if (HAS_CPP14_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  elseif (HAS_CPP11_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else()
    message(FATAL_ERROR "Unsupported compiler -- requires C++11 support!")
  endif()
  # Enable link time optimization and set the default symbol
  # visibility to hidden (very important to obtain small binaries)
  if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
    # Default symbol visibility
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
  endif()
endif()

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS src/*.h)

add_definitions(-DDATA_DIR="${PROJECT_SOURCE_DIR}/data")

add_executable(sia_td1 ${SOURCES} ${HEADERS})

target_link_libraries(sia_td1 PUBLIC glfw ${GLFW_LIBRARIES} glbinding::glbinding glbinding::glbinding-aux SOIL)

function(IndicateExternalFile _target)
    foreach(_file IN ITEMS ${ARGN})
        if ((IS_ABSOLUTE "${_file}" AND EXISTS "${_file}") OR
            (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_file}"))
            target_sources(${_target} PRIVATE "${_file}")
        endif()
    endforeach()
endfunction()

file(GLOB SHADERS "data/shaders/*.vert" "data/shaders/*.frag")
IndicateExternalFile("sia_td1" ${SHADERS})