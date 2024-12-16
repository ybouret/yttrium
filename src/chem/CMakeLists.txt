include_directories(. ../core ../mkl ../lingo ../lua++ ..)

Y_CreateLibrary(y-chem
chemical
chemical/type
chemical/weasel
chemical/weasel/formula
chemical/weasel/equilibrium
chemical/reactive
chemical/reactive/equilibrium
chemical/reactive/equilibrium/components
chemical/reactive/equilibria
chemical/reactive/cluster
chemical/reactive/conservation
chemical/plexus
chemical/plexus/conservation
chemical/plexus/mix
chemical/plexus/solver
)

add_subdirectory(tests)
Y_Regression(chem)
