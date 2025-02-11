#include "y/apex/api/univocal.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Natural Univocal:: MakeInteger(Writable<Integer> &arr)
        {
            //--------------------------------------------------------------
            //
            // initialize
            //
            //--------------------------------------------------------------
            const size_t s  = arr.size();
            Natural      g  = 0;
            size_t       p  = 0;
            size_t       n  = 0;
            size_t       a  = 0;
            SignType     f  = __Zero__;

            //--------------------------------------------------------------
            //
            // counting and computing gcd
            //
            //--------------------------------------------------------------
            {
                size_t       t  = s;

                //----------------------------------------------------------
                // find first no zero and initialize gcd
                //----------------------------------------------------------
                while(t>0)
                {
                    const Integer &x = arr[t--];
                    switch( x.s )
                    {
                        case __Zero__: continue;
                        case Positive: f = Positive; p=1; a=1; g = x.n; goto FOUND;
                        case Negative: f = Negative; n=1; a=1; g = x.n; goto FOUND;
                    }
                }

            FOUND:
                //----------------------------------------------------------
                // find other not zero and update gcd
                //----------------------------------------------------------
                while(t>0)
                {
                    assert(g>0);
                    const Integer &x = arr[t--];
                    switch( x.s )
                    {
                        case __Zero__: continue;
                        case Positive: f = Positive; ++p; ++a; g = Natural::GCD_(g,x.n); continue;
                        case Negative: f = Negative; ++n; ++a; g = Natural::GCD_(g,x.n); continue;
                    }
                }
            }

            //std::cerr << "(a=" << a << ", p=" << p << ", n=" << n << ", f=" << Sign::ToChar(f) << ", g=" << g <<")";

            //--------------------------------------------------------------
            //
            // act upon signs topology
            //
            //--------------------------------------------------------------
            switch(a)
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
                    assert(1==p||1==n);
                    for(size_t i=1;;++i)
                    {
                        Integer &x = arr[i];
                        if(__Zero__!=x.s)
                        {
                            x = 1;
                            break;
                        }
                    }
                    return 1;

                default:
                    //------------------------------------------------------
                    // generic case
                    //------------------------------------------------------
                    break;
            }



            Natural  res    = 0;
            unsigned                 flags  = Untouched;
            if(g>1)                  flags |= Normalize;
            if(MustSwapSigns(p,n,f)) flags |= SwapSigns;

            //std::cerr << "(flags=" << flags <<")";

            switch(flags)
            {
                case SwapSigns:
                    for(size_t i=s;i>0;--i) {
                        Integer &x = arr[i];
                        switch(x.s)
                        {
                            case __Zero__: continue;
                            case Positive: res += x.n.sqr(); Coerce(x.s) = Negative; continue;
                            case Negative: res += x.n.sqr(); Coerce(x.s) = Positive; continue;
                        }
                    }
                    break;

                case Normalize: {
                    for(size_t i=s;i>0;--i) {
                        Integer &x = arr[i];
                        switch(x.s)
                        {
                            case __Zero__: continue;
                            case Positive:
                            case Negative:
                                res += (x/=g).n.sqr(); continue;
                        }
                    }
                } break;


                case Normalize|SwapSigns: {
                    for(size_t i=s;i>0;--i) {
                        Integer &x = arr[i];
                        switch(Sign::Of(x))
                        {
                            case __Zero__: continue;
                            case Positive: res += (x/=g).n.sqr(); Coerce(x.s) = Negative; continue;
                            case Negative: res += (x/=g).n.sqr(); Coerce(x.s) = Positive; continue;
                        }
                    }
                } break;

                default:
                    assert(Untouched==flags);
                    for(size_t i=s;i>0;--i) {
                        const Integer &x = arr[i];
                        switch(x.s)
                        {
                            case __Zero__: continue;
                            case Positive:
                            case Negative:
                                res += x.n.sqr(); continue;
                        }
                    }
            }

            return res;
        }

    }

}
