
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
            //! Check colinearity of int/unsigned/apn/apz/apq arrays
            //
            //__________________________________________________________________
            template <typename LHS, typename RHS> static inline
            bool Of(LHS &lhs, RHS &rhs)
            {
                assert( lhs.size() == rhs.size() );
                const size_t count = lhs.size();  // processing length
                Rational     ratio;               // initial ratio, 0 means undefined
                for(size_t i=count;i>0;--i)
                {
                    apq l = lhs[i];
                    apq r = rhs[i];
                    switch( Sign::MakePair(l.numer.s,r.numer.s) )
                    {
                            //--------------------------------------------------
                            // zero against not zero => false
                            //--------------------------------------------------
                        case ZP_Signs:
                        case ZN_Signs:
                        case PZ_Signs:
                        case NZ_Signs:
                            return false;

                            //--------------------------------------------------
                            // zero against zero => ok so far
                            //--------------------------------------------------
                        case ZZ_Signs:
                            continue;

                            //--------------------------------------------------
                            // produce a positive factor
                            //--------------------------------------------------
                        case NN_Signs:
                        case PP_Signs:
                            switch(ratio.numer.s)
                            {
                                case Negative: // ratio sign already negative! => false
                                    return false;

                                case __Zero__: // initialize to positive value
                                    ratio = l/r; assert(Positive==ratio.numer.s);
                                    break;

                                case Positive: // test propto
                                    if( l/r != ratio) return false;
                                    //if(r.n*numer != l.n*denom) return false;
                                    break;

                            }
                            break;

                            //--------------------------------------------------
                            // produce a negative factor
                            //--------------------------------------------------
                        case NP_Signs:
                        case PN_Signs:
                            switch(ratio.numer.s)
                            {
                                case Positive: // ratio sign already positive! => false
                                    return false;
                                case __Zero__: // initialize to negative value
                                    ratio = l/r; assert(Negative==ratio.numer.s);
                                    break;
                                case Negative: // test propto
                                    if( l/r != ratio) return false;
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
