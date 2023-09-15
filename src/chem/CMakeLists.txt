include_directories(. ../core ../mkl) 

Y_CreateLibrary(y-chem
chem
chem/algebraic
)

add_subdirectory(tests)
Y_Regression(chem)
