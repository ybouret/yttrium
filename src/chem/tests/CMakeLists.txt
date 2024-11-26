Y_CreateTest(chem)

Y_LinkLibraries(test-chem y-chem y-lingo  y-mkl y-lua++ y-lua)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

enable_testing()

add_test(weasel  test-chem weasel)
add_test(formula test-chem formula)
add_test(library test-chem library)
add_test(eq      test-chem eq)


