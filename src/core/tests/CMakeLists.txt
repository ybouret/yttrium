Y_CreateTest(core
memory 
type
data
text
codec
calculus
concurrent
random
hashing
sort
ptr
io
fft
string
apex
kemp
tow
ordered
counting
container
sequence
associative
graphviz
mkl
vfs
information
woven
rtld
field
color
)

Y_LinkLibraries(test-core)
target_link_libraries(test-core y-z y-bz2)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

add_library(dso MODULE dso.cpp)

add_executable(test-dso dll.cpp)
Y_LinkLibraries(test-dso)


add_custom_command(TARGET dso POST_BUILD
COMMAND test-dso $<TARGET_FILE:dso>
DEPENDS test-dso 
COMMENT "[yttrium] RTLD"
VERBATIM)


enable_testing()

#main
add_test(main:excp      test-core excp)
add_test(main:atexit    test-core atexit)
add_test(main:lockable  test-core lockable)
add_test(main:wtime     test-core wtime)
add_test(main:singleton test-core singleton)
add_test(main:object    test-core object)
add_test(main:oversized test-core oversized)
add_test(main:quantized test-core quantized)
add_test(main:endian    test-core endian)
add_test(main:rtti      test-core rtti)
add_test(main:eta       test-core eta)
add_test(main:uname     test-core uname)

#text
add_test(text:plural         test-core text_plural)
add_test(text:human-readable test-core text_human_readable)
add_test(text:ops            test-core text_ops)
add_test(text:hexa           test-core text_hexa)
add_test(text:printable      test-core text_printable)
add_test(text:embedding      test-core text_embedding)
add_test(text:justify        test-core text_justify)
add_test(text:convert        test-core text_convert)
add_test(text:convflt        test-core text_convflt)
add_test(text:base64         test-core text_base64)
add_test(text:binary         test-core text_binary)
add_test(text:cxx_id         test-core text_cxx_id)




#memory
add_test(memory:chunk     test-core memory_chunk)
add_test(memory:pages     test-core memory_pages)
add_test(memory:arena     test-core memory_arena)
add_test(memory:album     test-core memory_album)
add_test(memory:sentry    test-core memory_sentry)
add_test(memory:blocks    test-core memory_blocks)
add_test(memory:guild     test-core memory_guild)
add_test(memory:notes     test-core memory_notes)
add_test(memory:strap     test-core memory_strap)
add_test(memory:straps    test-core memory_straps)
add_test(memory:quanta    test-core memory_quanta)
add_test(memory:corpus    test-core memory_corpus)
add_test(memory:allocator test-core memory_allocator)
add_test(memory:blanks    test-core memory_blanks)
add_test(memory:studio    test-core memory_studio)
add_test(memory:quarry    test-core memory_quarry)
add_test(memory:buffer    test-core memory_buffer)
add_test(memory:wad       test-core memory_wad)
add_test(memory:workspace test-core memory_workspace)
add_test(memory:embed     test-core memory_embed)
add_test(memory:embedding test-core memory_embedding)
add_test(memory:digest    test-core memory_digest)
add_test(memory:specimen  test-core memory_specimen)
add_test(memory:objloc    test-core memory_objloc)
add_test(memory:archon    test-core memory_archon)
add_test(memory:zone      test-core memory_zone)


#type
add_test(type:ints       test-core type_ints)
add_test(type:div        test-core type_div)
add_test(type:list       test-core type_list)
add_test(type:traits     test-core type_traits)
add_test(type:conversion test-core type_conversion)
add_test(type:args       test-core type_args)
add_test(type:fourcc     test-core type_fourcc)
add_test(type:functor    test-core type_functor)
add_test(type:temporary  test-core type_temporary)
add_test(type:moniker    test-core type_moniker)
add_test(type:auto_clean test-core type_auto_clean)
add_test(type:binder     test-core type_binder)

#data
add_test(data:list            test-core data_list)
add_test(data:pool            test-core data_pool)
add_test(data:rework          test-core data_rework)
add_test(data:cxx_linked      test-core data_cxx_linked)
add_test(data:ordered_list    test-core data_ordered_list)
add_test(data:bare-light-list test-core data_bare_light_list)
add_test(data:solo-light-list test-core data_solo_light_list)
add_test(data:coop-light-list test-core data_coop_light_list)
add_test(data:bare-heavy-list test-core data_bare_heavy_list)
add_test(data:solo-heavy-list test-core data_solo_heavy_list)
add_test(data:coop-heavy-list test-core data_coop_heavy_list)
add_test(data:ranked          test-core data_ranked)
add_test(data:gc              test-core data_gc)



