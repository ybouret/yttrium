


#include "y/associative/suffix/set.hpp"

#include "y/stream/libc/input.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ptr/ark.hpp"
#include "y/random/shuffle.hpp"


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

Y_UTEST(associative_suffix_set)
{

    Random::Rand ran;

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
                if(0==(indx%16))
                    (std::cerr << '.').flush();
            }
        }
        std::cerr << std::endl;
        std::cerr << "found #key=" << keys.size() << "/" << mySet.size() << std::endl;

        {
            std::cerr << "performing copy..." << std::endl;
            SuffixSet<String,Dummy> myCpy(mySet);
            std::cerr << "copy  #key=" << myCpy.size() << std::endl;
            std::cerr << mySet << std::endl;
            std::cerr << myCpy << std::endl;
        }

        std::cerr << "Checking Keys" << std::endl;
        if(keys.size()>0)
        {
            Random::Shuffle::Range(&keys[1], keys.size(), ran);
            for(size_t i=1;i<=keys.size();++i)
            {
                std::cerr << "- " << keys[i] << std::endl;
                const Dummy *pDum = mySet.search(keys[i]);
                Y_ASSERT(0!=pDum);
                std::cerr << "|_" << pDum->name << std::endl;
                Y_ASSERT(mySet.remove(keys[i]));
            }
        }
        std::cerr << "Done" << std::endl;
    }
    

}
Y_UDONE()
