
include_directories(.)
set(srcs "")

Y_CreateLibrary(y
.
config
check
type
system
calculus
ios
data
data/list
data/pool
concurrent
utest 
memory)

add_subdirectory(tests)
