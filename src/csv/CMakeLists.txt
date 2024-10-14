include_directories(. ../core ../jive)

Y_CreateLibrary(y-csv
csv
)

add_subdirectory(tests)
Y_Regression(csv)
