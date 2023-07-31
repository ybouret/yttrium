
#include "y/io/stream/pack64.hpp"
#include "y/calculus/byte-count.hpp"
#include "y/calculus/bit-count.hpp"
#include <cstring>

#include "y/text/hexadecimal.hpp"
#include <iostream>


namespace Yttrium
{
    namespace IO
    {

        Pack64:: ~Pack64() noexcept
        {
            memset( &Coerce(data[0]),0,sizeof(data));
            Coerce(size) = 0;
        }



        Pack64:: Pack64(const uint64_t qw) noexcept:
        size(0),
        data()
        {
            // prepare
            memset( &Coerce(data[0]),0,sizeof(data));

            // get info
            const unsigned dataBits = BitCount::For(qw);
            const unsigned infoByte = (dataBits > 0) ? dataBits-1 : 0;
            assert(BitCount::For(infoByte) <= InfoBits);
            std::cerr << "dataBits = " << dataBits << " => infoByte = " << Hexadecimal(infoByte) << std::endl;

            // initialize code
            uint8_t  code = uint8_t(infoByte);
            unsigned bits = InfoBits;
            uint64_t mask = 1;
            std::cerr << "code: " << Hexadecimal(code) << " => <";
            for(unsigned i=0;i<dataBits;++i,mask <<= 1)
            {
                assert(bits<8);
                code <<= 1;
                ++bits;
                if(0!=(qw&mask)) code |= 0x1;
                if(bits>=8) emit(code,bits);
            }
            if(bits>0) emit(code,bits);
            std::cerr << " > size=" << size << std::endl;

        }

        void Pack64:: emit(uint8_t &code, unsigned &bits) noexcept
        {
            assert(size<sizeof(data));
            assert(bits>0);
            assert(bits<=8);
            Coerce(data[ Coerce(size)++ ]) = code;// << (8-bits);
            std::cerr << ' ' << Hexadecimal( data[size-1]);
            code = 0;
            bits = 0;
        }


    }
}

#include "y/io/stream/output.hpp"

namespace Yttrium
{
    namespace IO
    {
        size_t Pack64:: Emit(OutputStream &os, const uint64_t qw)
        {
            const Pack64 encoded(qw);
            os.write(encoded.data,encoded.size);
            return encoded.size;
        }
    }
}

#include "y/io/stream/input.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace IO
    {


        uint64_t Pack64:: Read(InputStream &os, const char *ctx)
        {
            uint64_t qw   = 0;
            {
                uint8_t  code = 0;
                os.fetch(code,ctx);
                const unsigned dataBits = 1+unsigned(code >> InfoRoom);
                std::cerr << "Need to read #bits=" << dataBits << std::endl;

                unsigned bits = InfoRoom;
                qw = (code & InfoMask);

                while(bits<dataBits)
                {
                    qw <<= 8;
                    os.fetch(code,ctx);
                    qw |= code;
                    bits += 8;
                }
            }

            return qw;
        }
    }

}
