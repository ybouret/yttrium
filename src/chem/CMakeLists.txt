include_directories(. ../core ../mkl) 

Y_CreateLibrary(y-chem
chem
chem/type
chem/species
chem/reactive
chem/algebraic
)

add_subdirectory(tests)
Y_Regression(chem)
