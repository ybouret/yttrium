include(../../etc/fltk.cmake)
include_directories(. ../core)

if(FLTK_FOUND)
Y_FLTK_SILENCE()
Y_CreateLibrary(y-fltk
fltk
)
add_subdirectory(tests)
endif()

