cmake_minimum_required(VERSION 3.2)
project(SimpleRemoteDesktop_sdl_client)

if(WIN32)
	message("Windows OS detected")
	set(FFMPEG_WINDOWS_EXTRA --arch=${FFMPEG_ARCH} --target-os=${FFMPEG_TARGET_OS} --cross-prefix=${TOOLCHAIN_PREFIX}- --enable-shared)
	set(OPUS_WINDOWS_EXTRA --host=${TOOLCHAIN_PREFIX})
	set(EXTRA_LIB -lws2_32 -static-libgcc -static-libstdc++)	
endif()

if(UNIX)
	message("Linux OS detected")
	set(EXTRA_LIB -lz)
endif()

if(WIN32)

	include(ExternalProject)
	set(EXTERNALS_DIR ${CMAKE_BINARY_DIR}/external)

	ExternalProject_Add(opus
		URL https://archive.mozilla.org/pub/opus/opus-1.2.1.tar.gz
		CONFIGURE_COMMAND ./configure --disable-asm --prefix=${EXTERNALS_DIR} ${OPUS_WINDOWS_EXTRA}
		BUILD_COMMAND make -j4
		BUILD_IN_SOURCE 1
		)

	ExternalProject_Add(ffmpeg
		URL http://ffmpeg.org/releases/ffmpeg-3.4.tar.bz2
		CONFIGURE_COMMAND ./configure --prefix=${EXTERNALS_DIR} ${FFMPEG_WINDOWS_EXTRA}
		BUILD_COMMAND make -j4
		BUILD_IN_SOURCE 1
		)


	ExternalProject_Add(sdl2
		PREFIX ${EXTERNALS_DIR}
		URL https://www.libsdl.org/release/SDL2-2.0.7.tar.gz
		CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNALS_DIR} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} 
		)

	include_directories(${CMAKE_BINARY_DIR}/external/include)
	link_directories(${CMAKE_BINARY_DIR}/external/lib)
endif()

if(WIN32)
	add_custom_target(distZip 
		COMMAND rm -Rf ${CMAKE_BINARY_DIR}/out
		COMMAND mkdir ${CMAKE_BINARY_DIR}/out
		COMMAND cp ${CMAKE_BINARY_DIR}/external/bin/*.dll ${CMAKE_BINARY_DIR}/out/
		COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/dll/* ${CMAKE_BINARY_DIR}/out/
		COMMAND cp ${CMAKE_BINARY_DIR}/src/client.exe ${CMAKE_BINARY_DIR}/out/
		COMMAND zip -j windows.zip out/* 
		)
endif()
	


set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)
set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)

