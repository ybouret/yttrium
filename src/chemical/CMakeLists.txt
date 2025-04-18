include_directories(. ../core ../mkl ../lingo ../lua++ ..)

Y_CreateLibrary(y-chemical
chemical
chemical/type
chemical/reactive
chemical/reactive/equilibrium
chemical/weasel
chemical/weasel/formula
chemical/weasel/equilibrium
chemical/plexus
chemical/plexus/conservation
chemical/plexus/cluster
chemical/plexus/reactor
chemical/plexus/equalizing
chemical/plexus/equalizing/two-sided
chemical/plexus/initial
chemical/plexus/initial/axiom
)

add_subdirectory(tests)
Y_Regression(chemical)

