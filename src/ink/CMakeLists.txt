include_directories(. ../core ../jive)

Y_CreateLibrary(y-ink
ink
ink/image
ink/image/format
)

add_subdirectory(tests)
Y_Regression(ink)
