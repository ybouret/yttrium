
#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace Apex
    {

        //----------------------------------------------------------------------
        //
        // default comparasion
        //
        //----------------------------------------------------------------------
        SignType Integer:: Compare(const Integer &lhs, const Integer &rhs) noexcept
        {

            switch( Sign::MakePair(lhs.s,rhs.s) )
            {
                case NZ_Signs:
                case ZP_Signs:
                case NP_Signs:
                    return Negative;
                case PN_Signs:
                case ZN_Signs:
                case PZ_Signs:
                    return Positive;
                case PP_Signs: return Natural::Compare(lhs.n,rhs.n);
                case NN_Signs: return Natural::Compare(rhs.n,lhs.n);
                case ZZ_Signs: break;
            }
            return __Zero__;
        }

        //----------------------------------------------------------------------
        //
        // alias
        //
        //----------------------------------------------------------------------
        SignType Integer:: Cmp(const Integer &lhs, const Integer &rhs) noexcept
        {
            return Compare(lhs,rhs);
        }

        //----------------------------------------------------------------------
        //
        // comparisons with integral
        //
        //----------------------------------------------------------------------
        SignType Integer:: Cmp(const Integer &lhs, const int64_t rhs) noexcept
        {
            const SignType rs = Sign::Of(rhs);
            switch( Sign::MakePair(lhs.s,rs) )
            {
                case NZ_Signs:
                case ZP_Signs:
                case NP_Signs:
                    return Negative;
                    
                case PN_Signs:
                case ZN_Signs:
                case PZ_Signs:
                    return Positive;

                case PP_Signs: return Natural::Compare(lhs.n,uint64_t(rhs));
                case NN_Signs: return Natural::Compare(uint64_t(-rhs),lhs.n);
                case ZZ_Signs: break;
            }
            return __Zero__;
        }

        SignType Integer:: Cmp(const uint64_t lhs, const Integer &rhs) noexcept
        {
            return  Sign::Opposite( Cmp(rhs,lhs) );
        }

        //----------------------------------------------------------------------
        //
        // comparison with Natural
        //
        //----------------------------------------------------------------------
        SignType Integer:: Cmp(const Integer &lhs, const Natural &rhs) noexcept
        {
            if( __Zero__ == rhs.getSign() )
            {
                return lhs.s;
            }
            else
            {
                assert(rhs>0);
                switch(lhs.s)
                {
                    case Negative: return Negative;
                    case __Zero__: return Negative;
                    case Positive:
                        break;
                }
                return Natural::Compare(lhs.n,rhs);
            }
        }

        SignType Integer:: Cmp(const Natural &lhs, const Integer &rhs) noexcept
        {
            return Sign::Opposite(Cmp(rhs,lhs));
        }


        
        



    }

}


