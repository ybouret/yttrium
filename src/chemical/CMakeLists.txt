include_directories(. ../core ../mkl ../lingo ../lua++ ..)

Y_CreateLibrary(y-chemical
chemical
chemical/chemtran
)

add_subdirectory(tests)
Y_Regression(chemical)

