include_directories(. ../core ../mkl ../lingo ../lua++ ..)

Y_CreateLibrary(y-chem
chemical
chemical/weasel
)

add_subdirectory(tests)
Y_Regression(chem)
