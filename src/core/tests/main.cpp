#include "y/utest/driver.hpp"
#include "y/config/compiler.h"

Y_UTEST_DECL(256)
{


    Y_UTEST(excp);
    Y_UTEST(atexit);
    Y_UTEST(lockable);
    Y_UTEST(wtime);
    Y_UTEST(singleton);
    Y_UTEST(object);
    Y_UTEST(endian);
    Y_UTEST(rtti);

    Y_UTEST(memory_chunk);
    Y_UTEST(memory_pages);
    Y_UTEST(memory_arena);
    Y_UTEST(memory_album);
    Y_UTEST(memory_sentry);
    Y_UTEST(memory_blocks);
    Y_UTEST(memory_guild);
    Y_UTEST(memory_notes);
    Y_UTEST(memory_strap);
    Y_UTEST(memory_straps);
    Y_UTEST(memory_quanta);
    Y_UTEST(memory_corpus);
    Y_UTEST(memory_allocator);
    Y_UTEST(memory_blanks);
    Y_UTEST(memory_studio);
    Y_UTEST(memory_quarry);
    Y_UTEST(memory_buffer);
    Y_UTEST(memory_wad);
    Y_UTEST(memory_workspace);
    Y_UTEST(memory_embed);
    Y_UTEST(memory_digest);
    Y_UTEST(memory_specimen);
    Y_UTEST(memory_objloc);

    Y_UTEST(type_ints);
    Y_UTEST(type_div);
    Y_UTEST(type_list);
    Y_UTEST(type_traits);
    Y_UTEST(type_conversion);
    Y_UTEST(type_args);
    Y_UTEST(type_fourcc);
    Y_UTEST(type_functor);
    Y_UTEST(type_temporary);
    Y_UTEST(type_moniker);

    Y_UTEST(text_plural);
    Y_UTEST(text_human_readable);
    Y_UTEST(text_ops);
    Y_UTEST(text_hexa);
    Y_UTEST(text_printable);
    Y_UTEST(text_embedding);
    Y_UTEST(text_justify);
    Y_UTEST(text_convert);
    Y_UTEST(text_convflt);
    Y_UTEST(text_base64);




    Y_UTEST(data_list);
    Y_UTEST(data_pool);
    Y_UTEST(data_rework);
    Y_UTEST(data_cxx_linked);
    Y_UTEST(data_bare_light_list);
    Y_UTEST(data_solo_light_list);
    Y_UTEST(data_coop_light_list);
    Y_UTEST(data_bare_heavy_list);
    Y_UTEST(data_solo_heavy_list);
    Y_UTEST(data_coop_heavy_list);


    Y_UTEST(calculus_ilog2);
    Y_UTEST(calculus_align);
    Y_UTEST(calculus_base2); 
    Y_UTEST(calculus_base10);
    Y_UTEST(calculus_gcd);
    Y_UTEST(calculus_bcount);
    Y_UTEST(calculus_isqrt);
    Y_UTEST(calculus_ipower);
    Y_UTEST(calculus_xlog2);


    Y_UTEST(concurrent_mutex);
    Y_UTEST(concurrent_thread);
    Y_UTEST(concurrent_condition);
    Y_UTEST(concurrent_split1d);
    Y_UTEST(concurrent_split2d);
    Y_UTEST(concurrent_topo);
    Y_UTEST(concurrent_queue);
    Y_UTEST(concurrent_context);


    Y_UTEST(hashing_mix64);
    Y_UTEST(hashing_functions);
    Y_UTEST(hashing_to);
    Y_UTEST(hashing_mac);

    Y_UTEST(random_bits);
    Y_UTEST(random_shuffle);
    Y_UTEST(random_gaussian);

    Y_UTEST(sort_heap);
    Y_UTEST(sort_merge);
    Y_UTEST(sort_nw);

    Y_UTEST(ptr_auto);
    Y_UTEST(ptr_arc);
    Y_UTEST(ptr_light_string);

    Y_UTEST(io_char);
    Y_UTEST(io_buffers);
    Y_UTEST(io_istream);
    Y_UTEST(io_ostream);
    Y_UTEST(io_copy);
    Y_UTEST(io_lines);
    Y_UTEST(io_strings);
    Y_UTEST(io_pack64);
    Y_UTEST(io_ints);
    Y_UTEST(io_ser);
    Y_UTEST(io_gzip_read);
    Y_UTEST(io_gzip_write);
    Y_UTEST(io_bzip2_read);
    Y_UTEST(io_bzip2_write);
    Y_UTEST(io_xmlog);

    Y_UTEST(fft1);
    Y_UTEST(fft2);
    Y_UTEST(fft_mul);
    Y_UTEST(fft_mul2);
    Y_UTEST(fft_xbr);

    Y_UTEST(strings);
    Y_UTEST(string_tokenizer);
    Y_UTEST(string_env);

    Y_UTEST(apex_archon);
    Y_UTEST(apex_block);
    Y_UTEST(apex_batch);
    Y_UTEST(apex_proto);
    Y_UTEST(apex_perf);
    Y_UTEST(apex_n);
    Y_UTEST(apex_factorial);
    Y_UTEST(apex_z);
    Y_UTEST(apex_q);
    Y_UTEST(apex_mylar);
    Y_UTEST(apex_ortho);

    Y_UTEST(ordered_PQ);
    Y_UTEST(ordered_heap);
    Y_UTEST(ordered_dire);
    Y_UTEST(ordered_locate);
    Y_UTEST(ordered_vector);

    Y_UTEST(container_cxx_array);
    Y_UTEST(container_cxx_series);
    Y_UTEST(container_iterator);
    Y_UTEST(container_matrix);
    Y_UTEST(container_operating);

    Y_UTEST(sequence_vector);
    Y_UTEST(sequence_list);

    Y_UTEST(counting_perm);
    Y_UTEST(counting_comb);
    Y_UTEST(counting_part);
    Y_UTEST(counting_permutation);
    Y_UTEST(counting_combination);
    Y_UTEST(counting_partition);

    Y_UTEST(associative_suffix_tree);
    Y_UTEST(associative_suffix_map);
    Y_UTEST(associative_suffix_set);
    Y_UTEST(associative_key_variety);
    Y_UTEST(associative_hash_table);
    Y_UTEST(associative_hash_map);
    Y_UTEST(associative_hash_set);
    Y_UTEST(associative_lek);
    Y_UTEST(associative_key_set);
    Y_UTEST(associative_addrbook);

    Y_UTEST(mkl_numeric);
    Y_UTEST(mkl_complex);
    Y_UTEST(mkl_vtx);
    Y_UTEST(mkl_xreal);
    Y_UTEST(mkl_api);
    Y_UTEST(mkl_xadd);
    Y_UTEST(mkl_xmul);
    Y_UTEST(mkl_sum3);


    Y_UTEST(graphviz);

    Y_UTEST(vfs_names);
    Y_UTEST(vfs_entry);
    Y_UTEST(vfs_scan);
    Y_UTEST(vfs_dname);

    Y_UTEST(info_mtf);
    Y_UTEST(info_delta);
    Y_UTEST(info_arc4);
    Y_UTEST(info_bwt);

    Y_UTEST(woven_indices);
    Y_UTEST(woven_subspace);

    Y_UTEST(rtld_dll);

}
Y_UTEST_EXEC()
