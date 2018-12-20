file(GLOB SRC
	*.cpp
	*.c)

add_subdirectory(HWVideoDecoders)
add_executable (client ${SRC})
target_link_libraries( client HwVideoDecoders -Wall -lavutil -lavformat -lavcodec -lz -lavutil -lm -lswscale -lSDL2 -lSDL2_net -lopus)
