#include "y/apk/element.hpp"

#include "y/memory/allocator/archon.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"
#include "y/calculus/base2.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/check/static.hpp"
#include "y/text/hexadecimal.hpp"


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


            template <typename TARGET, typename SOURCE>
            struct Transmogrify
            {
                static const unsigned TargetSize = sizeof(TARGET);
                static const unsigned SourceSize = sizeof(SOURCE);
                static const How      Direction  = (TargetSize>SourceSize) ? Collect : ( TargetSize<SourceSize ? Scatter : Nothing );
                typedef Int2Type<Direction> Choice;
                typedef void (*Proc)(TARGET * &  , const SOURCE * & );

                static inline void To(TARGET * & target, const SOURCE * &source) noexcept
                {
                    static const Choice choice = {};
                    Run(target,source,choice);
                }

            private:
                //! collect source items into larger target item
                static inline void Run(TARGET * & target, const SOURCE * &source, const Int2Type<Collect> &) noexcept
                {
                    Y_STATIC_CHECK(TargetSize>SourceSize,BadSetup);
                    static const size_t   Words = TargetSize/SourceSize; assert(Words>0);
                    static const unsigned Shift = SourceSize * 8;
                    TARGET   result = *(source++);
                    unsigned bshift = 0;
                    for(size_t i=1;i<Words;++i)
                    {
                        TARGET src = *(source++);
                        result |= (src <<= (bshift += Shift));
                    }
                    *(target++) = result;
                }

                //! scatter source item into smaller target items
                static inline void Run(TARGET * & target, const SOURCE * &source, const Int2Type<Scatter> &) noexcept
                {
                    Y_STATIC_CHECK(TargetSize<SourceSize,BadSetup);
                    static const size_t   Words = SourceSize/TargetSize; assert(Words>0);
                    static const unsigned Shift = TargetSize * 8;

                    SOURCE value = *(source++);
                    for(size_t i=0;i<Words;++i,value>>=Shift)
                        *(target++) = static_cast<TARGET>(value);
                }
            };

#define Y_APK_TRANS(TGT,SRC) (void*) Transmogrify<uint##TGT##_t,uint##SRC##_t>::To

            // TranProc<TARGET,SOURCE>
            static  void *TransProc[4][4] =
            {
                {                 0,  Y_APK_TRANS(8,16),  Y_APK_TRANS(8,32), Y_APK_TRANS(8,64)  },
                { Y_APK_TRANS(16,8),                  0, Y_APK_TRANS(16,32), Y_APK_TRANS(16,64) },
                { Y_APK_TRANS(32,8), Y_APK_TRANS(32,16),                  0, Y_APK_TRANS(32,64) },
                { Y_APK_TRANS(64,8), Y_APK_TRANS(64,16), Y_APK_TRANS(64,32),                  0 }
            };


        }

    }
}

