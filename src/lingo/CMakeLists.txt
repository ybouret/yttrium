include_directories(. ../core)

Y_CreateLibrary(y-lingo
lingo
lingo/pattern
lingo/pattern/basic
lingo/pattern/joker
lingo/pattern/logic
lingo/pattern/regexp
)

add_subdirectory(tests)
Y_Regression(lingo)
