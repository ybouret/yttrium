include_directories(. ../core)

Y_CreateLibrary(y-jive
jive
jive/pattern
jive/pattern/basic
jive/pattern/logic
jive/pattern/joker
jive/pattern/alter
jive/pattern/regexp
jive/lexical
jive/lexical/plugin
jive/syntax
)

add_subdirectory(tests)
Y_Regression(jive)
