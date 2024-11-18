include_directories(. ../core ../jive ../lingo)

Y_CreateLibrary(y-json
json
)

add_subdirectory(tests)
Y_Regression(json)
