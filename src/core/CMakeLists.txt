include_directories(. ..)

message( STATUS "BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../y/zlib)

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
stream
stream/io
stream/libc
stream/libc/buffer
stream/data
stream/gzip
stream/hash
text
text/ascii
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
sort/nw
utest 
memory
memory/allocator
memory/exclusive
memory/buffer
memory/solitary
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
ordered/core
associative
associative/suffix
associative/suffix/core
associative/suffix/map
associative/suffix/set
associative/hash
associative/hash/map
associative/hash/set
graphviz
mkl
mkl/cameo
mkl/cameo/add
mkl/antelope
vfs
information
information/stream
information/bwt
functor
)

source_group(concurrent FILES ${quark})
add_subdirectory(tests)
Y_Regression(core)
