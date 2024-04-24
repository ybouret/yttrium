include_directories(. .. ../core ../mkl ../jive ../lua++)

Y_CreateLibrary(y-chemical
chemical
chemical/type
chemical/species
chemical/reactive
chemical/reactive/equilibrium
chemical/reactive/equilibria
chemical/reactive/conservation
chemical/reactive/cluster
chemical/reactive/plexus
chemical/lang
)

add_subdirectory(tests)
Y_Regression(chemical)
