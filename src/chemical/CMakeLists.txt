include_directories(. ../core ../mkl ../lingo ../lua++ ..)

Y_CreateLibrary(y-chemical
chemical
chemical/type
chemical/weasel
)

add_subdirectory(tests)
Y_Regression(chemical)

