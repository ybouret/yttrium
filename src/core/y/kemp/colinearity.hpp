
//! \file

#ifndef Y_Kemp_Colinearity_Included
#define Y_Kemp_Colinearity_Included 1

#include "y/kemp/rational.hpp"
#include "y/data/small/heavy/list/bare.hpp"


namespace Yttrium
{
    namespace Kemp
    {
        struct Colinearity
        {
            //__________________________________________________________________
            //
            //
            //! Check colinearity of int/unsigned/apz arrays
            //
            //__________________________________________________________________
            template <typename LHS, typename RHS> static inline
            bool Of(LHS &lhs, RHS &rhs)
            {
                const size_t n = lhs.size(); assert( lhs.size() == rhs.size() );

                SignType s = __Zero__;
                Natural  numer, denom;
                for(size_t i=n;i>0;--i)
                {
                    apz l = lhs[i];
                    apz r = rhs[i];
                    switch( Sign::MakePair(l.s,r.s) )
                    {
                            //--------------------------------------------------
                            // zero w.r.t not zero => false
                            //--------------------------------------------------
                        case ZP_Signs:
                        case ZN_Signs:
                        case PZ_Signs:
                        case NZ_Signs:
                            return false;

                            //--------------------------------------------------
                            // zero w.r.t zero => ok
                            //--------------------------------------------------
                        case ZZ_Signs:
                            continue;

                            //--------------------------------------------------
                            // produce a positive factor
                            //--------------------------------------------------
                        case NN_Signs:
                        case PP_Signs:
                            switch(s)
                            {
                                case Negative: // => false
                                    return false;
                                case __Zero__: // initialize to positive value
                                    s     = Positive;
                                    Natural::Simplify(numer=l.n,denom=r.n);
                                    break;
                                case Positive: // test propto
                                    if(r.n*numer != l.n*denom) return false;
                                    break;

                            }
                            break;

                            //--------------------------------------------------
                            // produce a negative factor
                            //--------------------------------------------------
                        case NP_Signs:
                        case PN_Signs:
                            switch(s)
                            {
                                case Positive: // => false
                                    return false;
                                case __Zero__: // initialize to negative value
                                    s     = Negative;
                                    Natural::Simplify(numer=l.n,denom=r.n);
                                    break;
                                case Negative: // test propto
                                    if(r.n*numer != l.n * denom) return false;
                                    break;
                            }
                            break;

                    }
                }

                return true;
            }
        };
    }

}

#endif
