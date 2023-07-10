
#include "y/memory/album.hpp"
#include "y/memory/corpus.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/check/sfh32.hpp"

#include "y/utest/run.hpp"

#include <cstring>

using namespace Yttrium;

struct block_t
{
    void  *addr;
    size_t size;
};




static inline
void displayPath(const uint64_t code)
{

    for(size_t i=0;i<8;++i)
    {
        size_t shift = (7-i)*8;
        std::cerr << Hexadecimal::Text[ uint8_t(code>>shift) ];
    }
}

Y_UTEST(memory_corpus)
{
#if 0
    size_t k=0;
    for(size_t i=0;i<16;++i)
    {
        for(size_t j=0;j<16;++j,++k)
        {
            char buffer[16];
            snprintf(buffer, sizeof(buffer), "%02x", unsigned(k));
            std::cerr << '"' << buffer << '"' << ',' << ' ';
        }
        std::cerr << std::endl;
    }

    return 0;
#endif

    Memory::Album  album;
    {
        Memory::Corpus corpus(album);
        for(unsigned shift=0;shift<=Memory::Notes::MaxShift+4;++shift)
        {
            Memory::Dyad &dyad = corpus[shift];
            dyad.release( dyad.acquire() );
        }
        corpus.displayInfo(0);
    }
    album.displayInfo(0);

    for(unsigned shift=0;shift<=Memory::Corpus::MaxShift;++shift)
    {
        const size_t bytes = Base2<size_t>::One << shift;
        std::cerr << "2^" << std::setw(2) << shift << " = " << std::setw(20) << bytes << " | ";
        displayPath(bytes);
        std::cerr << " | ";
        std::cerr << (SFH32::Of(bytes)&255);
        std::cerr << std::endl;

    }


}
Y_UDONE()
