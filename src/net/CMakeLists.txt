include_directories(. ../core)

Y_CreateLibrary(y-net
net
)

add_subdirectory(tests)
Y_Regression(net)
