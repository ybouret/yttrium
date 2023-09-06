Y_CreateTest(mkl
ode
opt
algebra
drvs
interp)

Y_LinkLibraries(test-mkl y-mkl)

enable_testing()

add_test(triplet test-mkl triplet)
add_test(interval test-mkl interval)

add_test(ode:rk4 test-mkl ode_rk4)

add_test(opt:parabolic test-mkl opt_parabolic)
add_test(opt:bracket   test-mkl opt_bracket)

add_test(algebra:lu          test-mkl algebra_lu)
add_test(algebra:ortho_space test-mkl algebra_ortho_space)

add_test(drvs:1d test-mkl drvs_1d)

add_test(interp:poly test-mkl interp_poly)

