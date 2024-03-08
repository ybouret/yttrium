include_directories(. ../core)

Y_CreateLibrary(y-ink
ink
)

add_subdirectory(tests)
Y_Regression(ink)
