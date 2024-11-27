include_directories(. ../core ../mkl ../lingo ../lua++ ..)

Y_CreateLibrary(y-chem
chemical
chemical/type
chemical/weasel
chemical/weasel/formula
chemical/weasel/equilibrium
chemical/reactive
)

add_subdirectory(tests)
Y_Regression(chem)
