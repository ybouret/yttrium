
#message(STATUS "BINDIR:${CMAKE_CURRENT_BINARY_DIR}")
include_directories(. ../core ../lingo ../mkl )
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
ink/image/format/pnm
ink/ops
ink/ops/blur
ink/ops/filter
ink/ops/gradient
ink/ops/block
)

add_subdirectory(tests)
Y_Regression(ink)
