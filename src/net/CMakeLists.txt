include_directories(. ../core)

Y_CreateLibrary(y-net
net
net/type
net/socket
)

add_subdirectory(tests)
Y_Regression(net)
