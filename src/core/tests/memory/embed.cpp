
#include "y/memory/embed.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/random/fill.hpp"

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

    Memory::Allocator &mgr = Memory::Pooled::Instance();
    size_t bytes = 0;
    void  *entry = Memory::Embed::Acquire(emb,sizeof(emb)/sizeof(emb[0]), mgr,bytes);

    for(size_t i=0;i<numA;++i)
    {
        Random::Fill::Fuzzy(a[i],ran);
    }

    for(size_t i=0;i<numB;++i)
    {
        Random::Fill::Fuzzy(b[i],ran);
    }



    mgr.release(entry,bytes);

}
Y_UDONE()
