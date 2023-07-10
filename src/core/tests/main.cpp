#include "y/utest/driver.hpp"
#include "y/config/compiler.h"

Y_UTEST_DECL(256)
{


    Y_UTEST(excp);
    Y_UTEST(atexit);
    Y_UTEST(lockable);
    Y_UTEST(wtime);
    Y_UTEST(singleton);
    
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

    Y_UTEST(type_ints);
    Y_UTEST(type_div);

    Y_UTEST(text_plural);
    Y_UTEST(text_human_readable);
    Y_UTEST(text_ops);



    Y_UTEST(data_list);
    Y_UTEST(data_pool);

    Y_UTEST(calculus_ilog2);
    Y_UTEST(calculus_align);
    Y_UTEST(calculus_base2);


    Y_UTEST(concurrent_mutex);

    Y_UTEST(hashing_mix64);

    Y_UTEST(random_bits);
    Y_UTEST(random_shuffle);

    Y_UTEST(sort_heap);
    //std::cerr << "-------- Platform: " << Y_PLATFORM << std::endl;
    //std::cerr << "-------- Compiler: " << Y_COMPILER << std::endl;
}
Y_UTEST_EXEC()
