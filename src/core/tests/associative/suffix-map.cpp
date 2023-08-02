#include "y/associative/suffix/map.hpp"

#include "y/io/libc/input.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"


using namespace Yttrium;


Y_UTEST(associative_suffix_map)
{
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
        }
        std::cerr << "found #key=" << keys.size() << std::endl;
    }


}
Y_UDONE()
