#include "y/apk/element.hpp"

#include "y/memory/allocator/archon.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"
#include "y/calculus/base2.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/check/static.hpp"


#include <cstring>

namespace Yttrium
{


    namespace APK
    {

#define Y_APK_Element_Ctor_Epilog(COUNT)         \
maxBytes( MaxBytesFor(COUNT,Coerce(shift) ) ),   \
entry( Memory::Archon::Acquire(Coerce(shift) ) ),\
maxNum16(maxBytes/sizeof(uint16_t)),             \
maxNum32(maxBytes/sizeof(uint32_t)),             \
maxNum64(maxBytes/sizeof(uint64_t))

        Element:: Element(const size_t usrBytes) :
        Object(),
        bits(0),
        state(AsBytes),
        bytes(0),
        num16(0),
        num32(0),
        num64(0),
        shift(0),
        Y_APK_Element_Ctor_Epilog(usrBytes)
        {
        }


        Element:: Element(const Element &el) :
        Object(),
        bits(el.bits),
        state(el.state),
        bytes(el.bytes),
        num16(el.num16),
        num32(el.num32),
        num64(el.num64),
        shift(0),
        Y_APK_Element_Ctor_Epilog(bytes)
        {
        }




        Element::~Element() noexcept
        {
            Memory::Archon::Release(entry,shift);
        }

        size_t  Element:: MaxBytesFor(const size_t usrBytes, unsigned &shift)
        {
            static const unsigned MinShift64 = iLog2<sizeof(uint64_t)>::Value;
            static const unsigned MinShiftIO = Memory::Archon::MinShift;
            static const unsigned MinShift   = Max(MinShift64,MinShiftIO);
            static const size_t   MinBytes   = size_t(1) << MinShift;

            if(usrBytes>=Base2<size_t>::MaxPowerOfTwo) throw Libc::Exception(ENOMEM, "APK::Element");

            size_t count = MinBytes;
            shift        = MinShift;
            while(count<usrBytes)
            {
                count <<= 1;
                ++shift;
            }
            return count;
        }

        void Element:: ldz() noexcept
        {
            memset(entry,0,maxBytes);
            Coerce(bits)  = 0;
            Coerce(bytes) = 0;
            Coerce(num16) = 0;
            Coerce(num32) = 0;
            Coerce(num64) = 0;
            Coerce(state) = AsBytes;
        }


        void Element:: set(const uint64_t qw) noexcept
        {
            assert(maxBytes>=sizeof(qw));
            if(qw<=0) return;
            *(uint64_t *)entry = qw;
            Coerce(state) = AsNum64;
            Coerce(bits)  = BitCount::For(qw);
            Coerce(bytes) = Y_ALIGN8(bits)  / 8;
            Coerce(num16) = Y_ALIGN16(bits) / 16;
            Coerce(num32) = Y_ALIGN32(bits) / 32;
            Coerce(num64) = Y_ALIGN64(bits) / 64;
        }


        namespace
        {
            enum How
            {
                Collect,
                Nothing,
                Scatter
            };

            static const unsigned BShift[8] =
            {
                0,   8, 16, 24,
                32, 40, 48, 56
            };

            template <typename TARGET, typename SOURCE>
            struct Transmogrify
            {
                static const unsigned TargetSize = sizeof(TARGET);
                static const unsigned SourceSize = sizeof(SOURCE);
                static const How      Direction  = (TargetSize>SourceSize) ? Collect : ( TargetSize<SourceSize ? Scatter : Nothing );
                typedef Int2Type<Direction> Choice;

                static inline void To(TARGET * & target, const SOURCE * &source) noexcept
                {
                    static const Choice choice = {};
                    To(target,source,choice);
                }

            private:
                static inline void To(TARGET * & target, const SOURCE * &source, const Int2Type<Collect> &) noexcept
                {
                    Y_STATIC_CHECK(TargetSize>SourceSize,BadSetup);
                    static const size_t Words = TargetSize/SourceSize;
                    static const size_t Loops = Words-1;
                    TARGET   result = *(source++);
                    for(size_t i=1;i<=Loops;++i)
                    {
                        TARGET src = *(source++);
                        result |= (src <<= BShift[i]);
                    }
                    *(target++) = result;
                }
            };

        }

    }
}

#include "y/text/hexadecimal.hpp"
#include "y/random/park-miller.hpp"
#include "y/hashing/sha1.hpp"

namespace Yttrium
{
    namespace APK
    {

        namespace
        {
            template <const size_t N>
            class TransCheck
            {
            public:

                inline  TransCheck() noexcept : data() { memset(&data,0,sizeof(data)); }
                inline ~TransCheck() noexcept {}

                void init(Random::Bits &ran) noexcept
                {
                    for(unsigned i=0;i<sizeof(data.a8);++i)
                    {
                        data.a8[i] = ran.to<uint8_t>();
                    }
                }


                void check(Random::Bits &ran)
                {
                    init(ran);
                    uint8_t org[sizeof(data.a8)] = { 0 };
                    memcpy(org,data.a8,sizeof(data.a8)); assert( 0 == memcmp(data.a8, org, sizeof(data.a8)));

                    Core::Display(std::cerr << "data=", data.a8, sizeof(data.a8), Hexadecimal::From<uint8_t> ) << std::endl;
                    {
                        const uint8_t *src = data.a8;
                        uint64_t      *tgt = data.a64;
                        for(size_t i=N;i>0;--i)
                        {
                            Transmogrify<uint64_t,uint8_t>::To(tgt,src);
                        }
                        if(data.a8  + 8*N != src) throw Exception("bad source pointer");
                        if(data.a64 + 1*N != tgt) throw Exception("bad target pointer");

                        Core::Display(std::cerr << "a64 =", data.a64, N, Hexadecimal::From<uint64_t> ) << std::endl;
                    }



                }


                union
                {
                    uint8_t   a8[8*N];
                    uint16_t a16[4*N];
                    uint32_t a32[2*N];
                    uint64_t a64[1*N];
                } data;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(TransCheck);
            };


        }


        void Element:: Check()
        {
            std::cerr << "APK::Element::Check" << std::endl;
            TransCheck<1>      chk;
            Random::ParkMiller ran;

            chk.check(ran);

        }


    }

}


