
include_directories(.)

file(GLOB quark y/concurrent/*.hxx)
set(srcs ${quark})

Y_CreateLibrary(y
.
locking
config
check
type
system
calculus
io
text
data
data/list
data/pool
concurrent
random
hashing
sort
utest 
memory
memory/allocator
memory/exclusive
memory/buffer
ptr
container
string
fft
apex
apex/m
apex/n
apex/z
apex/q
counting
counting/c
ordered
)

source_group(concurrent FILES ${quark})
add_subdirectory(tests)
Y_Regression(core)
