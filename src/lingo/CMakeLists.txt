include_directories(. ../core)

Y_CreateLibrary(y-lingo
lingo
lingo/pattern
lingo/pattern/basic
)

add_subdirectory(tests)
Y_Regression(lingo)
