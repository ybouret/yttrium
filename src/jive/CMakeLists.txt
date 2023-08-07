include_directories(. ../core)


Y_CreateLibrary(y-jive
jive
)

add_subdirectory(tests)
Y_Regression(jive)
