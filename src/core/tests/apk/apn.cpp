#include "y/apk/element.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/text/hexadecimal.hpp"

#include <cstring>

using namespace Yttrium;


namespace Yttrium
{
    namespace APK
    {

    }
}

#include "y/concurrent/mutex.hpp"

Y_UTEST(apk_element)
{
    Random::ParkMiller ran;

    APK::Element el(0);
    for(unsigned i=0;i<=64;++i)
    {
        const uint64_t qw = ran.to<uint64_t>(i); Y_ASSERT(i==BitCount::For(qw));
        el.u64(qw);
        Y_ASSERT(i==el.bits);
        std::cerr << Hexadecimal(qw)
        << " bits="  << std::setw(2) << el.bits
        << " bytes=" << std::setw(2) << el.bytes
        << " num16=" << std::setw(2) << el.num16
        << " num32=" << std::setw(2) << el.num32
        << " num64=" << std::setw(2) << el.num64
        << std::endl;

    }

    APK::Element::CheckTransmogrify();


    {
        std::cerr << "<Internal Representations>" << std::endl;
        APK::Element el;
        el.show(std::cerr << "el=") << std::endl;
        el.u64( ran.to<uint64_t>());
        el.show(std::cerr << "el=") << std::endl;
        el.set(APK::AsBytes);
        el.show(std::cerr << "el=") << std::endl;
        el.set(APK::AsNum16);
        el.show(std::cerr << "el=") << std::endl;
        el.set(APK::AsNum32);
        el.show(std::cerr << "el=") << std::endl;
    }


    {
        for(unsigned i=0;i<=64;++i)
        {
            const uint64_t qw = ran.to<uint64_t>(i);
            std::cerr << Hexadecimal(qw) << std::endl;
            {
                uint64_t temp = qw;
                size_t   count = 0;
                void *   entry = APK::Element::Tuned(temp,count, APK::AsBytes);
                Core::Display(std::cerr << "as8  = ", (uint8_t*) entry, count, Hexadecimal::From<uint8_t>) << std::endl;
                const uint8_t * const q = (const uint8_t *)entry;
                const uint64_t w[8] = { q[0], q[1], q[2], q[3], q[4], q[5], q[6], q[7] };
                Y_ASSERT( (w[0] | (w[1]<<8) | (w[2] << 16) | (w[3] << 24) | (w[4] << 32) | (w[5] << 40) | (w[6] << 48) | (w[7] << 56)) == qw );
            }

            {
                uint64_t temp = qw;
                size_t   count = 0;
                void *   entry = APK::Element::Tuned(temp,count, APK::AsNum16);
                Core::Display(std::cerr << "as16 = ", (uint16_t*) entry, count, Hexadecimal::From<uint16_t>) << std::endl;
                const uint16_t * const q = (const uint16_t *)entry;
                const uint64_t w[4] = { q[0], q[1], q[2], q[3] };
                Y_ASSERT( (w[0] | (w[1]<<16) | (w[2] << 32) | (w[3] << 48)) == qw);
            }


            {
                uint64_t temp = qw;
                size_t   count = 0;
                void *   entry = APK::Element::Tuned(temp,count, APK::AsNum32);
                Core::Display(std::cerr << "as32 = ", (uint32_t*) entry, count, Hexadecimal::From<uint32_t>) << std::endl;
                const uint32_t * const q = (const uint32_t *)entry;
                const uint64_t w[4] = { q[0], q[1] };
                Y_ASSERT( (w[0] | (w[1]<<32)) ==  qw);
            }


        }
        return 0;
    }

    {
        std::cerr << "<Random Bits>" << std::endl;
        for(unsigned i=0;i<=80;++i)
        {
            APK::Element el(i,ran); Y_ASSERT(i==el.bits);
            el.show(std::cerr << "el8 =") << std::endl;
            el.set(APK::AsNum16);
            el.show(std::cerr << "el16=") << std::endl;
            el.set(APK::AsNum32);
            el.show(std::cerr << "el32=") << std::endl;
            el.set(APK::AsNum64);
            el.show(std::cerr << "el64=") << std::endl;
        }
    }

    {
        std::cerr << "<Read U64>" << std::endl;
        for(unsigned i=0;i<=80;++i)
        {
            APK::Element el(i,ran);
            Y_ASSERT(APK::AsBytes==el.state);
            Y_ASSERT(i==el.bits);
            const uint64_t q1 = el.u64();
            const uint64_t q2 = el.set(APK::AsNum16).u64();
            const uint64_t q4 = el.set(APK::AsNum32).u64();
            const uint64_t q8 = el.set(APK::AsNum64).u64();

            //std::cerr << "q1 = " << Hexadecimal(q1) << std::endl;
            //std::cerr << "q2 = " << Hexadecimal(q2) << std::endl;
            //std::cerr << "q4 = " << Hexadecimal(q4) << std::endl;
            //std::cerr << "q8 = " << Hexadecimal(q8) << std::endl;
            Y_ASSERT(q1==q2);
            Y_ASSERT(q1==q4);
            Y_ASSERT(q1==q8);
        }
    }

    {
        std::cerr << "<Compare64>" << std::endl;
        for(unsigned i=0;i<=64;++i)
        {
            APK::Element    I(i,ran);     Y_ASSERT(I.bits==i);
            const uint64_t  ii = I.u64(); Y_ASSERT(BitCount::For(ii) == i);
            for(unsigned j=0;j<=64;++j)
            {
                APK::Element J(j,ran);        Y_ASSERT(J.bits==j);
                const uint64_t jj  = J.u64(); Y_ASSERT(BitCount::For(jj) == j);
                const SignType res = Sign::Of(ii,jj);

                for(unsigned k=0;k<4;++k)
                {
                    for(unsigned l=0;l<4;++l)
                    {
                        I.set(APK::Element::State[k]);
                        J.set(APK::Element::State[l]);
                        Y_ASSERT( APK::Element::Compare(I,J) == res);
                        
                        I.set(APK::Element::State[k]);
                        Y_ASSERT( APK::Element::Compare(I,jj) == res);
                        J.set(APK::Element::State[l]);
                        Y_ASSERT( APK::Element::Compare(ii,J) == res);

                    }
                }
            }
        }

        {
            std::cerr << "<CompareFull>" << std::endl;
            for(unsigned i=0;i<=256;++i)
            {
                APK::Element    I(i,ran);     Y_ASSERT(I.bits==i);
                Y_ASSERT( __Zero__ == APK::Element::Compare(I,I) );
                for(unsigned j=0;j<=256;++j)
                {
                    APK::Element J(j,ran);        Y_ASSERT(J.bits==j);
                    if(i<j)
                    {
                        Y_ASSERT( Negative == APK::Element::Compare(I.set(ran),J.set(ran)) );
                        Y_ASSERT( Positive == APK::Element::Compare(J.set(ran),I.set(ran)) );
                    }
                    else
                    {
                        if(i>j)
                        {
                            Y_ASSERT( Positive == APK::Element::Compare(I.set(ran),J.set(ran)) );
                            Y_ASSERT( Negative == APK::Element::Compare(J.set(ran),I.set(ran)) );
                        }
                        else
                        {
                            assert(i==j);
                            Y_ASSERT( APK::Element::Compare(I.set(ran),J.set(ran)) == Sign::Opposite(APK::Element::Compare(J.set(ran),I.set(ran))));
                        }
                    }
                }

            }
        }

    }

    Y_SIZEOF(APK::Element);
}
Y_UDONE()

