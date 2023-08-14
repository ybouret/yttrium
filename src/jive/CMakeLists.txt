include_directories(. ../core)

Y_CreateLibrary(y-jive
jive
jive/pattern
jive/pattern/basic
jive/pattern/logic
jive/pattern/joker
)

add_subdirectory(tests)
Y_Regression(jive)
