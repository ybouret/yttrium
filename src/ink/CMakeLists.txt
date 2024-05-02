
#message(STATUS "BINDIR:${CMAKE_CURRENT_BINARY_DIR}")
include_directories(. ../core ../jive )
include_directories(..)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../y/jpeg)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../y/png)
include_directories(../y/tiff/libtiff)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../y/tiff/libtiff)

Y_CreateLibrary(y-ink
ink
ink/parallel
ink/image
ink/image/format
ink/crux
ink/ops
)

add_subdirectory(tests)
Y_Regression(ink)
