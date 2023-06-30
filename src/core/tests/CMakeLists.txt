Y_CreateTest(core
memory 
type
data
calculus)

target_link_libraries(test-core y)

enable_testing()

#main
add_test(main:excp   test-core excp)
add_test(main:atexit test-core atexit)

#memory
add_test(memory:chunk test-core memory_chunk)

#type
add_test(type:ints test-core type_ints)

#data
add_test(data:list test-core data_list)
add_test(data:pool test-core data_pool)

#calculus
add_test(calculus:ilog2 test-core calculus_ilog2)
add_test(calculus:align test-core calculus_align)
add_test(calculus:base2 test-core calculus_base2)
