Y_CreateTest(core
memory 
type
data
calculus
concurrent
)

Y_LinkLibraries(test-core)

enable_testing()

#main
add_test(main:excp   test-core excp)
add_test(main:atexit test-core atexit)
add_test(main:lockable test-core lockable)

#memory
add_test(memory:chunk test-core memory_chunk)
add_test(memory:pages test-core memory_pages)
add_test(memory:arena test-core memory_arena)
add_test(memory:book  test-core memory_book)

#type
add_test(type:ints test-core type_ints)

#data
add_test(data:list test-core data_list)
add_test(data:pool test-core data_pool)

#calculus
add_test(calculus:ilog2 test-core calculus_ilog2)
add_test(calculus:align test-core calculus_align)
add_test(calculus:base2 test-core calculus_base2)

#concurrent
add_test(concurrent:mutex test-core concurrent_mutex)
