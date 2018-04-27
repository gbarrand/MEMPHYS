#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(MEMPHYS)

find_package(Geant4 REQUIRED)

option(APP_USE_INLIB_WROOT "Build so that MEMPHYS_sim uses inlib/wroot to write MEMPHYS.root file." ON)
if(APP_USE_INLIB_WROOT)
  add_definitions(-DAPP_USE_INLIB_WROOT)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions

include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project

include_directories(${PROJECT_SOURCE_DIR}/../MEMPHYS ${Geant4_INCLUDE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/../../inexlib/inlib)

if(APP_USE_INLIB_WROOT)
  include_directories(${PROJECT_SOURCE_DIR}/../../inexlib/exlib)
  add_definitions(-DEXLIB_USE_NATIVE_ZLIB)  # It will use G4zlib.
endif()

file(GLOB sources ${PROJECT_SOURCE_DIR}/../source/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/../MEMPHYS/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries

add_executable(MEMPHYS_sim ../applications/sim.cpp ${sources} ${headers})

target_link_libraries(MEMPHYS_sim ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)

add_custom_target(MEMPHYS DEPENDS MEMPHYS_sim)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX

install(TARGETS MEMPHYS_sim DESTINATION bin)

