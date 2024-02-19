
#include "y/stream/io/pack64.hpp"
#include "y/calculus/byte-count.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/type/utils.hpp"
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



        static const unsigned InfoBits = 4;
        static const unsigned InfoRoom = 8-InfoBits;

        Pack64:: Pack64(const uint64_t word) noexcept:
        size(0),
        data()
        {
            // prepare
            memset(data,0,sizeof(data));

            // counting bits to append
            const unsigned wordBits = BitCount::For(word);
            const unsigned dataBits = InfoBits + wordBits;
            const unsigned outBytes = unsigned( Y_ALIGN_ON(8,dataBits) ) / 8; assert(outBytes>0);
            const unsigned addBytes = outBytes-1; assert(BitCount::For(addBytes) <= InfoBits );

            uint8_t  code = uint8_t(addBytes);
            unsigned bits = InfoBits;
            uint64_t mask = 1;
            for(size_t i=wordBits;i>0;--i,mask <<= 1)
            {
                assert(bits<8);
                code <<= 1; ++bits;
                if(mask&word) code |=1 ;
                if(bits>=8) emit(code,bits);
            }
            assert(bits==dataBits%8);
            if(bits)
                emit(code,bits);
        }

        void Pack64:: emit(uint8_t &code, unsigned &bits) noexcept
        {
            assert(bits>0);
            assert(bits<=8);
            assert(size<=8);
            data[size++] = code << (8-bits);
            code = 0;
            bits = 0;
        }



    }
}

#include "y/stream/output.hpp"

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

#include "y/stream/input.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace IO
    {


        uint64_t Pack64:: Read(InputStream &os, const char *ctx)
        {
            uint64_t qw   = 0;
            uint8_t  code = 0;
            if(!os.fetch(code)) throw Specific::Exception(os.callSign(),"missing first byte for %s", os.From(ctx));

            unsigned addBytes = (code >> InfoRoom);

            code <<= InfoBits;
            unsigned bits   = InfoRoom;
            uint64_t mask   = 1;

            while(true)
            {
                while(bits-- > 0 )
                {
                    if(code & 0x80)
                    {
                        qw |= mask;
                    }
                    code <<= 1;
                    mask <<= 1;
                }
                if(addBytes-- <= 0) break;
                if(!os.fetch(code) )throw Specific::Exception(os.callSign(),"missing extra byte for %s", os.From(ctx));
                bits = 8;
            }


            return qw;
        }
    }

}
