
#include "y/associative/hash/map.hpp"

#include "y/io/libc/input.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/shuffle.hpp"
#include "y/apex/natural.hpp"

using namespace Yttrium;


Y_UTEST(associative_hash_map)
{

    HashMap<String,apn> hmap;
    Vector<String>      keys;
    Random::Rand        ran;

    if(argc>1)
    {
        apn             count = 0;
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
            Y_ASSERT(hmap.insert(line,count));
            Y_ASSERT(hmap.search(line));
        }
        std::cerr << "count=" << count << std::endl;
        {
            const HashMap<String,apn> tmp(hmap);
            Y_CHECK(tmp.size()==hmap.size());
        }

        if(keys.size())
        {
            Random::Shuffle::Range(keys(),keys.size(),ran);
            while(keys.size())
            {
                const String &key = keys.tail();
                Y_ASSERT(hmap.search(key));
                Y_ASSERT(hmap.remove(key));
                Y_ASSERT(!hmap.search(key));
                keys.popTail();
            }

        }
    }

}
Y_UDONE()
