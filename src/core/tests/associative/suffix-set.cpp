


#include "y/associative/suffix/set.hpp"

#include "y/io/libc/input.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"


using namespace Yttrium;

namespace Yttrium
{



    class Dummy
    {
    public:
        Dummy(const char *id, const int i) : name(id), indx(i)
        {
        }

        ~Dummy() noexcept {}

        Dummy(const Dummy &other) : name(other.name), indx(other.indx) {}

        const String &key() const noexcept { return name; }

        const String name;
        const int    indx;

    private:
        Y_DISABLE_ASSIGN(Dummy);
    };



}

Y_UTEST(associative_suffix_set)
{

    SuffixSet<String,Dummy> mySet;
    Vector<const String>    keys;

    if(argc>1)
    {
        Libc::InputFile fp(argv[1]);
        String line;
        int    indx=1;

        while(fp.gets(line))
        {
            const Dummy dum(line(),indx);
            if(mySet.insert(dum))
            {
                ++indx;
                keys << line;
            }
        }
        std::cerr << "found #key=" << keys.size() << std::endl;
    }
    

}
Y_UDONE()
