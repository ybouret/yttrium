
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
    }

}