#calculus
add_test(calculus:ilog2   test-core calculus_ilog2)
add_test(calculus:align   test-core calculus_align)
add_test(calculus:base2   test-core calculus_base2)
add_test(calculus:base10  test-core calculus_base10)
add_test(calculus:gcd     test-core calculus_gcd)
add_test(calculus:bcount  test-core calculus_bcount)
add_test(calculus:isqrt   test-core calculus_isqrt)
add_test(calculus:ipower  test-core calculus_ipower)
add_test(calculus:xlog2   test-core calculus_xlog2)
add_test(calculus:gray    test-core calculus_gray)
add_test(calculus:prime   test-core calculus_prime)
add_test(calculus:prime32 test-core calculus_prime32)




#concurrent
add_test(concurrent:mutex     test-core concurrent_mutex)
add_test(concurrent:thread    test-core concurrent_thread)
add_test(concurrent:condition test-core concurrent_condition)
add_test(concurrent:split1d   test-core concurrent_split1d)
add_test(concurrent:split2d   test-core concurrent_split2d)
add_test(concurrent:topo      test-core concurrent_topo)
add_test(concurrent:context   test-core concurrent_context)
add_test(concurrent:loop      test-core concurrent_loop)
add_test(concurrent:simt1d    test-core concurrent_simt1d)
add_test(concurrent:simt2d    test-core concurrent_simt2d)
add_test(concurrent:simtx     test-core concurrent_simtx)
add_test(concurrent:pipeline  test-core concurrent_pipeline)
add_test(concurrent:frame0d   test-core concurrent_frame0d)
add_test(concurrent:frame1d   test-core concurrent_frame1d)
add_test(concurrent:frame2d   test-core concurrent_frame2d)


#random
add_test(random:bits     test-core random_bits)
add_test(random:shuffle  test-core random_shuffle)
add_test(random:gaussian test-core random_gaussian)
add_test(random:isaac    test-core random_isaac)


#hashing
add_test(hashing:mix64     test-core hashing_mix64)
add_test(hashing:functions test-core hashing_functions)
add_test(hashing:to        test-core hashing_to)
add_test(hashing:mac       test-core hashing_mac)
add_test(hashing:perfect   test-core hashing_perfect)

#sort
add_test(sort:heap     test-core sort_heap)
add_test(sort:merge    test-core sort_merge)
add_test(sort:nw       test-core sort_nw)
add_test(sort:indexing test-core sort_indexing)

#ptr
add_test(ptr:auto         test-core ptr_auto)
add_test(ptr:arc          test-core ptr_arc)
add_test(ptr:light-string test-core ptr_light_string)
add_test(ptr:bare         test-core ptr_bare)

#io
add_test(io:char        test-core io_char)
add_test(io:buffers     test-core io_buffers)
add_test(io:istream     test-core io_istream)
add_test(io:ostream     test-core io_ostream)
add_test(io:copy        test-core io_copy  ${DATA}/small.db)
add_test(io:lines       test-core io_lines ${DATA}/small.db)
add_test(io:strings     test-core io_strings ${DATA}/small.db)
add_test(io:pack64      test-core io_pack64)
add_test(io:ints        test-core io_ints)
add_test(io:ser         test-core io_ser)
add_test(io:gzip-read   test-core io_gzip_read ${DATA}/compressed.db.gz)
add_test(io:gzip-write  test-core io_gzip_write)
add_test(io:bzip2-read  test-core io_bzip2_read ${DATA}/compressed.db2.bz2)
add_test(io:bzip2-write test-core io_bzip2_write)
add_test(io:xmlog       test-core io_xmlog)
add_test(io:bits        test-core io_bits)
add_test(io:disk       test-core io_disk)
add_test(io:pstream     test-core io_pstream)
add_test(io:input-data  test-core io_input_data)

#fft
add_test(fft:xbr  test-core fft_xbr)
add_test(fft:1    test-core fft1)
add_test(fft:2    test-core fft2)
add_test(fft:mul  test-core fft_mul)
add_test(fft:mul2 test-core fft_mul2)

#string(s)
add_test(strings          test-core strings)
add_test(string:tokenizer test-core string_tokenizer)
add_test(string:env       test-core string_env)

#ordered
add_test(ordered:PQ     test-core ordered_PQ)
add_test(ordered:heap   test-core ordered_heap)
add_test(ordered:dire   test-core ordered_dire)
add_test(ordered:locate test-core ordered_locate)
add_test(ordered:vector test-core ordered_vector)

#container
add_test(container:cxx-array  test-core container_cxx_array)
add_test(container:cxx-series test-core container_cxx_series)
add_test(container:iterator   test-core container_iterator)
add_test(container:matrix     test-core container_matrix)
add_test(container:matrix-ops test-core container_matrix_ops)
add_test(container:operating  test-core container_operating)
add_test(container:algo       test-core container_algo)
add_test(container:search     test-core container_search)

#sequence
add_test(sequence:vector test-core sequence_vector)
add_test(sequence:list   test-core sequence_list)
add_test(sequence:snake  test-core sequence_snake)
add_test(sequence:kmp    test-core sequence_kmp)

