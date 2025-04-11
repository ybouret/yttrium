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
chemical/plexus/equalizer
chemical/plexus/equalizer/two-sided
)

add_subdirectory(tests)
Y_Regression(chemical)

