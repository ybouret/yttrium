include_directories(. ../core ../mkl ../lingo ../lua++ ..)

Y_CreateLibrary(y-chem
chemical
chemical/type
chemical/weasel
chemical/weasel/formula
)

add_subdirectory(tests)
Y_Regression(chem)
