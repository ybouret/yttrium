include_directories(. ../core)

Y_CreateLibrary(y-lingo
lingo
)

add_subdirectory(tests)
Y_Regression(lingo)
