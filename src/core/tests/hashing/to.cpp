
#include "y/hashing/adler32.hpp"
#include "y/hashing/bjh32.hpp"
#include "y/hashing/crc16.hpp"
#include "y/hashing/crc32.hpp"
#include "y/hashing/elf.hpp"
#include "y/hashing/fnv.hpp"
#include "y/hashing/md2.hpp"
#include "y/hashing/md4.hpp"
#include "y/hashing/md5.hpp"
#include "y/hashing/pjw.hpp"
#include "y/hashing/rmd128.hpp"
#include "y/hashing/rmd160.hpp"
#include "y/hashing/sfh.hpp"
#include "y/hashing/sha1.hpp"
#include "y/hashing/sha256.hpp"
#include "y/hashing/sha512.hpp"

#include "y/hashing/to.hpp"

#include "y/associative/suffix/map.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/io/libc/input.hpp"

#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"


using namespace Yttrium;




#define HREG(NAME) \
do {\
const Hashing::Function::Pointer h =  Hashing::NAME::Create(); \
const String                     k = h->callSign();            \
Y_ASSERT(hmap.insert(k,h));                                     \
} while(false)

Y_UTEST(hashing_to)
{



    typedef SuffixMap<String,Hashing::Function::Pointer>  HFNmap;
    HFNmap hmap;

    HREG(Adler32);
    HREG(BJH32);
    HREG(CRC16);
    HREG(CRC32);
    HREG(ELF);
    HREG(FNV);
    HREG(MD2);
    HREG(MD4);
    HREG(MD5);
    HREG(PJW);
    HREG(RMD128);
    HREG(RMD160);
    HREG(SFH);
    HREG(SHA1);
    HREG(SHA256);
    HREG(SHA224);
    HREG(SHA512);
    HREG(SHA384);

    {
        Hashing::ToKey<uint32_t,Hashing::SHA1> H32;
        {
            const int      I = 3;
            const uint32_t k = H32(I);
            std::cerr << "k=" << k << std::endl;
        }

        {
            const String   s = "Toto";
            const uint32_t k = H32(s);
            std::cerr << "k=" << k << std::endl;
        }

        {
            const uint32_t k = H32("Toto");
            std::cerr << "k=" << k << std::endl;
        }


    }

    if(argc>1)
    {
        Vector<String,Memory::Dyadic> strings(100000,AsCapacity);
        {
            Libc::InputFile fp(argv[1]);
            String line;
            while(fp.gets(line))
            {
                bool found = false;
                for(size_t i=strings.size();i>0;--i)
                {
                    if(strings[i]==line)
                    {
                        found = true;
                        break;
                    }
                }
                if(!found) strings << line;
            }
        }
        const size_t keys = strings.size();
        std::cerr << "loaded #key=" << keys << std::endl;
        Vector<uint32_t,Memory::Dyadic> hkey(keys,AsCapacity);
        hkey.adjust(keys,0);

        for(HFNmap::Iterator it=hmap.begin();it!=hmap.end();++it)
        {
            Hashing::Function &H = **it;
            std::cerr << "Using " << H.callSign() << std::endl;
            for(size_t i=1;i<=keys;++i)
            {
                const uint32_t h32 = hkey[i] = Hashing::To<uint32_t>(H,strings[i]);
                for(size_t j=1;j<i;++j)
                {
                    if(h32==hkey[j])
                        std::cerr << "collision: " << strings[i] << " / " << strings[j] << std::endl;
                }
            }
        }


    }


}
Y_UDONE()
