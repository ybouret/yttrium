

//! \file

#ifndef Y_Kemp_Rational_Univocal_Included
#define Y_Kemp_Rational_Univocal_Included 1

#include "y/kemp/rational.hpp"
#include "y/container/matrix.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        struct Univocal
        {
            static const char * const CallSign; //!< "Kemp::Univocal"
            

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
                // Final reduction
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





            //! make for sequences
            typedef TL3(Natural,Integer,Rational) BuiltIn;
            enum {
                UseAPN = TL::IndexOf<BuiltIn,Natural>::  Value,
                UseAPZ = TL::IndexOf<BuiltIn,Integer>::  Value,
                UseAPQ = TL::IndexOf<BuiltIn,Rational>:: Value,
                UseARI = -1
            };
            /**
             - apn
             - apz
             - apq
             - signed -> apz + cast
             - unsigned -> apn + cast
             */
            template <typename SEQUENCE> static inline
            void Make(SEQUENCE &seq)
            {
                typedef typename SEQUENCE::Type Type;
                static const Int2Type< TL::IndexOf<BuiltIn,Type>::Value > choice = {};
                Call(seq,choice);
            }


            template <typename SEQUENCE> static inline
            void Call(SEQUENCE &seq, const Int2Type<UseAPN> &)
            {
                MakeNatural(seq);
            }

            template <typename SEQUENCE> static inline
            void Call(SEQUENCE &seq, const Int2Type<UseAPZ> &)
            {
                MakeInteger(seq);
            }


            template <typename SEQUENCE> static inline
            void Call(SEQUENCE &seq, const Int2Type<UseAPQ> &)
            {
                MakeRational(seq);
            }

            template <typename SEQUENCE> static inline
            void Call(SEQUENCE &seq, const Int2Type<UseARI> &)
            {
                typedef typename SEQUENCE::Type Type;
                static const Int2Type< IsSigned<Type>::Value > which = {};
                Call_<SEQUENCE,sizeof(Type)>(seq,which);
            }



            template <typename SEQUENCE, size_t N> static inline
            void Call_(SEQUENCE &seq, const Int2Type<false> &)
            {
                const size_t                     size = seq.size();
                CxxArray<Natural,Memory::Dyadic> temp(size);
                Load(temp,seq.begin(),size);
                std::cerr << "processing " << temp << " from " << seq << std::endl;
                MakeNatural(temp);
                std::cerr << " -> " << temp << std::endl;
                //Save(temp.begin(),size,temp);
            }

            template <typename TARGET, typename ITERATOR> static inline
            void Load(TARGET &tgt, ITERATOR curr, size_t size)
            {
                for(size_t i=1;i<=size;++i,++curr)
                {
                    tgt[i] = *curr;
                }
            }

            template <typename ITERATOR, typename SOURCE> static inline
            void Save(ITERATOR curr, size_t size, const SOURCE &src)
            {
                for(size_t i=1;i<=size;++i,++curr)
                {
                    *curr = src[i].template cast<typename SOURCE::Type>( CallSign );
                }
            }

            //__________________________________________________________________
            //
            //
            //! Make univocal rows of Integers
            //
            //__________________________________________________________________
            static void MakeMatrix(Matrix<apz> &a);



        private:
            static const Natural & Dispatch(size_t &numPos, size_t &numNeg, SignType &firstSign, const apq &q) noexcept;
            static  apq          & UpdateGCD(Natural &g, const apq &q);

            static const Natural & Dispatch(size_t &numPos, size_t &numNeg, SignType &firstSign, const apz &z) noexcept;


            template <typename ITERATOR> static inline
            void MulByPos(const Natural &common, ITERATOR curr, size_t n, Natural &g) { assert(0==g);
                while(n-- > 0) (void) UpdateGCD(g,(Coerce(*(curr++)) *= common));
            }

            template <typename ITERATOR> static inline
            void MulByNeg(const Natural &common, ITERATOR curr, size_t n, Natural &g) {
                while(n-- > 0) UpdateGCD(g,(Coerce(*(curr++)) *= common)).neg();
            }

            template <typename ITERATOR> static inline
            void DivByPos(ITERATOR curr, size_t n, const Natural &g) { assert(0!=g);
                while(n-- > 0) Coerce(*(curr++)) /= g;
            }


            template <typename ITERATOR> static inline
            void DivByNeg(ITERATOR curr, size_t n, const Natural &g) { assert(0!=g);
                while(n-- > 0) (Coerce(*(curr++)) /= g).neg();
            }

        };



    }

}

#endif

