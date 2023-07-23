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




    Y_UTEST(type_ints);
    Y_UTEST(type_div);
    Y_UTEST(type_list);
    Y_UTEST(type_traits);
    Y_UTEST(type_conversion);
    Y_UTEST(type_args);
    Y_UTEST(type_complex);

    Y_UTEST(text_plural);
    Y_UTEST(text_human_readable);
    Y_UTEST(text_ops);
    Y_UTEST(text_hexa);



    Y_UTEST(data_list);
    Y_UTEST(data_pool);
    Y_UTEST(data_rework);

    Y_UTEST(calculus_ilog2);
    Y_UTEST(calculus_align);
    Y_UTEST(calculus_base2);
    Y_UTEST(calculus_gcd);


    Y_UTEST(concurrent_mutex);

    Y_UTEST(hashing_mix64);

    Y_UTEST(random_bits);
    Y_UTEST(random_shuffle);

    Y_UTEST(sort_heap);
    Y_UTEST(sort_merge);

    Y_UTEST(ptr_auto);
    Y_UTEST(ptr_arc);

    Y_UTEST(io_char);

    Y_UTEST(fft_xbr);
    Y_UTEST(fft_1d);

    Y_UTEST(strings);

    Y_UTEST(apex_archon);
    Y_UTEST(apex_block);
    Y_UTEST(apex_proto);
    Y_UTEST(apex_perf);
    Y_UTEST(apex_n);
    Y_UTEST(apex_z);
    Y_UTEST(apex_q);

    Y_UTEST(ordered_PQ);

    Y_UTEST(container_cxx_array);


    Y_UTEST(counting_perm);
    Y_UTEST(counting_comb);
    Y_UTEST(counting_part);

    //std::cerr << "-------- Platform: " << Y_PLATFORM << std::endl;
    //std::cerr << "-------- Compiler: " << Y_COMPILER << std::endl;
}
Y_UTEST_EXEC()
