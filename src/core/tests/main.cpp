#include "y/utest/driver.hpp"
#include "y/config/compiler.h"

Y_UTEST_DECL(16)
{


    Y_UTEST(excp);
    Y_UTEST(atexit);
    
    Y_UTEST(memory_chunk);

    Y_UTEST(type_ints);

    Y_UTEST(data_list);
    Y_UTEST(data_pool);

    Y_UTEST(calculus_ilog2);
    Y_UTEST(calculus_align);
    Y_UTEST(calculus_base2);

    std::cerr << "-------- Platform: " << Y_PLATFORM << std::endl;
    std::cerr << "-------- Compiler: " << Y_COMPILER << std::endl;
}
Y_UTEST_EXEC()
