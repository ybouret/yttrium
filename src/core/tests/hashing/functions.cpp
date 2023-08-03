

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

#include "y/hashing/md.hpp"
#include "y/memory/digest.hpp"

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

Y_UTEST(hashing_functions)
{

    Hashing::MD5::    Tests();
    Hashing::SHA1::   Tests();
    Hashing::SHA256:: Tests();
    Hashing::SHA224:: Tests();
    Hashing::PJW::    Tests();
    Hashing::RMD160:: Tests();

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

    if(false)
    {
        for(HFNmap::Iterator it=hmap.begin();it!=hmap.end();++it)
        {
            std::cerr << (**it).callSign() << std::endl;
        }
    }

    if(argc>1)
    {
        Vector<char,Memory::Dyadic> content(100000,AsCapacity);
        std::cerr << "-- loading " << argv[1] << std::endl;
        {
            Libc::InputFile fp(argv[1]);
            fp.loadInto(content);
        }
        std::cerr << "-- loaded #" << content.size() << std::endl;
        if(argc>2)
        {
            const String                key = argv[2];
            Hashing::Function::Pointer *ppH = hmap.search(key);
            if(!ppH) throw Exception("couldn't find '%s'", key() );
            Hashing::Function &H = **ppH;
            const Digest       md = Hashing::MD::Of(H,content(),content.size());
            std::cerr << H.callSign() << " = " << md << std::endl;
        }
        else
        {
            Timing tmx;
            for(HFNmap::Iterator it=hmap.begin();it!=hmap.end();++it)
            {
                Hashing::Function &H = **it;
                H.set();
                tmx.reset();
                {
                    const uint64_t mark = WallTime::Ticks();
                    H.run(content(),content.size());
                    tmx.renew(mark);
                }
                const Digest       md   = Hashing::MD::Of(H);
                const uint64_t     rate = static_cast<uint64_t>(static_cast<long double>(content.size())/tmx.probe());

                std::cerr << std::setw(12) << H.callSign() << " @" << HumanReadable(rate) << "/s " << " = " << md << std::endl;

            }
        }
    }


}
Y_UDONE()

