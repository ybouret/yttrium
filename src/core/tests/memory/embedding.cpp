#include "y/memory/embedding/solo.hpp"
#include "y/memory/embedding/pair.hpp"
#include "y/memory/embedding/trio.hpp"
#include "y/memory/embedding/quad.hpp"

#include "y/memory/embedded.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/container/implanted.hpp"

#include "y/utest/run.hpp"

#include "y/string.hpp"

using namespace Yttrium;

Y_UTEST(memory_embedding)
{
    Y_SIZEOF(Memory::Embedding::Solo);
    Y_SIZEOF(Memory::Embedding::Pair);
    Y_SIZEOF(Memory::Embedding::Trio);
    Y_SIZEOF(Memory::Embedding::Quad);


    int    *a = 0;
    double *b = 0;
    String *s = 0;
    char   *c = 0;


    Memory::Allocator &mgr = Memory::Pooled::Instance();
    Y_ASSERT(0==a);
    Y_ASSERT(0==b);
    Y_ASSERT(0==s);
    Y_ASSERT(0==c);

    {
        Memory::Embedding::Solo solo(a,5);                Y_CHECK(1==solo.size);
        const Memory::Embedded  emb(solo,mgr);
        std::cerr << emb.bytes << " bytes =" << solo << std::endl;
        Y_ASSERT(solo[0].linkedTo(a));
        Implanted<int> A(solo[0]);
    }

    Y_ASSERT(0==a);
    Y_ASSERT(0==b);
    Y_ASSERT(0==s);
    Y_ASSERT(0==c);
    {
        Memory::Embedding::Pair pair(a,7,b,11);           Y_CHECK(2==pair.size);
        const Memory::Embedded  emb(pair,mgr);
        std::cerr << emb.bytes << " bytes =" << pair << std::endl;
        Y_ASSERT(pair[0].linkedTo(a));
        Y_ASSERT(pair[1].linkedTo(b));
        Implanted<int>    A(pair[0]);
        Implanted<double> B(pair[1]);
    }

    Y_ASSERT(0==a);
    Y_ASSERT(0==b);
    Y_ASSERT(0==s);
    Y_ASSERT(0==c);
    {
        Memory::Embedding::Trio trio(a,3,b,21,s,31);      Y_CHECK(3==trio.size);
        const Memory::Embedded  emb(trio,mgr);
        std::cerr << emb.bytes << " bytes =" << trio << std::endl;
        Y_ASSERT(trio[0].linkedTo(a));
        Y_ASSERT(trio[1].linkedTo(b));
        Y_ASSERT(trio[2].linkedTo(s));
        Implanted<int>    A(trio[0]);
        Implanted<double> B(trio[1]);
        Implanted<String> S(trio[2]);
        for(size_t i=0;i<S.blocks();++i)
        {
            s[i] = "Hello";
        }
    }

    Y_ASSERT(0==a);
    Y_ASSERT(0==b);
    Y_ASSERT(0==s);
    Y_ASSERT(0==c);
    {
        Memory::Embedding::Quad quad(a,2,b,13,s,27,c,93); Y_CHECK(4==quad.size);
        const Memory::Embedded  emb(quad,mgr);
        Y_ASSERT(quad[0].linkedTo(a));
        Y_ASSERT(quad[1].linkedTo(b));
        Y_ASSERT(quad[2].linkedTo(s));
        Y_ASSERT(quad[3].linkedTo(c));

        Implanted<int>    A(quad[0]);
        Implanted<double> B(quad[1]);
        Implanted<String> S(quad[2]);
        Implanted<char>   C(quad[3]);
    }

    Y_ASSERT(0==a);
    Y_ASSERT(0==b);
    Y_ASSERT(0==s);
    Y_ASSERT(0==c);
}
Y_UDONE()

