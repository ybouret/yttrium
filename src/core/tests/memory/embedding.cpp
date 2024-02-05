#include "y/memory/embedding/solo.hpp"
#include "y/memory/embedding/pair.hpp"
#include "y/memory/embedding/trio.hpp"
#include "y/memory/embedding/quad.hpp"

#include "y/memory/embedded.hpp"
#include "y/memory/allocator/pooled.hpp"

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
    Y_CHECK(0==a);
    Y_CHECK(0==b);
    Y_CHECK(0==s);
    Y_CHECK(0==c);

    {
        Memory::Embedding::Solo solo(a,5);                Y_CHECK(1==solo.size);
        const Memory::Embedded  emb(solo,mgr);
    }

    Y_CHECK(0==a);
    Y_CHECK(0==b);
    Y_CHECK(0==s);
    Y_CHECK(0==c);
    {
        Memory::Embedding::Pair pair(a,7,b,11);           Y_CHECK(2==pair.size);
        const Memory::Embedded  emb(pair,mgr);
    }

    Y_CHECK(0==a);
    Y_CHECK(0==b);
    Y_CHECK(0==s);
    Y_CHECK(0==c);
    {
        Memory::Embedding::Trio trio(a,3,b,21,s,31);      Y_CHECK(3==trio.size);
        const Memory::Embedded  emb(trio,mgr);
    }

    Y_CHECK(0==a);
    Y_CHECK(0==b);
    Y_CHECK(0==s);
    Y_CHECK(0==c);
    {
        Memory::Embedding::Quad quad(a,2,b,13,s,27,c,93); Y_CHECK(4==quad.size);
        const Memory::Embedded  emb(quad,mgr);
    }

    Y_CHECK(0==a);
    Y_CHECK(0==b);
    Y_CHECK(0==s);
    Y_CHECK(0==c);
}
Y_UDONE()

