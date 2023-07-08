
include_directories(.)

file(GLOB quark y/concurrent/*.hxx)
#message(STATUS "quark=${quark}")
set(srcs ${quark})

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
random
hashing
utest 
memory)

source_group(concurrent FILES ${quark})

add_subdirectory(tests)
