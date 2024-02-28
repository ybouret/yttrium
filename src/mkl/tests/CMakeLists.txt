Y_CreateTest(mkl
ode
opt
algebra
drvs
interp
io
root
eigen
stats
fit
strain
)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

Y_LinkLibraries(test-mkl y-mkl)

enable_testing()

add_test(triplet   test-mkl triplet)
add_test(interval  test-mkl interval)
add_test(intervals test-mkl intervals)
add_test(utils     test-mkl utils)
add_test(tao0      test-mkl tao0)
add_test(tao1      test-mkl tao1)
add_test(tao2      test-mkl tao2)
add_test(tao3      test-mkl tao3)
add_test(taoTr     test-mkl taoTr)
add_test(trigconv  test-mkl trigconv)


add_test(ode:rk4      test-mkl ode_rk4)
add_test(odeint       test-mkl odeint)
add_test(ode:scheme45 test-mkl ode_scheme45)


add_test(opt:bracket   test-mkl opt_bracket)
add_test(opt:minimize  test-mkl opt_minimize)

add_test(algebra:lu          test-mkl algebra_lu)
add_test(algebra:ortho_space test-mkl algebra_ortho_space)
add_test(algebra:tridiag     test-mkl algebra_tridiag)
add_test(algebra:cyclic      test-mkl algebra_cyclic)
add_test(algebra:svd         test-mkl algebra_svd)

add_test(drvs:1d test-mkl drvs1D)
add_test(drvs:nd test-mkl drvsND)

add_test(interp:poly  test-mkl interp_poly)
add_test(interp:ratio test-mkl interp_ratio)

add_test(io:data-stream test-mkl io_data_stream ${DATA}/sample.txt)

add_test(root:zfind     test-mkl root_zfind)

add_test(eigen:jacobi   test-mkl eigen_jacobi)
add_test(eigen:diag     test-mkl eigen_diag)


add_test(fit:samples    test-mkl fit_samples)
add_test(fit:1d         test-mll fit_1d)

add_test(strain:filter test-mkl strain_filter)
add_test(strain:arc    test-mkl strain_arc2d)
add_test(strain:sim    test-mkl strain_sim)


