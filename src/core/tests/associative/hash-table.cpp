#include "y/associative/hash/linked.hpp"
#include "y/associative/suffix/map/node.hpp"
#include "y/associative/suffix/set/node.hpp"
#include "y/hashing/to/hash-key.hpp"
#include "y/hashing/fnv.hpp"
#include "y/hashing/sha1.hpp"
#include "y/mkl/v2d.hpp"

#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/ptr/ark.hpp"

using namespace Yttrium;

namespace
{



    class Dummy : public Object, public Counted
    {
    public:

        Dummy(const char *id, const int i) : name(id), indx(i)
        {
        }

        ~Dummy() noexcept {}

        Dummy(const Dummy &other) : Object(), Counted(), name(other.name), indx(other.indx) {}

        const String &key() const noexcept { return name; }

        const String name;
        const int    indx;

    private:
        Y_DISABLE_ASSIGN(Dummy);
    };

    typedef ArkPtr<String,Dummy> DumPtr;


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

    {
        HashLinked<String,int,SuffixMapNode<String,int>, Hashing::ToHashKey<Hashing::FNV> > hm;
        const String key = "Hello";
        Y_CHECK(hm.insert_(key, 2));
    }

#if 1
    {
        typedef V2D<int> key2d_t;
        HashLinked<key2d_t,String,SuffixMapNode<key2d_t,String>,Hashing::ToHashKey<Hashing::FNV> > hm;
        key2d_t k(1,2);
        const String s = "toto";
        Y_CHECK(hm.insert_(k,s));
    }
#endif



    HashLinked<String,Dummy,SuffixSetNode<String,Dummy>, Hashing::ToHashKey<Hashing::SHA1> > hs;
    {
        const Dummy dum("World",3);
        Y_CHECK(hs.insert_(dum));
    }


}
Y_UDONE()

