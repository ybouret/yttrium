#include "y/apex/api/univocal.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Natural Univocal:: MakeRational(Writable<Rational> &arr)
        {
            const size_t s  = arr.size(); if(s<=0) return 0;
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
                        case Positive: f = Positive; p=1; a=1; l = q.denom; goto FOUND_L;
                        case Negative: f = Negative; n=1; a=1; l = q.denom; goto FOUND_L;
                    }
                }

            FOUND_L:
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

            switch(a)
            {
                case 0:
                    // all zero
                    return 0;

                case 1:
                    // set single to 1
                    for(size_t i=1;;++i)
                    {
                        Rational &q = arr[i];
                        if( __Zero__ != q.numer.s )
                        {
                            q = 1;
                            break;
                        }
                    }
                    return 1;

                default:
                    break;
            }


            //std::cerr << "(l=" << l << ")";

            const bool multiply = l->bits>1;
            Natural    g        = 0;
            {
                size_t i = s;
                while(i>0)
                {
                    Rational &q = arr[i--];
                    switch(q.numer.s)
                    {
                        case __Zero__: continue;
                        case Positive:
                        case Negative:
                            if(multiply) { q *=l; assert(1==q.denom); }
                            g = q.numer.n;
                            goto FOUND_G;
                    }
                }

            FOUND_G:
                while(i>0)
                {
                    Rational &q = arr[i--];
                    switch(q.numer.s)
                    {
                        case __Zero__: continue;
                        case Positive:
                        case Negative:
                            if(multiply) { q *=l; assert(1==q.denom); }
                            g = Natural::GCD_(g,q.numer.n);
                            continue;;
                    }
                }
            }
            //std::cerr << "(g=" << g << ")";

            Natural  res    = 0;
            unsigned                 flags  = Untouched;
            if(g->bits>1)            flags |= Normalize;
            if(MustSwapSigns(p,n,f)) flags |= SwapSigns;

            switch(flags)
            {

                case SwapSigns:
                    for(size_t i=s;i>0;--i)
                    {
                        Rational &q = arr[i]; assert(1==q.denom);
                        switch(q.numer.s)
                        {
                            case __Zero__:  continue;
                            case Positive: Coerce(q.numer.s) = Negative; break;
                            case Negative: Coerce(q.numer.s) = Positive; break;
                        }
                        assert(q.numer.n->bits>0);
                        res += q.numer.n.sqr();
                    }
                    break;

                case Normalize:
                    for(size_t i=s;i>0;--i)
                    {
                        Rational &q = arr[i]; assert(1==q.denom);
                        switch(q.numer.s)
                        {
                            case __Zero__:  continue;
                            case Positive:
                            case Negative:
                                break;

                        }
                        Coerce(q.numer.n) /= g;
                        assert(q.numer.n->bits>0);
                        res += q.numer.n.sqr();
                    }
                    break;

                case Normalize|SwapSigns:
                    for(size_t i=s;i>0;--i)
                    {
                        Rational &q = arr[i]; assert(1==q.denom);
                        switch(q.numer.s)
                        {
                            case __Zero__:  continue;
                            case Positive: Coerce(q.numer.s) = Negative; break;
                            case Negative: Coerce(q.numer.s) = Positive; break;

                        }
                        Coerce(q.numer.n) /= g;
                        assert(q.numer.n->bits>0);
                        res += q.numer.n.sqr();
                    }
                    break;

                default:
                    assert(Untouched==flags);
                    for(size_t i=s;i>0;--i)
                    {
                        const Rational &q = arr[i]; assert(1==q.denom);
                        switch(q.numer.s)
                        {
                            case __Zero__:  continue;
                            case Positive:
                            case Negative:
                                assert(q.numer.n->bits>0);
                                res += q.numer.n.sqr();
                                continue;
                        }
                    }
            }

            return res;
        }
    }

}

