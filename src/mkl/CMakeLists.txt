include_directories(. ../core)

Y_CreateLibrary(y-mkl
mkl
mkl/tao
mkl/tao/seq
mkl/tao/par
mkl/ode
mkl/ode/rk45
mkl/opt
mkl/algebra
mkl/derivatives
mkl/interpolation
mkl/io
mkl/filter
mkl/root
mkl/eigen
mkl/eigen/diag
mkl/statistics
)

add_subdirectory(tests)
Y_Regression(mkl)
