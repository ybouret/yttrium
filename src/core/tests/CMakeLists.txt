Y_CreateTest(core
memory 
type
data
text
calculus
concurrent
random
hashing
sort
ptr
io
fft
apex
ordered
)

Y_LinkLibraries(test-core)

enable_testing()

#main
add_test(main:excp     test-core excp)
add_test(main:atexit   test-core atexit)
add_test(main:lockable test-core lockable)
add_test(main:wtime    test-core wtime)
add_test(main:singleton test-core singleton)
add_test(main:object    test-core object)

#text
add_test(text:plural test-core text_plural)
add_test(text:human-readable test-core text_human_readable)
add_test(text:ops test-core text_ops)
add_test(text:hexa test-core text_hexa)



#memory
add_test(memory:chunk  test-core memory_chunk)
add_test(memory:pages  test-core memory_pages)
add_test(memory:arena  test-core memory_arena)
add_test(memory:album  test-core memory_album)
add_test(memory:sentry test-core memory_sentry)
add_test(memory:blocks test-core memory_blocks)
add_test(memory:guild  test-core memory_guild)
add_test(memory:notes  test-core memory_notes)
add_test(memory:strap  test-core memory_strap)
add_test(memory:straps test-core memory_straps)
add_test(memory:quanta test-core memory_quanta)
add_test(memory:corpus test-core memory_corpus)
add_test(memory:allocator test-core memory_allocator)
add_test(memory:blanks    test-core memory_blanks)
add_test(memory:studio    test-core memory_studio)
add_test(memory:quarry    test-core memory_quarry)


#type
add_test(type:ints       test-core type_ints)
add_test(type:div        test-core type_div)
add_test(type:list       test-core type_list)
add_test(type:traits     test-core type_traits)
add_test(type:conversion test-core type_conversion)
add_test(type:args       test-core type_args)

#data
add_test(data:list test-core data_list)
add_test(data:pool test-core data_pool)
add_test(data:rework test-core data_rework)

#calculus
add_test(calculus:ilog2 test-core calculus_ilog2)
add_test(calculus:align test-core calculus_align)
add_test(calculus:base2 test-core calculus_base2)

#concurrent
add_test(concurrent:mutex test-core concurrent_mutex)

#random
add_test(random:bits test-core random_bits)
add_test(random:shuffle test-core random_shuffle)


#hashing
add_test(hashing:mix64 test-core hashing_mix64)

#sort
add_test(sort:heap     test-core sort_heap)
add_test(sort:merge    test-core sort_merge)

#ptr
add_test(ptr:auto test-core ptr_auto)
add_test(ptr:arc  test-core ptr_arc)

#io
add_test(io::char test-core io_char)

#fft
add_test(fft:xbr test-core fft_xbr)
add_test(fft:1d test-core fft_1d)

#apex
add_test(apex:archon   test-core apex_archon)
add_test(apex:block    test-core apex_block)
add_test(apex:proto    test-core apex_proto)
add_test(apex:n        test-core apex_n)
add_test(apex:z        test-core apex_z)

#ordered
add_test(ordered:prioQ test-core ordered_prioQ)


