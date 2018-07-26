cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(download_enabler)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O3 -nostdlib")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

add_executable(download_enabler
  main.c
)

target_link_libraries(download_enabler
  SceIofilemgr_stub
  SceLibKernel_stub
  taiHEN_stub
)

vita_create_self(download_enabler.suprx download_enabler CONFIG exports.yml UNSAFE)

vita_create_stubs(stubs download_enabler ${CMAKE_SOURCE_DIR}/exports.yml)
