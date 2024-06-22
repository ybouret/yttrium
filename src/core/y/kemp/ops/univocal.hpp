

//! \file

#ifndef Y_Kemp_Rational_Univocal_Included
#define Y_Kemp_Rational_Univocal_Included 1

#include "y/kemp/rational.hpp"
#include "y/container/matrix.hpp"
#include "y/container/cxx/array.hpp"
#include "y/calculus/gcd.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        //______________________________________________________________________
        //
        //
        //
        //! Converting range/sequences to UNIVOCAL representation
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
            static const char * const CallSign; //!< "Kemp::Univocal"


            //__________________________________________________________________
            //
            //
            //! Make univocal range of Unsigned arithmetic type
            //
            //__________________________________________________________________
            template <typename T, typename ITERATOR> static inline
            void MakeUnsigned(const Type2Type<T> &, ITERATOR curr, const size_t size)
            {

                //--------------------------------------------------------------
                //
                // sorting out cases
                //
                //--------------------------------------------------------------
                switch(size)
                {
                        // do nothing
                    case 0: return;

                        // left zero untouched or make positive 1
                    case 1: { T &n = Coerce(*curr); if(n>0) n = 1;} return;

                        // generic case
                    default: break;
                }

                //--------------------------------------------------------------
                //
                // no problem of sign, just collect GCD
                //
                //--------------------------------------------------------------
                T g = *curr;
                {
                    ITERATOR temp = curr;
                    for(size_t i=size;i>1;--i)
                        g = GreatestCommonDivisor(g,*(++temp));
                }

                //--------------------------------------------------------------
                //
                // and simplify
                //
                //--------------------------------------------------------------
                if(g>1) {
                    for(size_t i=size;i>0;--i)
                        *(curr++) /= g;
                }
            }

            //__________________________________________________________________
            //
            //
            //! Make univocal sequence of Unsigned arithmetic type
            //
            //__________________________________________________________________
            template <typename SEQUENCE> static inline
            void MakeUnsigned(SEQUENCE &seq)
            {
                static const Type2Type< typename SEQUENCE::Type > which = {};
                MakeUnsigned(which,seq.begin(),seq.size());
            }


            //__________________________________________________________________
            //
            //
            //! Make univocal range of Signed arithmetic type
            //
            //__________________________________________________________________
            template <typename T, typename ITERATOR> static inline
            void MakeSigned( const Type2Type<T> &, ITERATOR curr, const size_t size)
            {
                typedef typename UnsignedInt<sizeof(T)>::Type U;

                //--------------------------------------------------------------
                //
                // sorting out cases
                //
                //--------------------------------------------------------------
                switch(size)
                {
                        // do nothing
                    case 0: return;

                        // left zero untouched or make positive 1
                    case 1: { T &z = Coerce(*curr); if(0!=z) z = 1;} return;

                        // generic case
                    default: break;
                }

                //--------------------------------------------------------------
                //
                // count signs and update GCD
                //
                //--------------------------------------------------------------
                size_t   numPos    = 0;
                size_t   numNeg    = 0;
                SignType firstSign = __Zero__;
                U        u         = AriDispatch<U,T>(numPos,numNeg,firstSign,*curr);
                {
                    ITERATOR temp = curr;
                    for(size_t i=size;i>1;--i)
                        u = GreatestCommonDivisor(u,AriDispatch<U,T>(numPos,numNeg,firstSign,*(++temp)));
                }
                const T g = static_cast<T>(u);

                //--------------------------------------------------------------
                //
                // update according to signs majority and simplify by g
                //
                //--------------------------------------------------------------
                switch( Sign::Of(numPos,numNeg) )
                {
                    case Negative: assert(numPos<numNeg); AriDivByNeg(curr,size,g); break;
                    case Positive: assert(numPos>numNeg); AriDivByPos(curr,size,g); break;
                    case __Zero__: assert(numPos==numNeg);
                        switch(firstSign)
                        {
                            case __Zero__: break; // all zero
                            case Positive: AriDivByPos(curr,size,g); break;
                            case Negative: AriDivByNeg(curr,size,g); break;
                        }
                        break;
                }
                }

            //__________________________________________________________________
            //
            //
            //! Make univocal sequence of Unsigned arithmetic type
            //
            //__________________________________________________________________
            template <typename SEQUENCE> static inline
            void MakeSigned(SEQUENCE &seq)
            {
                static const Type2Type< typename SEQUENCE::Type > which = {};
                MakeSigned(which,seq.begin(),seq.size());
            }


            //__________________________________________________________________
            //
            //
            //! Make univocal range of Naturals
            //
            //__________________________________________________________________
            template <typename ITERATOR> static inline
            void MakeNatural(ITERATOR curr, const size_t size)
            {
                //--------------------------------------------------------------
                //
                // sorting out cases
                //
                //--------------------------------------------------------------
                switch(size)
                {
                        // do nothing
                    case 0: return;

                        // left zero untouched or make positive 1
                    case 1: { apn &n = Coerce(*curr); if(n.bits()>0) n = 1;} return;

                        // generic case
                    default: break;
                }

                //--------------------------------------------------------------
                //
                // Find and simplify by GCD
                //
                //--------------------------------------------------------------
                Natural g = *curr;
                {
                    ITERATOR temp = curr;
                    for(size_t i=size;i>1;--i)
                        g = Natural::GCD(g,*(++temp));
                }
                if(g>1) {
                    for(size_t i=size;i>0;--i)
                        *(curr++) /= g;
                }

            }

            //__________________________________________________________________
            //
            //
            //! Make univocal sequence of Integers
            //
            //__________________________________________________________________
            template <typename SEQUENCE> static inline
            void MakeNatural(SEQUENCE &seq) { MakeNatural(seq.begin(),seq.size()); }

            //__________________________________________________________________
            //
            //
            //! Make univocal range of Integers
            //
            //__________________________________________________________________
            template <typename ITERATOR> static inline
            void MakeInteger(ITERATOR curr, const size_t size)
            {
                //--------------------------------------------------------------
                //
                // sorting out cases
                //
                //--------------------------------------------------------------
                switch(size)
                {
                        // do nothing
                    case 0: return;

                        // left zero untouched or make positive 1
                    case 1: { apz &z = Coerce(*curr); if(__Zero__!=z.s) z = 1;} return;

                        // generic case
                    default: break;
                }

                //--------------------------------------------------------------
                //
                // count signs and update GCD
                //
                //--------------------------------------------------------------
                size_t   numPos    = 0;
                size_t   numNeg    = 0;
                SignType firstSign = __Zero__;
                Natural  g         = Dispatch(numPos,numNeg,firstSign,*curr);
                {
                    ITERATOR temp = curr;
                    for(size_t i=size;i>1;--i)
                        g = Natural::GCD(g,Dispatch(numPos,numNeg,firstSign,*(++temp)));
                }

                //--------------------------------------------------------------
                //
                // update according to signs majority and simplify by g
                //
                //--------------------------------------------------------------
                switch( Sign::Of(numPos,numNeg) )
                {
                    case Negative: assert(numPos<numNeg); DivByNeg(curr,size,g); break;
                    case Positive: assert(numPos>numNeg); DivByPos(curr,size,g); break;
                    case __Zero__: assert(numPos==numNeg);
                        switch(firstSign)
                        {
                            case __Zero__: break; // all zero
                            case Positive: DivByPos(curr,size,g); break;
                            case Negative: DivByNeg(curr,size,g); break;
                        }
                        break;
                }

            }


            //__________________________________________________________________
            //
            //
            //! Make univocal sequence of Integers
            //
            //__________________________________________________________________
            template <typename SEQUENCE> static inline
            void MakeInteger(SEQUENCE &seq) { MakeInteger(seq.begin(),seq.size()); }

            //__________________________________________________________________
            //
            //
            //! Make univocal range of Rationals
            //
            //__________________________________________________________________
            template <typename ITERATOR> static inline
            void MakeRational(ITERATOR curr, const size_t size)
            {
                //--------------------------------------------------------------
                //
                // sorting out cases
                //
                //--------------------------------------------------------------
                switch(size)
                {
                        // do nothing
                    case 0: return;

                        // left zero untouched or make positive 1
                    case 1: { apq &q = Coerce(*curr); if(__Zero__!=q.numer.s) q = 1;} return;

                        // generic case
                    default: break;
                }

                //--------------------------------------------------------------
                //
                // check univocal signs and set denominators to 1
                //
                //--------------------------------------------------------------
                Natural  g = 0; // final scaling factor
                {
                    //----------------------------------------------------------
                    //
                    // counting signs and computing common denominator
                    //
                    //----------------------------------------------------------
                    assert(size>=2);
                    size_t   numPos    = 0;
                    size_t   numNeg    = 0;
                    SignType firstSign = __Zero__;
                    Natural  commonDen = Dispatch(numPos,numNeg,firstSign,*curr);
                    {
                        ITERATOR temp = curr;
                        for(size_t i=size;i>1;--i)
                            commonDen = Natural::LCM(commonDen,Dispatch(numPos,numNeg,firstSign,*(++temp)));
                    }

                    //----------------------------------------------------------
                    //
                    // update according to signs majority and and gather GCD
                    //
                    //----------------------------------------------------------
                    switch( Sign::Of(numPos,numNeg) )
                    {
                        case Negative: assert(numPos<numNeg); MulByNeg(commonDen,curr,size,g); break;
                        case Positive: assert(numPos>numNeg); MulByPos(commonDen,curr,size,g); break;
                        case __Zero__: assert(numPos==numNeg);
                            switch(firstSign)
                            {
                                case __Zero__: break; // all zero
                                case Positive: MulByPos(commonDen,curr,size,g); break;
                                case Negative: MulByNeg(commonDen,curr,size,g); break;
                            }
                            break;
                    }

                }

                //--------------------------------------------------------------
                //
                // Final reduction by GCD
                //
                //--------------------------------------------------------------
                if(g>1)
                {
                    for(size_t i=size;i>0;--i,++curr)
                    {
                        const apq &q = (*curr); assert(1==q.denom);
                        switch(q.numer.s)
                        {
                            case __Zero__: continue;
                            case Negative:
                            case Positive:
                                Coerce(q.numer.n) /= g;
                                continue;
                        }
                    }
                }

            }

            //__________________________________________________________________
            //
            //
            //! Make univocal sequence of rationals
            //
            //__________________________________________________________________
            template <typename SEQUENCE> static inline
            void MakeRational(SEQUENCE &seq) { MakeRational(seq.begin(),seq.size()); }

            //__________________________________________________________________
            //
            //
            // Using proxies to convert any sequence
            //
            //__________________________________________________________________


            typedef TL3(Natural,Integer,Rational) BuiltIn;                   //!< alias
            static const int UseAPN = TL::IndexOf<BuiltIn,Natural>::  Value; //!< alias
            static const int UseAPZ = TL::IndexOf<BuiltIn,Integer>::  Value; //!< alias
            static const int UseAPQ = TL::IndexOf<BuiltIn,Rational>:: Value; //!< alias
            static const int UseARI = -1;                                    //!< alias


            //__________________________________________________________________
            //
            //
            //! Make univocal according to type
            /**
             - apn
             - apz
             - apq
             - signed
             - unsigned
             */
            //__________________________________________________________________
            template <typename SEQUENCE> static inline
            void Make(SEQUENCE &seq)
            {
                typedef typename SEQUENCE::Type Type;
                static const Int2Type< TL::IndexOf<BuiltIn,Type>::Value > choice = {};
                Call(seq,choice);
            }


            //__________________________________________________________________
            //
            //
            //! Make univocal matrix ROWS
            //
            //__________________________________________________________________
            template <typename T, typename ALLOC> static inline
            void MakeMatrix(Matrix<T,ALLOC> &M)
            {
                for(size_t i=M.rows;i>0;--i)
                    Make(M[i]);
            }


        private:

            //! forward to Natural
            template <typename SEQUENCE> static inline
            void Call(SEQUENCE &seq, const Int2Type<UseAPN> &)
            {
                MakeNatural(seq);
            }

            //! forward to Integer
            template <typename SEQUENCE> static inline
            void Call(SEQUENCE &seq, const Int2Type<UseAPZ> &)
            {
                MakeInteger(seq);
            }

            //! forward to Rational
            template <typename SEQUENCE> static inline
            void Call(SEQUENCE &seq, const Int2Type<UseAPQ> &)
            {
                MakeRational(seq);
            }

            //! dispatch to Unsigned/Signed
            template <typename SEQUENCE> static inline
            void Call(SEQUENCE &seq, const Int2Type<UseARI> &)
            {
                typedef typename SEQUENCE::Type Type;
                static const Int2Type< IsSigned<Type>::Value > which = {};
                Call_<SEQUENCE,sizeof(Type)>(seq,which);
            }


            //! forward to Unsigned
            template <typename SEQUENCE, size_t N> static inline
            void Call_(SEQUENCE &seq, const Int2Type<false> &)
            {
                MakeUnsigned(seq);
            }

            //! forward to Signed
            template <typename SEQUENCE, size_t N> static inline
            void Call_(SEQUENCE &seq, const Int2Type<true> &)
            {
                MakeSigned(seq);
            }


            static const Natural & Dispatch(size_t &numPos, size_t &numNeg, SignType &firstSign, const apq &q) noexcept;
            static  apq          & UpdateGCD(Natural &g, const apq &q);
            static const Natural & Dispatch(size_t &numPos, size_t &numNeg, SignType &firstSign, const apz &z) noexcept;

            template <typename U, typename T> static inline
            U AriDispatch(size_t &numPos, size_t &numNeg, SignType &firstSign, const T &z) noexcept
            {
                const SignType s = Sign::Of(z);

                if(__Zero__==firstSign) firstSign = s;

                switch(s)
                {
                    case Negative: ++numNeg; return static_cast<U>(-z);
                    case Positive: ++numPos; return static_cast<U>( z);
                    case __Zero__: break;
                }
                return 0;
            }
            

            template <typename ITERATOR> static inline
            void MulByPos(const Natural &common, ITERATOR curr, size_t n, Natural &g) { assert(0==g);
                while(n-- > 0) (void) UpdateGCD(g,(Coerce(*(curr++)) *= common));
            }

            template <typename ITERATOR> static inline
            void MulByNeg(const Natural &common, ITERATOR curr, size_t n, Natural &g) {
                while(n-- > 0) UpdateGCD(g,(Coerce(*(curr++)) *= common)).neg();
            }

            template <typename ITERATOR, typename T> static inline
            void DivByPos(ITERATOR curr, size_t n, const T &g) { assert(0!=g);
                while(n-- > 0) Coerce(*(curr++)) /= g;
            }


            template <typename ITERATOR, typename T> static inline
            void DivByNeg(ITERATOR curr, size_t n, const T &g) { assert(0!=g);
                while(n-- > 0) (Coerce(*(curr++)) /= g).neg();
            }

            template <typename ITERATOR, typename T> static inline
            void AriDivByPos(ITERATOR curr, size_t n, const T &g) { assert(0!=g);
                while(n-- > 0) Coerce(*(curr++)) /= g;
            }

            template <typename ITERATOR, typename T> static inline
            void AriDivByNeg(ITERATOR curr, size_t n, const T &g) { assert(0!=g);
                while(n-- > 0) Coerce(*(curr++)) /= -g;
            }


        };



    }

}

#endif

