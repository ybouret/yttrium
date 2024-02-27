

#include "y/string.hpp"
#include "y/apex/rational.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"
#include "y/stream/libc/input.hpp"

#include "y/random/bits.hpp"


using namespace Yttrium;


Y_UTEST(io_ser)
{

    Random::Rand ran;

    String    s; for(size_t i=5+ran.leq(10);i>0;--i) s << ran.in<char>('a', 'z');
    const apn n(200,ran);
    const apz z(200,ran);
    const apq q(100,100,ran);

    std::cerr << "s = " << s << std::endl;
    std::cerr << "n = " << n << std::endl;
    std::cerr << "z = " << z << std::endl;
    std::cerr << "q = " << q << std::endl;

    {
        OutputFile fp("ser.dat");
        s.serialize(fp);
        n.serialize(fp);
        z.serialize(fp);
        q.serialize(fp);
    }

    {
        InputFile fp("ser.dat");
        const String S = String::ReadFrom(fp);
        std::cerr << "S = " << S << std::endl;
        const apn    N = apn::ReadFrom(fp);
        std::cerr << "N = " << N << std::endl;
        const apz    Z = apz::ReadFrom(fp);
        std::cerr << "Z = " << Z << std::endl;
        const apq    Q = apq::ReadFrom(fp);
        std::cerr << "Q = " << Q << std::endl;
        Y_CHECK(s==S);
        Y_CHECK(n==N);
        Y_CHECK(z==Z);
        Y_CHECK(q==Q);

    }


}
Y_UDONE()
