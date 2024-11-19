include_directories(. ../core ../lingo)

Y_CreateLibrary(y-json
json
)

add_subdirectory(tests)
Y_Regression(json)
