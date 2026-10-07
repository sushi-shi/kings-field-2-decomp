cmake_minimum_required(VERSION 3.25)
project(kings_field_2 VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-fno-rtti)

# The original sources and their static layout checks assume 32-bit pointers
# and longs. Until resource and runtime structures have host representations,
# the port targets ILP32 hosts: i686 Linux and wasm32.
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(FATAL_ERROR "King's Field II currently requires an ILP32 target (i686 or wasm32); use the linux preset in nix develop.")
endif()

option(KF_SANITIZE "Enable address and undefined behavior sanitizers" OFF)
if(KF_SANITIZE AND NOT EMSCRIPTEN)
  # Callbacks are stored through generic function-pointer types, as in the original.
  add_compile_options(-fsanitize=address,undefined -fno-sanitize=function -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-resource-dir
  OUTPUT_VARIABLE KF_COMPILER_RESOURCE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ERROR_IS_FATAL ANY)

# Each original program (PSX, OPEN, GAME, END) is compiled from build.json, the
# same membership the source branch uses. A generated wrapper places each unit
# in the program's namespace and in program-owned data/BSS sections, so the
# runtime can reload a program's globals the way loading its executable did.
file(READ "${CMAKE_SOURCE_DIR}/build.json" KF_BUILD_MANIFEST)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/build.json")
string(JSON KF_IMAGE_COUNT LENGTH "${KF_BUILD_MANIFEST}" images)
math(EXPR KF_IMAGE_LAST "${KF_IMAGE_COUNT} - 1")
set(KF_PROGRAM_OBJECTS)
foreach(image_index RANGE ${KF_IMAGE_LAST})
  string(JSON image_name GET "${KF_BUILD_MANIFEST}" images ${image_index} name)
  string(REGEX REPLACE "\\.EXE$" "" program "${image_name}")
  string(TOLOWER "${program}" program)
  string(JSON unit_count LENGTH "${KF_BUILD_MANIFEST}" images ${image_index} units)
  math(EXPR unit_last "${unit_count} - 1")
  set(program_wrappers)
  foreach(unit_index RANGE ${unit_last})
    string(JSON unit_source GET "${KF_BUILD_MANIFEST}" images ${image_index} units ${unit_index} source)
    string(JSON define_count LENGTH "${KF_BUILD_MANIFEST}" images ${image_index} units ${unit_index} options defines)
    set(unit_defines)
    if(define_count GREATER 0)
      math(EXPR define_last "${define_count} - 1")
      foreach(define_index RANGE ${define_last})
        string(JSON define GET "${KF_BUILD_MANIFEST}" images ${image_index} units ${unit_index} options defines ${define_index})
        list(APPEND unit_defines "${define}")
      endforeach()
    endif()
    string(REPLACE "/" "_" wrapper_name "${unit_source}")
    set(wrapper "${CMAKE_BINARY_DIR}/programs/${program}/${wrapper_name}")
    set(KF_WRAPPER_PROGRAM "${program}")
    set(KF_WRAPPER_SOURCE "${CMAKE_SOURCE_DIR}/${unit_source}")
    configure_file("${CMAKE_SOURCE_DIR}/cmake/program_unit.cpp.in" "${wrapper}" @ONLY)
    set_source_files_properties("${wrapper}" PROPERTIES COMPILE_DEFINITIONS "${unit_defines}")
    list(APPEND program_wrappers "${wrapper}")
  endforeach()
  add_library(kf_program_${program} OBJECT ${program_wrappers})
  target_include_directories(kf_program_${program} PRIVATE include include/psx-sdk)
  target_compile_options(kf_program_${program} PRIVATE
    -ffreestanding -nostdinc -isystem "${KF_COMPILER_RESOURCE_DIR}/include"
    -fno-exceptions -fwrapv -fno-strict-aliasing
    -Wno-deprecated-anon-enum-enum-conversion -Wno-c++23-extensions -Wno-writable-strings)
  list(APPEND KF_PROGRAM_OBJECTS $<TARGET_OBJECTS:kf_program_${program}>)
endforeach()

if(EMSCRIPTEN)
  set(SDL_SHARED OFF CACHE BOOL "" FORCE)
  set(SDL_STATIC ON CACHE BOOL "" FORCE)
  set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
  if(DEFINED ENV{KF_SDL_SOURCE} AND NOT "$ENV{KF_SDL_SOURCE}" STREQUAL "")
    add_subdirectory("$ENV{KF_SDL_SOURCE}" "${CMAKE_BINARY_DIR}/sdl3" EXCLUDE_FROM_ALL)
  else()
    include(FetchContent)
    FetchContent_Declare(SDL3
      URL https://github.com/libsdl-org/SDL/releases/download/release-3.4.8/SDL3-3.4.8.tar.gz
      URL_HASH SHA256=e9fff7467fb60f037e6708da18b25560649e4c63edc2a69bb871b960d9cbfbba
      DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
    FetchContent_MakeAvailable(SDL3)
  endif()
else()
  find_package(SDL3 CONFIG REQUIRED)
  find_library(KF_GLES_LIBRARY GLESv2 REQUIRED)
endif()

# Platform, renderer and the transitional PlayStation library runtime.
add_library(kf_host STATIC
  src/audio/codec.cpp
  src/audio/sound.cpp
  src/platform/controls.cpp
  src/platform/host.cpp
  src/renderer/renderer.cpp
  src/renderer/textures.cpp
  src/runtime/cd.cpp
  src/runtime/disc.cpp
  src/runtime/etc.cpp
  src/runtime/gpu.cpp
  src/runtime/gte.cpp
  src/runtime/kernel.cpp
  src/runtime/press.cpp
  src/runtime/sound.cpp)
target_include_directories(kf_host PUBLIC include)
target_link_libraries(kf_host PUBLIC SDL3::SDL3)
target_compile_options(kf_host PRIVATE -Wall -Wextra -Werror)

add_executable(kings-field-2 src/platform/entry.cpp ${KF_PROGRAM_OBJECTS})
target_link_libraries(kings-field-2 PRIVATE kf_host)
target_compile_options(kings-field-2 PRIVATE -Wall -Wextra)
if(EMSCRIPTEN)
  target_link_options(kings-field-2 PRIVATE
    -sASYNCIFY=1 -sASYNCIFY_STACK_SIZE=262144
    -sUSE_WEBGL2=1 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2
    -sALLOW_MEMORY_GROWTH=1 -sMAXIMUM_MEMORY=536870912
    -sENVIRONMENT=web -sEXIT_RUNTIME=1 -sFORCE_FILESYSTEM=1)
  set_target_properties(kings-field-2 PROPERTIES SUFFIX .html)
else()
  target_link_libraries(kings-field-2 PRIVATE ${KF_GLES_LIBRARY})
endif()
install(TARGETS kings-field-2 RUNTIME DESTINATION bin)
