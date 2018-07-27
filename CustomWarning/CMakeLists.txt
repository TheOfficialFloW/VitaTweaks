cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(custom_warning)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O3 -nostdlib -fshort-wchar")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

add_executable(custom_warning
  main.c
)

target_link_libraries(custom_warning
  SceIofilemgr_stub
  SceLibKernel_stub
  taiHEN_stub
)

vita_create_self(custom_warning.suprx custom_warning CONFIG exports.yml UNSAFE)

vita_create_stubs(stubs custom_warning ${CMAKE_SOURCE_DIR}/exports.yml)
