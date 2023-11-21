#include "y/text/binary.hpp"
#include <cstring>
#include <iostream>

namespace Yttrium
{

    Binary:: ~Binary() noexcept
    {
        memset(data,0,sizeof(data));
    }

    Binary:: Binary(const Binary &other) noexcept :
    data()
    {
        memcpy(data,other.data,sizeof(data));
    }

    void Binary:: setup(const uint64_t   qw,
                        const size_t     nw,
                        const OutputSize outputSize) noexcept
    {
        static const uint8_t tst[8] =
        {
            128, 64, 32, 16,
            8,  4,  2,  1
        };
        memset(data,0,sizeof(data));

        assert(nw>0);
        assert(nw<=8);
        size_t     k=0;
        for(size_t w=1;w<=nw;++w)
        {
            const uint8_t u = (qw >> ((nw-w)*8));
            //std::cerr << "adding " << unsigned(u) << std::endl;
            for(unsigned i=0;i<8;++i)
            {
                data[k++] = ( 0 != (u&tst[i]) ) ? '1' : '0';
            }
        }
        switch(outputSize)
        {
            case Default:
                return;

            case Compact:
                break;
        }

        const size_t kmax = k-1; // keeping a zero
        size_t       nz   = 0;
        for(size_t i=0;i<kmax;++i)
        {
            if('0'!=data[i])
                break;
            ++nz;
        }
        const size_t nk = k-nz;
        //std::cerr << "removing " << nz << ", keeping " << nk << std::endl;
        char *target = data;
        char *source = data+nz;
        for(size_t i=nk;i>0;--i)
        {
            *(target++) = *(source++);
        }
        for(size_t i=nk;i<k;++i) data[i] = 0;
    }

    std::ostream & operator<<(std::ostream &os, const Binary &h)
    {
        os << h.data;
        return os;
    }

}


#include "y/stream/output.hpp"

namespace Yttrium
{
    OutputStream & operator<<(OutputStream &os, const Binary &h)
    {
        os << h.data;
        return os;
    }
}
