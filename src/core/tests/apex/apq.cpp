


#include "y/apex/rational.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

#include <cstdio>

using namespace Yttrium;


Y_UTEST(apex_q)
{
    Random::Rand ran;

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
        const apz     z1(1);
        Y_CHECK(q1_2<q4_3);
        Y_CHECK(q1_2<=q4_3);
        Y_CHECK(q4_3>q1_2);
        Y_CHECK(q4_3>=q1_2);

        Y_CHECK(q1_2<i1);
        Y_CHECK(q1_2<=i1);
        Y_CHECK(i1<q4_3);
        Y_CHECK(i1<=q4_3);

        Y_CHECK(q1_2<z1);
        Y_CHECK(q1_2<=z1);
        Y_CHECK(z1<q4_3);
        Y_CHECK(z1<=q4_3);


    }

    // square root
    {
        for(size_t i=0;i<10;++i)
        {
            apq q(32,25,ran);
            q = q.abs();
            apq s = q.sqrt();
            apq s2 = s*s;
            std::cerr << q << "=> " << s << std::endl;
            Y_ASSERT(s2<=q);
        }
    }




}
Y_UDONE()
