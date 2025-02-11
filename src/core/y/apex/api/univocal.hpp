//! \file

#ifndef Y_Apex_Univocal_Included
#define Y_Apex_Univocal_Included 1

#include "y/apex/rational.hpp"
#include "y/calculus/gcd.hpp"

namespace Yttrium
{
    namespace Apex
    {

        //______________________________________________________________________
        //
        //
        //
        //! Algorithms to produce univocal arrays
        //
        //
        //______________________________________________________________________
        struct Univocal
        {
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unsigned Untouched = 0x00; //!< left Untouched
            static const unsigned Normalize = 0x01; //!< must Normalize
            static const unsigned SwapSigns = 0x02; //!< must SwapSigns

            //! (n>p) || (n==p && f == Negative);
            static bool MustSwapSigns(const size_t p, const size_t n, const SignType f) noexcept;

            //__________________________________________________________________
            //
            //
            //! call algorithm for given type, return reduced |arr|^2
            //
            //__________________________________________________________________
            template <typename T> static inline
            Natural Make(Writable<T> &arr, size_t &nonZero)
            {
                typedef TL3(Natural,Integer,Rational) ListType;
                static const Int2Type<TL::IndexOf<ListType,T>::Value>   Kind = {};
                return Make(arr,nonZero,Kind);
            }

        private:
            template <typename T> static inline Natural Make(Writable<T> &arr, size_t &nonZero, const Int2Type<0> &) { return MakeNatural(arr,nonZero);  }
            template <typename T> static inline Natural Make(Writable<T> &arr, size_t &nonZero, const Int2Type<1> &) { return MakeInteger(arr,nonZero);  }
            template <typename T> static inline Natural Make(Writable<T> &arr, size_t &nonZero, const Int2Type<2> &) { return MakeRational(arr,nonZero); }
            template <typename T> static inline Natural Make(Writable<T> &arr, size_t &nonZero, const Int2Type< -1 > &)
            {
                static const Int2Type< IsSigned<T>::Value > SignKind = {};
                return Prim(arr,nonZero,SignKind);
            }
            template <typename T> static inline Natural Prim(Writable<T> &arr, size_t &nonZero, const Int2Type<false> &) { return MakeUnsigned(arr,nonZero); }
            template <typename T> static inline Natural Prim(Writable<T> &arr, size_t &nonZero, const Int2Type<true>  &) { return MakeSigned(arr,nonZero);   }




        public:
            static Natural MakeNatural(Writable<Natural>   &arr, size_t &nonZero); //!< make univocal array of Natural
            static Natural MakeInteger(Writable<Integer>   &arr, size_t &nonZero); //!< make univocal array of Integer
            static Natural MakeRational(Writable<Rational> &arr, size_t &nonZero); //!< make univocal array of Rational as Integer

            //! make univocal array of unsigned integrals
            template <typename T> static inline
            Natural MakeUnsigned(Writable<T> &arr, size_t &nonZero)
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                nonZero         = 0;
                const size_t s  = arr.size(); if(s<=0) return 0;
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
                        nonZero = 1;
                        return 1;

                    default:
                        nonZero = p;
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






            //! make univocal array of signed integrals
            template <typename T> static inline
            Natural MakeSigned(Writable<T> &arr, size_t &nonZero)
            {
                typedef typename IntegerFor<T>::UInt::Type U;
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                nonZero         = 0;
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
                            T &x = arr[i];
                            if(0!=x)
                            {
                                x = 1;
                                break;
                            }
                        }
                        nonZero = 1;
                        return 1;

                    default:
                        //------------------------------------------------------
                        // generic case
                        //------------------------------------------------------
                        nonZero = a;
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
                            T &x = arr[i];
                            switch(Sign::Of(x))
                            {
                                case __Zero__: continue;
                                case Positive: { const Natural y =  static_cast<U>(x);  x=-x; res += y.sqr(); } continue;
                                case Negative: { const Natural y =  static_cast<U>(-x); x=-x; res += y.sqr(); } continue;
                            }
                        }
                        break;

                    case Normalize: {
                        const T den = -static_cast<T>(g); assert( den < -1 );
                        for(size_t i=s;i>0;--i) {
                            T &x = arr[i];
                            switch(Sign::Of(x))
                            {
                                case __Zero__: continue;
                                case Positive: {  const Natural y =  static_cast<U>(  -(x/=den) ); res += y.sqr(); } continue;
                                case Negative: {  const Natural y =  static_cast<U>(   (x/=den) ); res += y.sqr(); } continue;
                            }
                        }
                    } break;

                    case Normalize|SwapSigns: {
                        const T den = -static_cast<T>(g); assert(den < -1);
                        for(size_t i=s;i>0;--i) {
                            T &x = arr[i];
                            switch(Sign::Of(x))
                            {
                                case __Zero__: continue;
                                case Positive: { const Natural y =  static_cast<U>( -(x/=den) ); res += y.sqr(); } continue;
                                case Negative: { const Natural y =  static_cast<U>(  (x/=den) ); res += y.sqr(); } continue;
                            }
                        }
                    } break;

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
