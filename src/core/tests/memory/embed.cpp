
#include "y/container/implanted.hpp"
#include "y/memory/embedded.hpp"
#include "y/memory/embedding/pair.hpp"
#include "y/memory/embedding/solo.hpp"

#include "y/utest/run.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/random/fill.hpp"
#include "y/string.hpp"

using namespace Yttrium;


Y_UTEST(memory_embed)
{
    Random::Rand ran;

    {
        int  *a = 0;
        char *b = 0;
        const size_t numA = 3;
        const size_t numB = 17;

        Memory::Embed emb[] =
        {
            Memory::Embed(a,numA),
            Memory::Embed(b,numB)
        };

        Memory::EmbeddingPair   pair(a,numA,b,numB);
        const Memory::Embedded  resources(pair,Memory::Pooled::Instance() );

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

    {
        String *S = 0;
        Memory::EmbeddingSolo  emb(S,13);
        const Memory::Embedded resources(emb,Memory::Pooled::Instance() );
        Implanted<String>      implanted(emb[0]);
        std::cerr << "string : " << emb[0] << std::endl;
    }
    
}
Y_UDONE()
