file(GLOB SRC
	*.cpp
	*.c)

add_executable (client ${SRC})
target_link_libraries( client -Wall -ggdb3 -lavutil -lavformat -lavcodec -lz -lavutil -lm -lswscale -lSDL2 -lSDL2_net -lopus)
