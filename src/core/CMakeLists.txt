
include_directories(.)
set(srcs "")

Y_CreateLibrary(y
.
config
check
type
system
utest 
memory)

add_subdirectory(tests)
