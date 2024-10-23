include_directories(. ../core)

Y_CreateLibrary(y-lingo
lingo
lingo/pattern
lingo/pattern/basic
lingo/pattern/joker
lingo/pattern/logic
lingo/pattern/regexp
lingo/pattern/ops
)

add_subdirectory(tests)
Y_Regression(lingo)
