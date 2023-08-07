#include "y/associative/hash/linked.hpp"
#include "y/associative/suffix/map/node.hpp"
#include "y/hashing/to/hash-key.hpp"
#include "y/hashing/fnv.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;

namespace Yttrium
{

}

Y_UTEST(associative_hash_table)
{
    if(false)
    {
        HashTable t(0);
        std::cerr << t.size() << std::endl;
        HashKnot knot(13,0);
        t[knot.hkey].pushHead(&knot);

        t.grow();
        std::cerr << t.size() << std::endl;
        Y_CHECK(t[knot.hkey].size==1);
        t[knot.hkey].popHead();
    }
    
    HashLinked<String,int,SuffixMapNode<String,int>, Hashing::ToHashKey<Hashing::FNV> > hm;

    const String key = "Hello";
    Y_CHECK(hm.insert_(key, 2));
    

}
Y_UDONE()

