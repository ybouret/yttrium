include_directories(. ../core)


Y_CreateLibrary(y-jive
jive
jive/pattern
jive/pattern/basic
)

add_subdirectory(tests)
Y_Regression(jive)
