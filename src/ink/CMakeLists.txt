include_directories(. ../core ../jive)

Y_CreateLibrary(y-ink
ink
ink/format
)

add_subdirectory(tests)
Y_Regression(ink)
