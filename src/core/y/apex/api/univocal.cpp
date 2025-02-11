
#include "y/apex/api/univocal.hpp"

namespace Yttrium
{
    namespace Apex
    {

        bool Univocal:: MustSwapSigns(const size_t p, const size_t n, const SignType f) noexcept
        {
            assert(f!=__Zero__);
            return n>p || (n==p && f == Negative);
        }

        Natural Univocal:: MakeNatural(Writable<Natural> &arr)
        {
            //--------------------------------------------------------------
            //
            // initialize
            //
            //--------------------------------------------------------------
            const size_t s  = arr.size();
            Natural      g  = 0;
            size_t       p  = 0;

            //--------------------------------------------------------------
            //
            // counting and computing gcd
            //
            //--------------------------------------------------------------
            {
                size_t       t  = s;

                //----------------------------------------------------------
                // find first positive and initialize gcd
                //----------------------------------------------------------
                while(t>0)
                {
                    const Natural &x = arr[t--];
                    if(x->bits>0)
                    {
                        g=x;
                        p=1;
                        break;
                    }
                }

                //----------------------------------------------------------
                // find other positive and update gcd
                //----------------------------------------------------------
                while(t>0)
                {
                    assert(g>0);
                    const Natural &x = arr[t--];
                    if(x->bits>0)
                    {
                        ++p;
                        g = Natural::GCD_(g,x);
                    }
                }
            }

            //--------------------------------------------------------------
            //
            // act depending on p
            //
            //--------------------------------------------------------------
            switch(p)
            {
                case 0:
                    //------------------------------------------------------
                    // all zero
                    //------------------------------------------------------
                    return 0;

                case 1:
                    //------------------------------------------------------
                    // set single to 1
                    //------------------------------------------------------
                    for(size_t i=1;;++i)
                    {
                        Natural &x = arr[i];
                        if(x->bits>0)
                        {
                            x = 1;
                            break;
                        }
                    }
                    return 1;

                default:
                    break;
            }

            //--------------------------------------------------------------
            //
            // generic case
            //
            //--------------------------------------------------------------
            Natural res = 0;
            if(g>1)
            {
                //----------------------------------------------------------
                // normalize
                //----------------------------------------------------------
                for(size_t i=s;i>0;--i) {
                    const Natural y = (arr[i]/=g);
                    res += y.sqr();
                }
            }
            else
            {
                //----------------------------------------------------------
                // untouched
                //----------------------------------------------------------
                for(size_t i=s;i>0;--i) {
                    const Natural &x = arr[i]; if(x->bits<=0) continue;
                    const Natural y = x;
                    res += y.sqr();
                }
            }
            return res;
        }


    }

}

