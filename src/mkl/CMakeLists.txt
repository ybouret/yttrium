include_directories(. ../core)

Y_CreateLibrary(y-mkl
mkl
mkl/ode
mkl/opt
mkl/algebra
mkl/derivatives
mkl/interpolation
mkl/io
mkl/filter
mkl/root
mkl/eigen
)

add_subdirectory(tests)
Y_Regression(mkl)