#counting
add_test(counting:perm        test-core counting_perm)
add_test(counting:comb        test-core counting_comb)
add_test(counting:part        test-core counting_part)
add_test(counting:partition   test-core counting_partition)
add_test(counting:permutation test-core counting_permutation)
add_test(counting:combination test-core counting_combination)
add_test(counting:nested      test-core counting_nested)

#associative
add_test(associative:suffix-tree  test-core associative_suffix_tree)
add_test(associative:suffix-map   test-core associative_suffix_map ${DATA}/small.db)
add_test(associative:suffix-set   test-core associative_suffix_set ${DATA}/small.db)
add_test(associative:key-variety  test-core associative_key_variety)
add_test(associative:hash-table   test-core associative_hash_table)
add_test(associative:hash-map     test-core associative_hash_map ${DATA}/small.db)
add_test(associative:hash-set     test-core associative_hash_set ${DATA}/small.db)
add_test(associative:lek          test-core associative_lek)
add_test(associative:key-set      test-core associative_key_set)
add_test(associative:address-book test-core associative_addrbook)
add_test(associative:pod-key      test-core associative_pod_key)
add_test(associative:flexible-key test-core associative_flexible_key)
add_test(associative:leak         test-core associative_leak)


#mkl
add_test(mkl:numeric test-core mkl_numeric)
add_test(mkl:complex test-core mkl_complex)
add_test(mkl:vtx     test-core mkl_vtx)
add_test(mkl:xreal   test-core mkl_xreal)
add_test(mkl:api     test-core mkl_api)
add_test(mkl:xadd    test-core mkl_xadd)
add_test(mkl:xmul    test-core mkl_xmul)
add_test(mkl:sum3    test-core mkl_sum3)
add_test(mkl:caddy   test-core mkl_caddy)


#graphviz
add_test(graphviz test-core graphviz)

#VFS
add_test(vfs:names test-core vfs_names)
add_test(vfs:entry test-core vfs_entry)
add_test(vfs:scan  test-core vfs_scan)
add_test(vfs:dname test-core vfs_dname)
add_test(vfs:mkdir test-core vfs_mkdir tmp)
add_test(vfs:rm    test-core vfs_rm)
add_test(vfs:cwd   test-core vfs_cwd)
add_test(vfs:subdirs test-core vfs_subdirs)


#Information
add_test(info:mtf   test-core info_mtf)
add_test(info:delta test-core info_delta)
add_test(info:arc4  test-core info_arc4)
add_test(info:bwt   test-core info_bwt)
add_test(info:pack  test-core info_pack)

#WOVEn
add_test(woven:indices  test-core woven_indices)
add_test(woven:subspace test-core woven_subspace)

#rtld
#add_test(NAME rtld:dll COMMAND test-core rtld_dll $<TARGET_FILE:dso> COMMAND_EXPAND_LISTS)

#field
add_test(field:1d test-core field1d)
add_test(field:2d test-core field2d)
add_test(field:3d test-core field3d)
add_test(field:4d test-core field4d)
add_test(field:mk test-core fieldmk)
add_test(field:layouts test-core field_layouts)

#color
add_test(color:rgb test-core color_rgb)
add_test(color:rgba test-core color_rgba)
add_test(color:yuv test-core color_yuv)
add_test(color:conv test-core color_conv)
add_test(color:channels test-core color_channels)

#tow
add_test(tow:api test-core tow_api)

#kemp
add_test(kemp:element  test-core kemp_element)
add_test(kemp:natural  test-core kemp_natural)
add_test(kemp:addsub   test-core kemp_addsub)
add_test(kemp:muldiv   test-core kemp_muldiv)
add_test(kemp:integer  test-core kemp_integer)
add_test(kemp:rational test-core kemp_rational)
add_test(kemp:modular  test-core kemp_modular)
add_test(kemp:input    test-core kemp_input)
add_test(kemp:comden   test-core kemp_comden)
add_test(kemp:univocal test-core kemp_univocal)
add_test(kemp:colinear test-core kemp_colinear)
add_test(kemp:narrow   test-core kemp_narrow)
add_test(kemp:kar      test-core kemp_kar)
add_test(kemp:words    test-core kemp_words)
add_test(kemp:ortho    test-core kemp_ortho)
add_test(kemp:rsa      test-core kemp_rsa)

#codec
add_test(codec:b64 test-core codec_b64)
add_test(codec:u64 test-core codec_u64)

#apex
add_test(apex:types test_core apex_type)
add_test(apex:add   test-core apex_add)
add_test(apex:sub   test-core apex_sub)
add_test(apex:n     test-core apex_n)
add_test(apex:z     test-core apex_z)
add_test(apex:conv  test-core apex_conv)


