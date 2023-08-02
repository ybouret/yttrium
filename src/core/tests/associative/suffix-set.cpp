


#include "y/associative/suffix/set.hpp"

#include "y/io/libc/input.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ptr/ark.hpp"


using namespace Yttrium;

namespace Yttrium
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

Y_UTEST(associative_suffix_set)
{

    SuffixSet<String,Dummy>  mySet;
    SuffixSet<String,DumPtr> mySetOfPtr;
    Vector<const String>     keys;

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
                const DumPtr ptr = new Dummy(dum);
                if(!mySetOfPtr.insert(ptr))
                {
                    throw Exception("corrupted insertion!!");
                }

            }
        }
        std::cerr << "found #key=" << keys.size() << "/" << mySet.size() << std::endl;
        SuffixSet<String,Dummy> myCpy(mySet);
        std::cerr << "copy  #key=" << myCpy.size() << std::endl;

    }
    

}
Y_UDONE()
