include_directories(. ../core)

Y_CreateLibrary(y-lingo
lingo
lingo/pattern
)

add_subdirectory(tests)
Y_Regression(lingo)
