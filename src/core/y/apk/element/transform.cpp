#include "y/apk/element.hpp"
#include "y/apk/element/transmogrify.hpp"
#include "y/random/park-miller.hpp"
#include "y/exception.hpp"
#include "y/text/hexadecimal.hpp"
#include <iomanip>
#include <cstring>

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
                    static Forward  const forward = (Forward) TransmogrifyProc::Table[t][s]; if(0==forward) throw Exception("Bad Forward");
                    static Reverse  const reverse = (Reverse) TransmogrifyProc::Table[s][t]; if(0==reverse) throw Exception("Bad Reverse");

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


        void Element:: CheckTransmogrify()
        {
            std::cerr << "APK::Element::CheckTranmogrify" << std::endl;
            Random::ParkMiller ran;
            { TransCheck<1> chk; chk.check(ran); }
            { TransCheck<2> chk; chk.check(ran); }
            { TransCheck<3> chk; chk.check(ran); }
            { TransCheck<4> chk; chk.check(ran); }
            { TransCheck<5> chk; chk.check(ran); }
            { TransCheck<6> chk; chk.check(ran); }
            { TransCheck<7> chk; chk.check(ran); }
            { TransCheck<8> chk; chk.check(ran); }

        }

    }

}
