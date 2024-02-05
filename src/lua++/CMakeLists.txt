include_directories(. .. ../core)
#add_definitions("-DLUA_C89_NUMBERS")

Y_CreateLibrary(y-lua++
lua++
)

add_subdirectory(tests)
Y_Regression(lua++)

