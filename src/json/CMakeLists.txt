include_directories(. ../core ../jive)

Y_CreateLibrary(y-json
json
)

add_subdirectory(tests)
Y_Regression(json)
