

#include "y/apex/integer.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/stream/libc/output.hpp"
#include "y/stream/libc/input.hpp"
#include "y/sequence/vector.hpp"
#include "y/string.hpp"

using namespace Yttrium;
using namespace Apex;


static inline integer_t z64(Random::Bits &ran)
{
    const integer_t z = ran.to<uint64_t>( unsigned(ran.lt(63)) );
    return ran.choice() ? z : -z;
}

static inline integer_t z32(Random::Bits &ran)
{
    const integer_t z = ran.to<uint64_t>( unsigned(ran.lt(31)) );
    return ran.choice() ? z : -z;
}

Y_UTEST(apex_z)
{
    Random::ParkMiller ran;

    std::cerr << "Assign/Comparison 64 Bits" << std::endl;
    for(size_t iter=0;iter<1000;++iter)
    {
        const integer_t z = z64(ran);
        const Integer   Z = z;
        //std::cerr << Z << " / " << z << std::endl;

        Y_ASSERT( Z == z  );
        Y_ASSERT( z == Z  );
        Y_ASSERT( !(z!=Z) );
        Y_ASSERT( !(Z!=z) );
        Y_ASSERT( __Zero__ == Integer::Compare(Z,Z) );
        Y_ASSERT( __Zero__ == Integer::Compare(Z,z) );
        Y_ASSERT( __Zero__ == Integer::Compare(z,Z) );


        const integer_t  w = z64(ran);
        const Integer    W = w;
        const SignType   s = Sign::Of(z,w);
        Y_ASSERT( s == Integer::Compare(Z,W) );
        Y_ASSERT( s == Integer::Compare(z,W) );
        Y_ASSERT( s == Integer::Compare(Z,w) );

        if(w>=0)
        {
            const Natural N = w;
            Y_ASSERT( s == Integer::Compare(Z,N) );
        }

        if(z>=0)
        {
            const Natural N = z;
            Y_ASSERT( s == Integer::Compare(N,W) );
        }



    }

    std::cerr << "Serialize" << std::endl;
    {
        const String    fn = "n.dat";
        Vector<Integer> v;
        size_t          written = 0;
        {
            OutputFile fp(fn);
            for(size_t i=10+ran.leq(100);i>0;--i)
            {
                const Integer z(ran,ran.leq(4000));
                written += z.serialize(fp);
                v << z;
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
                const Integer z = Integer::Read(fp);
                Y_ASSERT(z == v[i]);
            }
        }
    }


    std::cerr << "Add 64" << std::endl;

    for(size_t iter=0;iter<1000;++iter)
    {
        const integer_t a = z64(ran);
        const integer_t b = z64(ran);
        const integer_t sum = a+b;
        const Integer   A = a;
        const Integer   B = b;
        const Integer   Sum = A+B;
        Y_ASSERT(Sum==sum);
        Y_ASSERT(Sum==A+b);
        Y_ASSERT(Sum==a+B);
        if(b>=0)
        {
            const Natural U = b;
            Y_ASSERT(Sum==A+U);
        }

        if(a>=0)
        {
            const Natural U = a;
            Y_ASSERT(Sum==U+B);
        }
    }

    std::cerr << "Sub 64" << std::endl;
    {
        for(size_t iter=0;iter<10000;++iter)
        {
            const integer_t a = z64(ran);
            const integer_t b = z64(ran);
            const integer_t dif = a-b;
            const Integer   A = a;
            const Integer   B = b;
            const Integer   Dif = A-B;
            Y_ASSERT(Dif==dif);
            Y_ASSERT(Dif==A-b);
            Y_ASSERT(Dif==a-B);
            if(b>=0)
            {
                const Natural U = b;
                Y_ASSERT(Dif==A-U);
            }
            
            if(a>=0)
            {
                const Natural U = a;
                Y_ASSERT(Dif==U-B);
            }
        }
    }

    for(Integer z=-5;z<=5;++z)
    {
        std::cerr << " " << z;
    }
    std::cerr << std::endl;

    for(Integer z=-5;z<=5;z++)
    {
        std::cerr << " " << z;
    }
    std::cerr << std::endl;


    for(Integer z=5;z>=-5;--z)
    {
        std::cerr << " " << z;
    }
    std::cerr << std::endl;

    for(Integer z=5;z>=-5;z--)
    {
        std::cerr << " " << z;
    }
    std::cerr << std::endl;


    std::cerr << "Add/Sub" << std::endl;

    for(size_t cycle=0;cycle<10000;++cycle)
    {
        const Integer A(ran, ran.lt(5000) );
        {
            const Integer B(ran, ran.lt(5000) );
            const Integer S = A+B;
            Y_ASSERT(S-A==B);
            Y_ASSERT(S-B==A);
        }
        {
            const integer_t b = z64(ran);
            const Integer   S = A+b;
            Y_ASSERT(S-b==A);
            Y_ASSERT(b-S==-A);
        }
        {
            const Natural N(ran, ran.lt(5000) );
            const Integer S = A+N;
            Y_ASSERT(S-N==A);
            Y_ASSERT(N-S==-A);
        }
    }


    std::cerr << "Mul64" << std::endl;
    for(size_t cycle=0;cycle<10000;++cycle)
    {
        const integer_t a = z32(ran);
        const integer_t b = z32(ran);
        const integer_t p = a*b;
        const Integer   A = a;
        const Integer   B = b;
        const Integer   P = A*B;
        Y_ASSERT(p==P);
        Y_ASSERT(P == A*b);
        Y_ASSERT(P == b*A);

        if(a>=0)
        {
            const Natural U = a;
            Y_ASSERT(P==U*B);
        }

        if(b>=0)
        {
            const Natural U = b;
            Y_ASSERT(P==A*U);
        }
    }

    std::cerr << "Div64" << std::endl;
    for(size_t cycle=0;cycle<10000;++cycle)
    {
        const integer_t a = z64(ran);
        integer_t       b = z64(ran);
        while(0==b)     b = z64(ran);
        const integer_t q = a/b;
        const Integer   A = a;
        const Integer   B = b;
        const Integer   Q = A/B;
        Y_ASSERT(Q==q);

        if(b>0)
        {
            const Natural U = b;
            Y_ASSERT(A/U==Q);
        }

        if(a>0)
        {
            const Natural U = a;
            Y_ASSERT(U/B==Q);
        }

    }


}
Y_UDONE()
