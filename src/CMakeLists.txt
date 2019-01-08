file(GLOB SRC
        *.cpp
        *.c)


find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBVA libva)
pkg_check_modules(LIBVA_X11 libva-x11)

add_executable(client ${SRC})
target_link_libraries(client -Wall -lavutil -lavformat -lavcodec -lz -lavutil -lm -lswscale -lSDL2 -lSDL2_net -lopus)

set(VA_ACCEL_FOUND FALSE)
if (LIBVA_FOUND AND LIBVA_X11_FOUND)
    set(VA_ACCEL_FOUND TRUE)
endif ()

if (WITH_CRYSTALHD)
    add_subdirectory(crystalhd)
    list(APPEND SRD HAVE_CRYSTAL_HD)
    target_link_libraries(client ${LIBVA_LIBRARIES} ${LIBVA_X11_LIBRARIES} crystalhd)
    message("CRYSTAL_HD support enabled")
endif ()

if (VA_ACCEL_FOUND)
    list(APPEND SRD HAVE_VAAPI)
    list(APPEND SRD_OPTIONS VAAPI)
    add_subdirectory(vaapi)
    target_link_libraries(client ${LIBVA_LIBRARIES} ${LIBVA_X11_LIBRARIES} decoder_vaapi)
    message("VAAPI support found")
endif ()


set_property(TARGET client PROPERTY COMPILE_DEFINITIONS ${SRD})
