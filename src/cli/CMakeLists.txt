
include_directories(../core ../jive)

add_executable(y_trunc y_trunc.cpp)
Y_LinkLibraries(y_trunc)

add_executable(y_rgb y_rgb.cpp)
Y_LinkLibraries(y_rgb)

if(APPLE)
    add_executable(y_mp_missing y_mp_missing.cpp)
    Y_LinkLibraries(y_mp_missing y-jive)
endif()
