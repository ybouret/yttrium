include_directories(. .. ../core ../mkl ../jive ../lua++)

Y_CreateLibrary(y-chemical
chemical
chemical/type
chemical/species
chemical/lang
)

add_subdirectory(tests)
Y_Regression(chemical)
