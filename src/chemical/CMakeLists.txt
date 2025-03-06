include_directories(. ../core ../mkl ../lingo ../lua++ ..)

Y_CreateLibrary(y-chemical
chemical
chemical/type
chemical/reactive
chemical/weasel
chemical/weasel/equilibrium
)

add_subdirectory(tests)
Y_Regression(chemical)