#include "y/random/park-miller.hpp"
#include "y/hashing/sha1.hpp"
#include <iomanip>

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

                inline  TransCheck() noexcept :
                a8(),
                a16(),
                a32(),
                a64(),
                arr()
                {
                    arr[0] = &a8;
                    arr[1] = &a16;
                    arr[2] = &a32;
                    arr[3] = &a64;

#if 0
                    num[0] = 8*N;
                    num[1] = 4*N;
                    num[2] = 2*N;
                    num[3] = 1*N;
#endif
                }

                inline ~TransCheck() noexcept {}

                void ldz() noexcept
                {
                    memset(a8,0,sizeof(a8));
                    memset(a16,0,sizeof(a16));
                    memset(a32,0,sizeof(a32));
                    memset(a64,0,sizeof(a64));
                }

                template <typename TARGET, typename SOURCE>
                inline void run(Random::Bits &ran)
                {

                    std::cerr << "running" << std::endl;
                    typedef typename Transmogrify<TARGET,SOURCE>::Proc Forward;
                    typedef typename Transmogrify<SOURCE,TARGET>::Proc Reverse;

                    static const unsigned t    = iLog2Of<TARGET>::Value;
                    static const unsigned s    = iLog2Of<SOURCE>::Value;
                    static const unsigned NT   = (1 << (3-t) )*N;
                    static const unsigned NS   = (1 << (3-s) )*N;
                    static const size_t   SLEN = NS * sizeof(SOURCE);   
                    static const size_t   TLEN = NT * sizeof(TARGET);
                    static Forward  const forward = (Forward) TransProc[t][s]; if(0==forward) throw Exception("Bad Forward");
                    static Reverse  const reverse = (Reverse) TransProc[s][t]; if(0==reverse) throw Exception("Bad Reverse");

                    //----------------------------------------------------------
                    //
                    // clean all
                    //
                    //----------------------------------------------------------
                    ldz();

                    std::cerr << "target: " << std::setw(3) << sizeof(TARGET) * 8 << " bits -> t=" << t << " NT=" << NT << std::endl;
                    std::cerr << "source: " << std::setw(3) << sizeof(SOURCE) * 8 << " bits -> s=" << s << " NS=" << NS <<  std::endl;
                    TARGET * const target = (TARGET *) arr[t];
                    SOURCE * const source = (SOURCE *) arr[s];

                    // initialize source
                    for(size_t i=0;i<NS;++i) source[i] = ran.to<SOURCE>();

                    uint8_t sorg[SLEN] = { 0 };
                    uint8_t torg[TLEN] = { 0 };
                    memcpy(sorg,source,SLEN); // save original source

                    Core::Display(std::cerr << "source=", source, NS, Hexadecimal::From<SOURCE> ) << std::endl;

                    //std::cerr << "Forward" << std::endl;
                    {
                        TARGET *      tgt = target;
                        const SOURCE *src = source;
                        for(size_t i=NT;i>0;--i)
                            forward(tgt,src);
                        if( memcmp(source,sorg,SLEN) != 0) throw Exception("Corrupted Source on Forward");
                        memcpy(torg,target,TLEN);
                    }
                    Core::Display(std::cerr << "target=", target, NT, Hexadecimal::From<TARGET> ) << std::endl;



                    //std::cerr << "Reverse" << std::endl;
                    {
                        const TARGET *tgt = target;
                        SOURCE       *src = source;
                        memset(src,0,SLEN);
                        for(size_t i=NT;i>0;--i)
                            reverse(src,tgt);
                        if( 0 != memcmp(target,torg,TLEN)) throw Exception("Corrupted Target on Reverse");
                    }
                    Core::Display(std::cerr << "source=", source, NS, Hexadecimal::From<SOURCE> ) << std::endl;
                    if( memcmp(source,sorg,SLEN) != 0) throw Exception("Corrupted Reverse");

                }

                void check(Random::Bits &ran)
                {
                    {
                        run<uint64_t,uint8_t>(ran);
                        run<uint64_t,uint16_t>(ran);
                        run<uint64_t,uint32_t>(ran);
                    }

                    {
                        run<uint32_t,uint8_t>(ran);
                        run<uint32_t,uint16_t>(ran);
                    }

                    {
                        run<uint16_t,uint8_t>(ran);
                    }
                }


                uint8_t   a8[8*N];
                uint16_t a16[4*N];
                uint32_t a32[2*N];
                uint64_t a64[1*N];
                void *   arr[4];

            private:
                Y_DISABLE_COPY_AND_ASSIGN(TransCheck);
            };


        }


        void Element:: Check()
        {
            std::cerr << "APK::Element::Check" << std::endl;
            Random::ParkMiller ran;
            { TransCheck<1> chk; chk.check(ran); }
            { TransCheck<2> chk; chk.check(ran); }
            { TransCheck<3> chk; chk.check(ran); }
            { TransCheck<4> chk; chk.check(ran); }

        }


    }

}


