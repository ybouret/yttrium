//! \file

#ifndef Y_Apex_Univocal_Included
#define Y_Apex_Univocal_Included 1

#include "y/apex/rational.hpp"
#include "y/calculus/gcd.hpp"

namespace Yttrium
{
    namespace Apex
    {

        struct Univocal
        {

            //! Unsigned Integral
            template <typename T> static inline
            Natural MakeUnsigned(Writable<T> &arr)
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                const size_t s  = arr.size();
                T            g  = 0;
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
                        const T &x = arr[t--];
                        if(x>0)
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
                        const T &x = arr[t--];
                        if(x>0)
                        {
                            ++p;
                            g = GreatestCommonDivisor(g,x);
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
                            T &x = arr[i];
                            if(0!=x)
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
                        const T      &x = arr[i]; if(x<=0) continue;
                        const Natural y = x;
                        res += y.sqr();
                    }
                }
                return res;
            }


            static
            bool MustSwapSigns(const size_t p, const size_t n, const SignType f) noexcept
            {
                assert(f!=__Zero__);
                return n>p || (n==p && f == Negative);
            }

            static const unsigned Untouched = 0x00;
            static const unsigned Normalize = 0x01;
            static const unsigned SwapSigns = 0x02;



            //! Signed Integral
            template <typename T> static inline
            Natural MakeSigned(Writable<T> &arr)
            {
                typedef typename IntegerFor<T>::UInt::Type U;
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                const size_t s  = arr.size();
                U            g  = 0;
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
                        const T &x = arr[t--];
                        switch( Sign::Of(x) )
                        {
                            case __Zero__: continue;
                            case Positive: f = Positive; p=1; a=1; g = static_cast<U>(x);  goto FOUND;
                            case Negative: f = Negative; n=1; a=1; g = static_cast<U>(-x); goto FOUND;
                        }
                    }

                FOUND:
                    //----------------------------------------------------------
                    // find other not zero and update gcd
                    //----------------------------------------------------------
                    while(t>0)
                    {
                        assert(g>0);
                        const T &x = arr[t--];
                        switch( Sign::Of(x) )
                        {
                            case __Zero__: continue;
                            case Positive: f = Positive; ++p; ++a; g = GreatestCommonDivisor(g,static_cast<U>(x) ); continue;
                            case Negative: f = Negative; ++n; ++a; g = GreatestCommonDivisor(g,static_cast<U>(-x)); continue;
                        }
                    }
                }

                std::cerr << "a=" << a << ", p=" << p << ", n=" << n << ", f=" << Sign::ToChar(f) << ", g=" << g << std::endl;

                //--------------------------------------------------------------
                //
                // act upon topology
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
                            T &x = arr[i];
                            if(0!=x)
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

                switch(flags)
                {
                    case SwapSigns:
                        for(size_t i=s;i>0;--i) {
                            T &x = arr[i];
                            switch(Sign::Of(x))
                            {
                                case __Zero__: continue;
                                case Positive: { const Natural y =  static_cast<U>(x);  x=-x; res += y.sqr(); } continue;
                                case Negative: { const Natural y =  static_cast<U>(-x); x=-x; res += y.sqr(); } continue;
                            }
                        }
                        break;

                    case Normalize:
                        for(size_t i=s;i>0;--i) {
                            T &x = arr[i];
                            switch(Sign::Of(x))
                            {
                                case __Zero__: continue;
                                case Positive: { const Natural y =  static_cast<U>(   x/=g );  res += y.sqr(); } continue;
                                case Negative: { const Natural y =  static_cast<U>( -(x/=g) ); res += y.sqr(); } continue;
                            }
                        }
                        break;

                    case Normalize|SwapSigns:
                        for(size_t i=s;i>0;--i) {
                            T &x = arr[i];
                            switch(Sign::Of(x))
                            {
                                case __Zero__: continue;
                                case Positive: { const Natural y =  static_cast<U>(   x/=g );  x=-x; res += y.sqr(); } continue;
                                case Negative: { const Natural y =  static_cast<U>( -(x/=g) ); x=-x; res += y.sqr(); } continue;
                            }
                        }
                        break;

                    default:
                        assert(Untouched==flags);
                        for(size_t i=s;i>0;--i) {
                            const T &x = arr[i];
                            switch(Sign::Of(x))
                            {
                                case __Zero__: continue;
                                case Positive: { const Natural y = static_cast<U>(x);  res += y.sqr(); } continue;
                                case Negative: { const Natural y = static_cast<U>(-x); res += y.sqr(); } continue;
                            }
                        }
                }

                return res;
            }

        };

    }

}

#endif
