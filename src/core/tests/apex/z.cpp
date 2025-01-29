

#include "y/apex/integer.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/stream/libc/output.hpp"
#include "y/stream/libc/input.hpp"
#include "y/sequence/vector.hpp"
#include "y/string.hpp"

using namespace Yttrium;
using namespace Apex;



Y_UTEST(apex_z)
{
    Random::ParkMiller ran;

    std::cerr << "Assign/Comparison 64 Bits" << std::endl;
    for(size_t iter=0;iter<1000;++iter)
    {
        integer_t       z = ran.to<uint64_t>( unsigned(ran.lt(64)) ); if(ran.choice()) z=-z;
        const Integer   Z = z;
        //std::cerr << Z << " / " << z << std::endl;

        Y_ASSERT( Z == z  );
        Y_ASSERT( z == Z  );
        Y_ASSERT( !(z!=Z) );
        Y_ASSERT( !(Z!=z) );
        Y_ASSERT( __Zero__ == Integer::Compare(Z,Z) );
        Y_ASSERT( __Zero__ == Integer::Compare(Z,z) );
        Y_ASSERT( __Zero__ == Integer::Compare(z,Z) );


        integer_t      w = ran.to<uint64_t>( unsigned(ran.lt(64)) ); if(ran.choice()) w=-w;
        const Integer  W = w;
        const SignType s = Sign::Of(z,w);
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


}
Y_UDONE()
