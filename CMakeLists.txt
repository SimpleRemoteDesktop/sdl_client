cmake_minimum_required(VERSION 3.1)
project(SimpleRemoteDesktop_sdl_client)

if(WIN32)
	message("Windows architecture detected")
endif()


include(ExternalProject)
set(EXTERNALS_DIR ${CMAKE_BINARY_DIR}/external)

ExternalProject_Add(sdl2
	PREFIX ${EXTERNALS_DIR}
	URL https://www.libsdl.org/release/SDL2-2.0.7.tar.gz
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNALS_DIR}/installed -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} 
   )


include_directories(external/installed/include)
link_directories(external/installed/lib)




set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)
set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)

