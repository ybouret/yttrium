include_directories(. .. ../core ../mkl ../jive ../lua++)

Y_CreateLibrary(y-chemical
chemical
)

add_subdirectory(tests)
Y_Regression(chemical)
