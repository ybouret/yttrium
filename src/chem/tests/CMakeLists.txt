Y_CreateTest(chem)

Y_LinkLibraries(test-chem y-chem y-lingo  y-mkl y-lua++ y-lua)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

enable_testing()

add_test(weasel test-weasel weasel)
add_test(formula test-weasel formula)
add_test(library test-weasel library)


