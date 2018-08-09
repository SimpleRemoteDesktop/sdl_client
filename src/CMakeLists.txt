file(GLOB SRC
	*.cpp
	*.c)

add_executable (client ${SRC} )
target_link_libraries( client -Wall -lavutil -lavformat -lavcodec -lz -lavutil -lm -lswscale -g -lSDL2 -lSDL2_net -lopus)
