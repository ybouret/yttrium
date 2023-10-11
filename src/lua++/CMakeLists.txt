include_directories(. .. ../core)

Y_CreateLibrary(y-lua++
lua++
)

add_subdirectory(tests)
Y_Regression(lua++)

