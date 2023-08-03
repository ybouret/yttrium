
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
io/stream
io/libc
io/libc/buffer
text
data
data/list
data/pool
data/small
data/small/proxy
data/small/proto
data/small/light
data/small/light/list
data/small/heavy
data/small/heavy/list
concurrent
random
hashing
hashing/to
sort
utest 
memory
memory/allocator
memory/exclusive
memory/buffer
ptr
container
container/iterator
container/matrix
container/algo
string
fft
apex
apex/m
apex/n
apex/n/proto
apex/z
apex/q
counting
counting/c
sequence
ordered
associative
associative/suffix
associative/suffix/core
associative/suffix/map
associative/suffix/set
associative/hash
mkl
)

source_group(concurrent FILES ${quark})
add_subdirectory(tests)
Y_Regression(core)
