include_directories(. ../core)

Y_CreateLibrary(y-lingo
lingo
lingo/pattern
lingo/pattern/basic
lingo/pattern/joker
lingo/pattern/logic
lingo/pattern/regexp
lingo/pattern/ops
lingo/lexical
lingo/lexical/add-on
lingo/syntax
lingo/syntax/rule
lingo/syntax/internal
lingo/syntax/internal/wildcard
lingo/syntax/internal/compound
lingo/syntax/grammar
lingo/vfs
)

add_subdirectory(tests)
Y_Regression(lingo)
