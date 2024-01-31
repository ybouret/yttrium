#include "y/memory/embedding/solo.hpp"
#include "y/memory/embedding/pair.hpp"
#include "y/memory/embedding/trio.hpp"
#include "y/utest/run.hpp"

#include "y/string.hpp"

using namespace Yttrium;

Y_UTEST(memory_embedding)
{
    Y_SIZEOF(Memory::EmbeddingSolo);
    Y_SIZEOF(Memory::EmbeddingPair);
    Y_SIZEOF(Memory::EmbeddingTrio);
    Y_SIZEOF(Memory::Embedding::Room<4>);


    int    *a = 0;
    double *b = 0;
    String *s = 0;

    Memory::EmbeddingSolo solo(a,5);            Y_CHECK(1==solo.size);
    Memory::EmbeddingPair pair(a,7,b,11);       Y_CHECK(2==pair.size);
    Memory::EmbeddingTrio trio(a,3,b,21,s,31);  Y_CHECK(3==trio.size);



}
Y_UDONE()

