
#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/stream/libc/output.hpp"
#include "y/stream/libc/input.hpp"
#include "y/sequence/vector.hpp"
#include "y/string.hpp"

using namespace Yttrium;
using namespace Apex;



Y_UTEST(apex_n)
{
    Random::ParkMiller ran;

    {
        const Natural lhs = 8;
        const Natural rhs = 10;
        const Natural q = Natural::Div(lhs,rhs);
        
    }

    std::cerr << "Set/Assign 64-bits" << std::endl;
    for(unsigned iter=0;iter<1024;++iter)
    {
        const uint64_t u = ran.to<uint64_t>();
        Natural        U = u;
        Y_ASSERT(U.lsw() == u );
        const uint64_t v = ran.to<uint64_t>();
        U  = v;
        Y_ASSERT(U.lsw() == v);
    }

    std::cerr << "Compare 64-bits" << std::endl;
    for(unsigned ubits=0;ubits<=64;++ubits)
    {

        for(size_t uter=0;uter<8;++uter)
        {
            const uint64_t u = ran.to<uint64_t>(ubits);
            const Natural  U = u;
            Y_ASSERT(U._block().bits == ubits);
            Y_ASSERT(U.lsw() == u );

            Y_ASSERT( U == u );
            Y_ASSERT( u == U );
            Y_ASSERT( ! (U!=u) );
            Y_ASSERT( ! (u!=U) );


            for(unsigned vbits=0;vbits<=64;++vbits)
            {

                for(size_t vter=0;vter<8;++vter)
                {
                    const uint64_t v = ran.to<uint64_t>(vbits);
                    const Natural  V = v;

                    switch( Sign::Of(u,v) )
                    {
                        case Negative:
                            Y_ASSERT(Negative == Natural::Compare(U,V));
                            Y_ASSERT(Negative == Natural::Compare(U,v));
                            Y_ASSERT(Negative == Natural::Compare(u,V));

                            Y_ASSERT(U<V);
                            Y_ASSERT(u<V);
                            Y_ASSERT(U<v);

                            Y_ASSERT(U<=V);
                            Y_ASSERT(u<=V);
                            Y_ASSERT(U<=v);
                            break;

                        case __Zero__:
                            Y_ASSERT(__Zero__ == Natural::Compare(U,V));
                            Y_ASSERT(__Zero__ == Natural::Compare(U,v));
                            Y_ASSERT(__Zero__ == Natural::Compare(u,V));
                            break;

                        case Positive:
                            Y_ASSERT(Positive == Natural::Compare(U,V));
                            Y_ASSERT(Positive == Natural::Compare(U,v));
                            Y_ASSERT(Positive == Natural::Compare(u,V));

                            Y_ASSERT(U>V);
                            Y_ASSERT(u>V);
                            Y_ASSERT(U>v);

                            Y_ASSERT(U>=V);
                            Y_ASSERT(u>=V);
                            Y_ASSERT(U>=v);

                            break;
                    }
                }

            }

        }
    }


    std::cerr << "Compare Full" << std::endl;
    for(unsigned lbits=0;lbits<=1024;lbits += 4)
    {

        const Natural lhs(ran,lbits);

        Y_ASSERT( __Zero__ == Natural::Compare(lhs,lhs) );
        Y_ASSERT( lhs <= lhs );
        Y_ASSERT( lhs == lhs );
        Y_ASSERT( lhs >= lhs );
        Y_ASSERT( ! (lhs != lhs) );

        for(unsigned rbits=0;rbits<=1024; rbits += 4)
        {
            const Natural rhs(ran,lbits);

            switch( Natural::Compare(lhs,rhs) )
            {
                case Negative:
                    Y_ASSERT(lhs<rhs);
                    Y_ASSERT(!(lhs>=rhs));
                    break;

                case __Zero__:
                    break;

                case Positive:
                    Y_ASSERT(lhs>rhs);
                    Y_ASSERT(!(lhs<=rhs));
                    break;
            }

        }

    }

    std::cerr << "Serialize" << std::endl;
    {
        const String    fn = "n.dat";
        Vector<Natural> v;
        size_t          written = 0;
        {
            OutputFile fp(fn);
            for(size_t i=10+ran.leq(100);i>0;--i)
            {
                const Natural n(ran,ran.leq(4000));
                written += n.serialize(fp);
                v << n;
            }
        }
        std::cerr << "Written Bytes = " << written << std::endl;

        {
            InputFile fp(fn);
            size_t    i=0;
            while( fp.ready() )
            {
                ++i;
                Y_ASSERT(i<=v.size());
                const Natural n = Natural::Read(fp);
                Y_ASSERT(n == v[i]);
            }
        }
    }




    std::cerr << "Add 64-bits" << std::endl;
    for(unsigned lbits=0;lbits<=63;++lbits)
    {
        for(unsigned rbits=0;rbits<=63;++rbits)
        {
            const uint64_t l = ran.to<uint64_t>(lbits);
            const uint64_t r = ran.to<uint64_t>(rbits);
            const uint64_t s = l+r;
            const Natural  L = l;
            const Natural  R = r;
            Y_ASSERT( s == L+R);
            Y_ASSERT( s == l+R);
            Y_ASSERT( s == L+r);
            {
                Natural tmp = L;
                Y_ASSERT(s == (tmp+=R));
                tmp = L;
                Y_ASSERT(s == (tmp+=r));
            }



        }
        Natural I = ran.to<uint64_t>(lbits);
        std::cerr << "I   = " << I   << std::endl;
        std::cerr << "++I = " << ++I << std::endl;
        std::cerr << "I++ = " << I++ << std::endl;
        std::cerr << "I   = " << I   << std::endl;

    }

    for(Natural J=0;J<16;++J)
    {
        std::cerr << " " << J;
    }
    std::cerr << std::endl;

    std::cerr << "Sub 64-bits" << std::endl;
    for(unsigned lbits=0;lbits<=64;++lbits)
    {
        for(unsigned rbits=0;rbits<=lbits;++rbits)
        {
            uint64_t l = ran.to<uint64_t>(lbits);
            uint64_t r = ran.to<uint64_t>(rbits);
            if(r>l) Swap(l,r);
            Y_ASSERT(l>=r);
            const Natural L = l;
            const Natural R = r;
            Y_ASSERT(L>=R);
            const uint64_t d = l-r;
            Y_ASSERT(L-R == d);
            Y_ASSERT(l-R == d);
            Y_ASSERT(L-r == d);
        }

        if(lbits>1)
        {
            Natural I = ran.to<uint64_t>(lbits);
            std::cerr << "I   = " << I   << std::endl;
            std::cerr << "--I = " << --I << std::endl;
            std::cerr << "I-- = " << I-- << std::endl;
            std::cerr << "I   = " << I   << std::endl;
        }

    }

    std::cerr << "Add/Sub Full" << std::endl;
    for(size_t iter=0;iter<10000;++iter)
    {
        const Natural a(ran,ran.in<size_t>(0,4096));
        const Natural b(ran,ran.in<size_t>(0,4096));
        const Natural c = a+b;
        { Natural tmp = a; Y_ASSERT( c == (tmp+=b) ); }
        { Natural tmp = b; Y_ASSERT( c == (tmp+=a) ); }
        Y_ASSERT(c-b==a);
        Y_ASSERT(c-a==b);
    }


    std::cerr << "Mul 64bits" << std::endl;
    for(unsigned lbits=0;lbits<=31;++lbits)
    {
        for(unsigned rbits=0;rbits<=32;++rbits)
        {
            const uint64_t l = ran.to<uint64_t>(lbits);
            const uint64_t r = ran.to<uint64_t>(rbits);
            const uint64_t p = l*r;
            const Natural  L = l;
            const Natural  R = r;
            Y_ASSERT(L*R == p);
            Y_ASSERT(L*r == p);
            Y_ASSERT(l*R == p);
            const Natural L2 = L.sqr();
            Y_ASSERT(L*L == L2);
            Y_ASSERT(l*l == L2);
        }

    }

    std::cerr << "Exp2" << std::endl;
    for(size_t ibit=0;ibit<=10;++ibit)
    {
        const Natural n(Exp2,ibit);
        std::cerr << "2^" << ibit << " = " << n << std::endl;
    }

    // return 0;


    std::cerr << "_SHR(any)" << std::endl;
    {
        Natural n(ran,80);
        std::cerr << n << std::endl;
        while(n>0)
        {
            n._shr( ran.in<size_t>(1,4) );
            std::cerr << n << std::endl;
        }
    }

    std::cerr << "_SHR()" << std::endl;
    {
        Natural n(ran,80);
        std::cerr << n << std::endl;
        while(n>0)
        {
            n._shr();
            std::cerr << n << std::endl;
        }
    }

    std::cerr << ">>" << std::endl;
    {
        Natural n(ran,80);
        std::cerr << n << std::endl;
        while(n>0)
        {
            n = (n >> ran.in<size_t>(1,4) );
            std::cerr << n << std::endl;
        }
    }


    std::cerr << ">>=" << std::endl;
    {
        Natural n(ran,80);
        std::cerr << n << std::endl;
        while(n>0)
        {
            n >>= ran.in<size_t>(1,4);
            std::cerr << n << std::endl;
        }
    }

    std::cerr << "shl" << std::endl;
    {
        Natural n(ran,20);
        std::cerr << n << std::endl;
        while(n.bits()<=100)
        {
            n = (n.shl(ran.in<size_t>(1,4) ));
            std::cerr << n << std::endl;
        }
    }

    std::cerr << "Division/Modulus" << std::endl;
    {
        for(size_t qbits=0;qbits<=256; qbits += ran.in<size_t>(4,10) )
        {
            for(size_t dbits=1;dbits<=256; dbits += ran.in<size_t>(4,10))
            {
                const Natural quot(ran,qbits);
                const Natural den(ran,dbits);
                Natural rem = 0;
                for(unsigned cycle=0;cycle<2;++cycle)
                {
                    const Natural num = quot * den + rem;
                    const Natural q   = Natural::Div(num,den);
                    const Natural m   = Natural::Mod(num,den);
                    Y_ASSERT(q==quot);
                    Y_ASSERT(m==rem);
                    Y_ASSERT(num/den==quot);
                    Y_ASSERT(num%den==rem);
                    { Natural n = num; Y_ASSERT( quot == (n/=den) ); }
                    { Natural n = num; Y_ASSERT( rem  == (n%=den) ); }
                    Natural Q = 0, R=0;
                    Natural::Div(Q,R,num,den);
                    Y_ASSERT(quot==Q);
                    Y_ASSERT(rem ==R);

                    rem = Natural(ran,ran.lt(dbits));
                }
            }
        }
    }

    std::cerr << "Sqrt" << std::endl;
    for(size_t bits=0;bits<=100;bits += ran.in<size_t>(1,8) )
    {
        const Natural n(ran,bits);
        const Natural s = n.sqrt();
        std::cerr <<  n << " => " << s << std::endl;
    }

    std::cerr << "ToHex" << std::endl;
#define _SHOW(value) n = value; std::cerr << Hexadecimal(value) << " => " << n.toHex() << std::endl;
    {
        Natural n = 0;
        _SHOW(0);
        _SHOW(0xa);
        _SHOW(0xab);
        _SHOW(0xabc);
        _SHOW(0xabcd);
        _SHOW(0xabcde);
        _SHOW(0xabcdef);
        _SHOW(0xabcdef1);
        _SHOW(0xabcdef12);
    }

#undef _SHOW
#define _SHOW(value) n = value; std::cerr << natural_t(value) << " => " << n.toDec() << std::endl;
    std::cerr << "ToDec" << std::endl;
    {
        Natural n = 0;
        _SHOW(0);
        _SHOW(7);
        _SHOW(8);
        _SHOW(10);
        _SHOW(13);
        _SHOW(234);
    }

    std::cerr << "GCD" << std::endl;
    {
        const Natural a = 24;
        const Natural b = 12;
        const Natural g = Natural::GCD(a,b);
        std::cerr << "gcd=" << g << std::endl;
    }



}
Y_UDONE()
