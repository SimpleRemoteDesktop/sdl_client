file(GLOB SRC
	*.cpp
	*.c)

add_subdirectory(vdpau)
add_executable (client ${SRC} rpi/OpenMaxDecoder.cpp rpi/OpenMaxDecoder.h rpi/bcm_host.h)
target_link_libraries( client -Wall -lavutil -lavformat -lavcodec -lz -lavutil -lm -lswscale -lSDL2 -lSDL2_net -lopus)
