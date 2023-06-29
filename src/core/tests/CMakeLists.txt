Y_CreateTest(core
memory 
type
data)

target_link_libraries(test-core y)

enable_testing()

#main
add_test(main:excp test-core excp)

#memory
add_test(memory:chunk test-core memory_chunk)

#type
add_test(type:ints test-core type_ints)

#data
add_test(data:list test-core data_list)
