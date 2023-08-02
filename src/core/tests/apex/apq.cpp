


#include "y/apex/rational.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

#include <cstdio>

using namespace Yttrium;


Y_UTEST(apex_q)
{
    // equality
    {
        const apq     q(2);
        const int64_t i = 2;
        const apz     z = 2;
        const apn     n = 2;
        Y_CHECK(q==q);
        Y_CHECK(q==i);
        Y_CHECK(i==q);
        Y_CHECK(q==z);
        Y_CHECK(z==q);
        Y_CHECK(q==n);
        Y_CHECK(n==q);
    }

    // difference
    {
        apq           q(-2);
        const int64_t i = 2;
        const apz     z = 2;
        const apn     n = 2;
        {
            apq f(2,3);
            Y_CHECK(f!=q);
        }
        Y_CHECK(!(q!=q));
        Y_CHECK(q!=i);
        Y_CHECK(i!=q);
        Y_CHECK(q!=z);
        Y_CHECK(z!=q);
        Y_CHECK(q!=n);
        Y_CHECK(n!=q);
        q = 2;
        Y_CHECK(!(q!=i));
        Y_CHECK(!(i!=q));
        Y_CHECK(!(q!=z));
        Y_CHECK(!(z!=q));
        Y_CHECK(!(q!=n));
        Y_CHECK(!(n!=q));
    }

    // comparison
    {
        const apq q1_2(1,2);
        const apq q4_3(4,3);
        const int64_t i1(1);
        Y_CHECK(q1_2<q4_3);
        Y_CHECK(q1_2<=q4_3);
        Y_CHECK(q4_3>q1_2);
        Y_CHECK(q4_3>=q1_2);
        Y_CHECK(q1_2<i1);
        Y_CHECK(q1_2<=i1);
        Y_CHECK(i1<q4_3);
        Y_CHECK(i1<=q4_3);


    }



    return 0;
    
    { apq q;      std::cerr << q << std::endl; }
    { apq q(5,2); std::cerr << q << std::endl; }
    { apq q(-6,14); std::cerr << q << std::endl; }

    {
        apq a(1,2);
        apq b(3,2);
        std::cerr << a+b << std::endl;
        b = apq(-1,2);
        std::cerr << a+b << std::endl;
        std::cerr << a+2 << std::endl;
        std::cerr << -3+a << std::endl;
        apz z =  -2;
        std::cerr << a+z << std::endl;
        std::cerr << z+a << std::endl;
        apn n = 2;
        std::cerr << a+n << std::endl;
        std::cerr << n+a << std::endl;
    }

    {
        apq a(1,2);
        apq b(1,3);
        apq c = a/b;
        std::cerr << c << std::endl;
    }

    {
        apq a(1,2);
        apq b(-1,3);
        apq c = a/b;
        std::cerr << c << std::endl;
    }

    {
        apq a(-1,2);
        apq b(1,3);
        apq c = a/b;
        std::cerr << c << std::endl;
    }

    {
        apq a(-1,2);
        apq b(-1,3);
        apq c = a/b;
        std::cerr << c << std::endl;
    }

}
Y_UDONE()
