include_directories(. ../core ../mkl ../jive ../lua++ ..)

Y_CreateLibrary(y-chemical
chemical
chemical/reactive
chemical/reactive/equilibrium
chemical/reactive/equilibria
chemical/weasel
chemical/plexus
chemical/plexus/cluster
chemical/plexus/cluster/conservation
chemical/plexus/solver
chemical/plexus/solver/norm
)

add_subdirectory(tests)
Y_Regression(chemical)
