#include "y/associative/hash/set.hpp"
#include "y/stream/libc/input.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/shuffle.hpp"
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

        inline friend std::ostream & operator<<(std::ostream &os, const Dummy &d)
        {
            os << d.indx;
            return os;
        }

        const String name;
        const int    indx;

    private:
        Y_DISABLE_ASSIGN(Dummy);
    };

    typedef ArkPtr<String,Dummy> DumPtr;


}

Y_UTEST(associative_hash_set)
{
    Random::Rand           ran;
    Vector<String>         keys;
    HashSet<String,Dummy>  hset;
    HashSet<String,DumPtr> pset;

    int count = 0;
    if(argc>1)
    {
        Libc::InputFile fp(argv[1]);
        String          line;
        while(fp.gets(line))
        {
            bool found = false;
            for(size_t i=keys.size();i>0;--i)
            {
                if(keys[i] == line)
                {
                    found = true;
                    break;
                }
            }
            if(found) continue;
            keys << line;
            ++count;
            const Dummy dum(line(),count);
            Y_ASSERT(hset.insert(dum));
            const DumPtr ptr = new Dummy(line(),count);
            Y_ASSERT(pset.insert(ptr));
        }

        {
            HashSet<String,Dummy>  hcpy(hset);
            Y_CHECK(hcpy.size()==hset.size());

            HashSet<String,Dummy> hdum;
            hdum.swapWith(hcpy);
            Y_CHECK(hcpy.size()==0);
            Y_CHECK(hdum.size()==hset.size());

        }

        {
            HashSet<String,DumPtr>  pcpy(pset);
            Y_CHECK(pcpy.size()==pset.size());
            std::cerr << hset << std::endl;
            std::cerr << pset << std::endl;
        }

        if(keys.size())
        {

            Random::Shuffle::Range(keys(),keys.size(),ran);
            while(keys.size())
            {
                const String &key = keys.tail();
                Y_ASSERT(hset.search(key));
                Y_ASSERT(pset.search(key));
                Y_ASSERT(hset.remove(key));
                Y_ASSERT(pset.remove(key));
                Y_ASSERT(!hset.search(key));
                Y_ASSERT(!pset.search(key));
                keys.popTail();
            }
        }
    }
}
Y_UDONE()
