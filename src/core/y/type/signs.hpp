
//! \file

#ifndef Y_Type_Signs_Included
#define Y_Type_Signs_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! system wide signs
    //
    //_________________________________________________________________________
    enum SignType
    {
        Negative = -1, //!< alias for negative sign
        __Zero__ =  0, //!< alias for zero sign
        Positive =  1  //!< alias for positive sign
    };

    enum PairOfSigns
    {
        NN_Signs,
        NZ_Signs,
        NP_Signs,

        ZN_Signs,
        ZZ_Signs,
        ZP_Signs,

        PN_Signs,
        PZ_Signs,
        PP_Signs
    };

    struct Sign
    {

        //______________________________________________________________________
        //
        //
        //! single value sign
        //
        //______________________________________________________________________
        template <typename T> static inline
        SignType Of(T &value)
        {
            return value < 0 ? Negative : (0<value? Positive : __Zero__);
        }

        //______________________________________________________________________
        //
        //
        //! natural comparison to sign
        //
        //______________________________________________________________________
        template <typename LHS, typename RHS> static inline
        SignType Of(LHS &lhs, RHS &rhs)
        {
            if(lhs<rhs)
                return Negative;
            else
            {
                if(rhs<lhs)
                    return Positive;
                else
                    return __Zero__;
            }
        }


        //______________________________________________________________________
        //
        //
        //! opposite sign
        //
        //______________________________________________________________________
        static SignType    Opposite(const SignType s) noexcept;

        //______________________________________________________________________
        //
        //
        //! Make Pair of Signs
        //
        //______________________________________________________________________
        static PairOfSigns MakePair(const SignType lhs, const SignType rhs) noexcept;

        //______________________________________________________________________
        //
        //
        //! Make Pair From Signs of each argument
        //
        //______________________________________________________________________
        template <typename LHS, typename RHS> static inline
        PairOfSigns MakePairFrom( LHS &lhs, RHS &rhs ) noexcept
        {
            return MakePair( Of(lhs), Of(rhs) );
        }

    };









}


#endif

