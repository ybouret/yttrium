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
mkl/strain
mkl/root
mkl/eigen
mkl/eigen/diag
mkl/statistics
mkl/fit
mkl/fit/sample
mkl/fit/variable
mkl/fit/sequential
mkl/fit/least-squares
mkl/fit/executive
)

add_subdirectory(tests)
Y_Regression(mkl)
