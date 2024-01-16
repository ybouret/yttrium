project(test-fltk)
file(GLOB srcs test-*.cpp)

FLUID_UIC(matte)
FLUID_UIC(canvas)

add_executable(test-fltk 
main.cpp 
${srcs} 
${canvas_SOURCES}
${matte_SOURCES}
)

Y_LinkLibraries( test-fltk y-fltk)
TARGET_LINK_FLTK( test-fltk )

#INSTALL( TARGETS test-fltk RUNTIME DESTINATION var/y )
