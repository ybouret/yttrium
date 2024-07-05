include_directories(. ../core)

Y_CreateLibrary(y-chemical
chemical
chemical/reactive
)

add_subdirectory(tests)
Y_Regression(chemical)
