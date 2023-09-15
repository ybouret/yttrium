Y_CreateTest(mkl
ode
opt
algebra
drvs
interp
io
woven
filter)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

Y_LinkLibraries(test-mkl y-mkl)

enable_testing()

add_test(triplet test-mkl triplet)
add_test(interval test-mkl interval)

add_test(ode:rk4 test-mkl ode_rk4)
add_test(odeint  test-mkl odeint)

add_test(opt:parabolic test-mkl opt_parabolic)
add_test(opt:bracket   test-mkl opt_bracket)

add_test(algebra:lu          test-mkl algebra_lu)
add_test(algebra:ortho_space test-mkl algebra_ortho_space)
add_test(algebra:tridiag     test-mkl algebra_tridiag)
add_test(algebra:cyclic      test-mkl algebra_cyclic)

add_test(drvs:1d test-mkl drvs1D)
add_test(drvs:nd test-mkl drvsND)

add_test(interp:poly  test-mkl interp_poly)
add_test(interp:ratio test-mkl interp_ratio)

add_test(io:data-stream test-mkl io_data_stream ${DATA}/sample.txt)

add_test(woven:chemsys  test-mkl woven_chemsys)

add_test(filter:smooth  test-mkl filter_smooth)
