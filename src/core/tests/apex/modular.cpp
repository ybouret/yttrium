#include "y/apex/modular.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

namespace {
    /*
     function inverse(a, n)
       t := 0;     newt := 1
       r := n;     newr := a

       while newr ≠ 0 do
         quotient := r div newr
         (t, newt) := (newt, t − quotient × newt)
         (r, newr) := (newr, r − quotient × newr)

       if r > 1 then
         return "a is not invertible"
       if t < 0 then
         t := t + n

       return t
     */

    unsigned inverse(unsigned a, unsigned n)
    {
        int      t=0, newt=1;
        unsigned r=n, newr=a;

        while(newr!=0)
        {
            const int quotient = r/newr;
            {
                const int _ = t-quotient*newt;
                t = newt;
                newt = _;
            }
            {
                const unsigned _ = r - quotient * newr;
                r = newr;
                newr = _;
            }
        }
        if(r>1)
            throw Exception("bad");
        if(t<0) t+=n;
        return t;
    }
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



    {
        const unsigned a = 2;
        const unsigned n = 97;
        const unsigned b = inverse(a,n);
        std::cerr << a << "^(-1) [" << n << "] = " << b << std::endl;
    }

}
Y_UDONE()

