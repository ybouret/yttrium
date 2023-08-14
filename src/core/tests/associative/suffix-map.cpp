
#include "y/associative/suffix/map.hpp"
#include "y/stream/libc/input.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/shuffle.hpp"


using namespace Yttrium;


Y_UTEST(associative_suffix_map)
{
    Random::Rand ran;

    SuffixMap<String,int> myMap;
    Vector<const String>  keys;

    if(argc>1)
    {
        Libc::InputFile fp(argv[1]);
        String line;
        int    indx=1;

        while(fp.gets(line))
        {
            if(myMap.insert(line,indx))
            {
                ++indx;
                keys << line;
            }
            if(0==(indx%16))
                (std::cerr << '.').flush();
        }
        std::cerr << std::endl;
        std::cerr << "found #key=" << keys.size() << "/" << myMap.size() << std::endl;

        if(true)
        {
            std::cerr << "performing copy..." << std::endl;
            SuffixMap<String,int> myCpy(myMap);
            std::cerr << "copy  #key=" << myCpy.size() << std::endl;
            std::cerr << myMap << std::endl;
            std::cerr << myCpy << std::endl;
        }

        std::cerr << "Checking Keys" << std::endl;
        if(keys.size()>0)
        {
            Random::Shuffle::Range(&keys[1], keys.size(), ran);


            for(size_t i=1;i<=keys.size();++i)
            {
                std::cerr << "- " << keys[i] << std::endl;
                const int *p = myMap.search(keys[i]);
                Y_ASSERT(0!=p);
                std::cerr << "|_" << *p << std::endl;
                Y_ASSERT(myMap.remove(keys[i]));
            }
        }
        std::cerr << "Done" << std::endl;



    }
    

}
Y_UDONE()
