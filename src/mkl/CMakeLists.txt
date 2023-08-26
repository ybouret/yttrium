include_directories(. ../core)

Y_CreateLibrary(y-mkl
mkl
mkl/ode
)

add_subdirectory(tests)
Y_Regression(mkl)
