
#message(STATUS "BINDIR:${CMAKE_CURRENT_BINARY_DIR}")
include_directories(. ../core ../jive )
include_directories(.. ${CMAKE_CURRENT_BINARY_DIR}/../y/jpeg)

Y_CreateLibrary(y-ink
ink
ink/image
ink/image/format
)

add_subdirectory(tests)
Y_Regression(ink)
