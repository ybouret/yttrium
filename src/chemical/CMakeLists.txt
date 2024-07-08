include_directories(. ../core ../jive)

Y_CreateLibrary(y-chemical
chemical
chemical/reactive
chemical/reactive/equilibrium
chemical/reactive/equilibria
chemical/weasel
)

add_subdirectory(tests)
Y_Regression(chemical)
