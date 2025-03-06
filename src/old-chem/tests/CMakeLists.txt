Y_CreateTest(chem)

Y_LinkLibraries(test-chem y-chem y-lingo  y-mkl y-lua++ y-lua)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

enable_testing()

add_test(weasel  test-chem weasel)
add_test(formula test-chem formula)
add_test(library test-chem library)
add_test(eq      test-chem eq)
add_test(db      test-chem db)
add_test(aftermath test-chem aftermath)
add_test(warden    test-chem warden)
add_test(eqz       test-chem eqz)
add_test(plexus  test-chem plexus)
add_test(metrics test-chem metrics)

