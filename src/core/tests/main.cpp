#include "y/utest/driver.hpp"

Y_UTEST_DECL(16)
{
    Y_UTEST(excp);
    
    Y_UTEST(memory_chunk);

    Y_UTEST(type_ints);

    Y_UTEST(data_list);
    Y_UTEST(data_pool);

}
Y_UTEST_EXEC()
