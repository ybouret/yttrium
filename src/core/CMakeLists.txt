
include_directories(.)
set(srcs "")

Y_CreateLibrary(y
config
check
utest 
memory)

add_subdirectory(tests)
