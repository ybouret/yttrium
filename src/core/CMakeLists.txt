
include_directories(.)

file(GLOB quark y/concurrent/*.hxx)
message(STATUS "quark=${quark}")
set(srcs ${quark})
source_group(concurrent FILES ${quark})

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
