include_directories(. .. ../core ../mkl ../jive ../lua++)

Y_CreateLibrary(y-chem
chem
chem/type
chem/species
chem/reactive
chem/reactive/lua
chem/algebraic
chem/lang
chem/plexus
chem/plexus/cluster
chem/plexus/conservation
chem/plexus/janitor
)

add_subdirectory(tests)
Y_Regression(chem)
