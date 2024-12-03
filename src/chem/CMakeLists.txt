include_directories(. ../core ../mkl ../lingo ../lua++ ..)

Y_CreateLibrary(y-chem
chemical
chemical/type
chemical/weasel
chemical/weasel/formula
chemical/weasel/equilibrium
chemical/reactive
chemical/reactive/components
)

add_subdirectory(tests)
Y_Regression(chem)
