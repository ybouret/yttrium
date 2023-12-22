#include "y/calculus/prime.hpp"
#include "y/utest/run.hpp"
#include "y/data/small/heavy/list/bare.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;

namespace {
    template <typename T>
    static inline void saveTable(OutputStream &fp, const Small::BareHeavyList<T> &p)
    {
        typedef typename Small::BareHeavyList<T>::NodeType Node;
        const size_t n = p.size;
        fp("[%u]={\n",unsigned(n));
        size_t i=1;
        for(const Node *node=p.head;node;node=node->next,++i)
        {
            fp(" %6u",unsigned(**node));
            if(i<n) fp << ',';
            if(0==(i%8)) fp << '\n';
        }
        fp << " }\n";
    }
}
Y_UTEST(calculus_prime)
{

    Small::BareHeavyList<uint8_t> p8;
    uint64_t p      = 0;

    // find primes < 256
    std::cerr << "Looking for <= 8-bits primes" << std::endl;
    while(true)
    {
        p = Prime64::Next(++p);
        if(p<256)
        {
            p8 << uint8_t(p);
            std::cerr << std::setw(4) << p << " #" << std::setw(3) << p8.size << std::endl;
        }
        else
        {
            break;
        }
    }


    // find prime < 65536
    std::cerr << "Looking for <= 16-bits primes" << std::endl;
    Y_ASSERT(p>256);
    Small::BareHeavyList<uint16_t> p16;
    p16 << uint16_t(p);

    std::cerr << p16 << std::endl;
    while(true)
    {
        p = Prime64::Next(++p);
        if(p<65536)
        {
            p16 << uint16_t(p);
            std::cerr << std::setw(6) << p << " #" << std::setw(3) << p16.size << std::endl;
        }
        else
        {
            break;
        }
    }

    std::cerr << "#8  = " << p8.size  << std::endl;
    std::cerr << "#16 = " << p16.size << std::endl;
    {
        Libc::OutputFile fp("p8.hxx");
        saveTable(fp,p8);
    }

    {
        Libc::OutputFile fp("p16.hxx");
        saveTable(fp,p16);
    }

    std::cerr << "First 17 bits: " << p << std::endl;

    for(size_t i=1;i<=Core::Prime::Pi16;++i)
    {
        const unsigned q = Core::Prime::Get(i);
        std::cerr << q << std::endl;
    }

    for(size_t i=0;i<=32;++i)
    {
        std::cerr << "Pi(2^" << std::setw(2) << i << ") = " << std::setw(10) << Core::Prime::Pi2ToThe[i] << std::endl;
    }

    Y_CHECK(Core::Prime::Pi8  == Core::Prime::Pi2ToThe[8]);
    Y_CHECK(Core::Prime::Pi16 == Core::Prime::Pi2ToThe[16]);



}
Y_UDONE()

