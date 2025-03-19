
include_directories(../core) 

add_executable(y_trunc y_trunc.cpp)
Y_LinkLibraries(y_trunc)

add_executable(y_rgb y_rgb.cpp)
Y_LinkLibraries(y_rgb)

if(APPLE)
    add_executable(y_mp_missing y_mp_missing.cpp)
    Y_LinkLibraries(y_mp_missing)

    add_executable(y_mp_deps y_mp_deps.cpp)
    Y_LinkLibraries(y_mp_deps)
endif()
