#include "y/apex/api/univocal.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Natural Univocal:: MakeRational(Writable<Rational> &arr)
        {
            const size_t s = arr.size(); if(s<=0) return 0;
            size_t       p  = 0;
            size_t       n  = 0;
            size_t       a  = 0;
            SignType     f  = __Zero__;
            Natural      l  = 0;

            //--------------------------------------------------------------
            //
            // counting and computing lcm
            //
            //--------------------------------------------------------------
            {
                size_t       t  = s;

                //----------------------------------------------------------
                // find first no zero and initialize gcd
                //----------------------------------------------------------
                while(t>0)
                {
                    const Rational &q = arr[t--];
                    switch( q.numer.s )
                    {
                        case __Zero__: continue;
                        case Positive: f = Positive; p=1; a=1; l = q.denom; goto FOUND;
                        case Negative: f = Negative; n=1; a=1; l = q.denom;; goto FOUND;
                    }
                }

            FOUND:
                //----------------------------------------------------------
                // find other not zero and update gcd
                //----------------------------------------------------------
                while(t>0)
                {
                    const Rational &q = arr[t--];
                    switch( q.numer.s )
                    {
                        case __Zero__: continue;
                        case Positive: f = Positive; ++p; ++a; l = Natural::LCM(l,q.denom); continue;
                        case Negative: f = Negative; ++n; ++a; l = Natural::LCM(l,q.denom); continue;
                    }
                }
            }

            if(a<=0) return 0; // all zero

            std::cerr << "(l=" << l << ")";

            if(l>1)
            {
                for(size_t i=s;i>0;--i)
                {
                    Rational &q = arr[i];
                    switch(q.numer.s)
                    {
                        case __Zero__: continue;
                        case Positive:
                        case Negative:
                            q *= l; assert(1==q.denom);
                            break;
                    }
                }
            }

            return 0;
        }
    }

}

