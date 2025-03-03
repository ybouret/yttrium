include_directories(. ../core ../lingo)

Y_CreateLibrary(y-csv
csv
)

add_subdirectory(tests)
Y_Regression(csv)
