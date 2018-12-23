file(GLOB SRC
	*.cpp
	*.c)

add_library(decoder_vdpau ${SRC})
