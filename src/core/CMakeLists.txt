
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
text
data
data/list
data/pool
concurrent
utest 
memory)

add_subdirectory(tests)
