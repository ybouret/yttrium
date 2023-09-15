#include "y/counting/c/perm.h"
#include "y/container/cxx/array.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/pooled.hpp"

using namespace Yttrium;

Y_UTEST(counting_perm)
{
    Y_Perm ctx;

    for(size_t i=1;i<=6;++i)
    {
        CxxArray<size_t,Memory::Pooled> perm(i);
        Y_Perm_Init(&ctx,i);
        Y_Perm_Boot(&ctx,perm.legacy());
        do
        {
            std::cerr << perm << std::endl;
        } while( Y_Perm_Next(&ctx,perm.legacy()) != 0);
    }
}
Y_UDONE()

