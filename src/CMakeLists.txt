file(GLOB SRC
	*.cpp
	*.c)

find_package(PkgConfig REQUIRED)
if(UNIX)
	pkg_check_modules(LIBVA libva)
	pkg_check_modules(LIBVA_X11 libva-x11)
endif()

add_executable (client ${SRC})
target_link_libraries( client -lavutil -lavformat -lavcodec -lavutil -lm -lswscale -lSDL2 -lopus ${EXTRA_LIB} )

set(VA_ACCEL_FOUND FALSE)
set(HAVE_VAAPI FALSE)

if (UNIX AND LIBVA_FOUND AND LIBVA_X11_FOUND AND UNIX)
	set(VA_ACCEL_FOUND TRUE)
endif()

if(VA_ACCEL_FOUND)
	list(APPEND SRD HAVE_VAAPI )
	list(APPEND SRD_OPTIONS VAAPI)
	add_subdirectory(vaapi)
	target_link_libraries(client ${LIBVA_LIBRARIES} ${LIBVA_X11_LIBRARIES} decoder_vaapi)
	message("VAAPI support found")
endif()


set_property(TARGET client PROPERTY COMPILE_DEFINITIONS ${SRD})
