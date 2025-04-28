
#include "y/apex/api/simplify.hpp"
#include "y/apex/rational.hpp"

namespace Yttrium
{
    namespace Apex
    {


        Natural Simplify:: Array(Writable<Rational> &arr)
        {
            Natural      lcm = 1;
            const size_t len = arr.size();
            for(size_t i=len;i>0;--i)
                lcm = Natural::LCM(lcm,arr[i].denom);
            if(lcm>1)
            {
                for(size_t i=len;i>0;--i) {
                    arr[i] *= lcm;
                    assert(arr[i].denom==1);
                }
            }
            return lcm;
        }

        Natural Simplify:: Array(Writable<Integer> &arr)
        {
            const size_t n = arr.size();
            size_t       i = arr.size();
            Natural      g = 1;

            while(i>0)
            {
                const Integer z = arr[i--];
                if(z.s!=__Zero__)
                {
                    g = z.n;
                    break;
                }
            }

            while(i>0)
            {
                const Integer z = arr[i--];
                if(z.s!=__Zero__)
                {
                    g = Natural::GCD_(g,z.n);
                }
            }

            if(g>1) {
                for(size_t i=n;i>0;--i) Coerce(arr[i].n) /= g;
            }

            return g;
        }


    }

}

