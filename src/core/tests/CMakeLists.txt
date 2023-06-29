Y_CreateTest(core memory)

target_link_libraries(test-core y)

enable_testing()

add_test(main:excp test-core excp)

add_test(memory:chunk test-core memory_chunk)
