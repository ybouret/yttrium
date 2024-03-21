#include "y/apex/modular.hpp"
#include "y/apex/integer.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"

using namespace Yttrium;

namespace {

}

Y_UTEST(apex_modular)
{
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

    for(size_t loop=0;loop<64;++loop)
    {
        const apn base(1+ran.leq(15),ran);
        const apn exponent(1+ran.leq(10),ran);
        const apn modulus(5+ran.leq(10),ran);
        (std::cerr << base << "^" << exponent << "[" << modulus << "] = ").flush();

        apn result = 1;
        for(apn i=exponent;i>0;--i)
        {
            result *= base;
        }
        result %= modulus;
        std::cerr << result;

        const apn check = Apex::Modular::Exp(base, exponent, modulus);
        std::cerr << " / " << check;
        std::cerr << std::endl;

    }


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



}
Y_UDONE()

