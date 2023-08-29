include_directories(. ../core)

Y_CreateLibrary(y-mkl
mkl
mkl/ode
mkl/opt
mkl/lak
)

add_subdirectory(tests)
Y_Regression(mkl)
