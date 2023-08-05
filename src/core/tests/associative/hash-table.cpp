#include "y/associative/hash/table.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(associative_hash_table)
{
    HashTable t(0);
    std::cerr << t.size() << std::endl;
    HashTableKnot knot(13,0);
    t[knot.hkey].pushHead(&knot);

    t.grow();
    std::cerr << t.size() << std::endl;
    Y_CHECK(t[knot.hkey].size==1);
    t[knot.hkey].popHead();

}
Y_UDONE()

