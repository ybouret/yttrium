
#include "y/memory/embedded.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/random/fill.hpp"
#include "y/object.hpp"

using namespace Yttrium;


Y_UTEST(memory_embed)
{
    Random::Rand ran;

    int  *a = 0;
    char *b = 0;
    const size_t numA = 3;
    const size_t numB = 17;

    Memory::Embed emb[] =
    {
        Memory::Embed(a,numA),
        Memory::Embed(b,numB)
    };


    const Memory::Embedded resources(emb,sizeof(emb)/sizeof(emb[0]), Memory::Pooled::Instance() );

    for(size_t i=0;i<numA;++i)
    {
        Random::Fill::Fuzzy(a[i],ran);
    }

    for(size_t i=0;i<numB;++i)
    {
        Random::Fill::Fuzzy(b[i],ran);
    }

    Y_SIZEOF(Memory::Embedded);



}
Y_UDONE()
