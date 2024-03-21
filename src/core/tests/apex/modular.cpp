#include "y/apex/modular.hpp"
#include "y/apex/integer.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"
#include "y/calculus/prime.hpp"

using namespace Yttrium;

namespace {

}

Y_UTEST(apex_modular)
{
    size_t Loops = 32;

    std::cerr << "Modular Exponent Example" << std::endl;
    {
        const apn base     = 4;
        const apn exponent = 13;
        const apn modulus  = 497;

        apn result = 1;
        for(apn i=exponent;i>0;--i)
        {
            result *= base;
        }
        result %= modulus;
        Y_CHECK(445==result);

        const apn check = Apex::Modular::Exp(base, exponent, modulus);
        Y_CHECK(result==check);
    }

    Random::Rand ran;

    std::cerr << "Modular Exponent Checking" << std::endl;
    for(size_t loop=0;loop<Loops;++loop)
    {
        const apn base(1+ran.leq(15),ran);
        const apn exponent(1+ran.leq(10),ran);
        const apn modulus(5+ran.leq(10),ran);
        (std::cerr << std::setw(6) << base << "^" << std::setw(5) << exponent << " [" << std::setw(5) << modulus << "] = ").flush();

        apn result = 1;
        for(apn i=exponent;i>0;--i)
        {
            result *= base;
        }
        result %= modulus;
        std::cerr << result;

        const apn check = Apex::Modular::Exp(base, exponent, modulus);
        Y_ASSERT(check==result);
        std::cerr << std::endl;

    }

    std::cerr << "Modular Inverse Example" << std::endl;
    {
        const unsigned a = 2;
        const unsigned n = 97;
        //const unsigned b = inverse(a,n);
        //std::cerr << a << "^(-1) [" << n << "] = " << b << std::endl;

        const apn A = a;
        const apn N = n;
        const apn B = Apex::Modular::Inv(A,N);
        std::cerr << A << "^(-1) [" << N << "] = " << B << std::endl;

    }

    std::cerr << "Modular Inverse Checking" << std::endl;
    for(size_t loop=0;loop<Loops;++loop)
    {
        const size_t   i = ran.in<size_t>(1,Core::Prime::Pi16);
        const uint64_t n = Core::Prime::Get(i);
        const apn      N = n;
        std::cerr << std::setw(6) << n << " : [";

        for(size_t k=0;k<Loops;++k)
        {
            const uint64_t a = ran.in<uint64_t>(1,n-1);
            (std::cerr << '.').flush();
            const apn A = a;
            const apn B = Apex::Modular::Inv(A,N);
            const apn P = (A*B) % N;
            Y_ASSERT(1==P);
        }
        std::cerr << "]" << std::endl;
    }


}
Y_UDONE()

