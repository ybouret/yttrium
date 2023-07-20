
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

    //__________________________________________________________________________
    //
    //
    //! system wide pair of signs
    //
    //__________________________________________________________________________
    enum PairOfSigns
    {
        NN_Signs, //!< Negative | Negative
        NZ_Signs, //!< Negative | __Zero__
        NP_Signs, //!< Negative | Positive

        ZN_Signs, //!< __Zero__ | Negative
        ZZ_Signs, //!< __Zero__ | __Zero__
        ZP_Signs, //!< __Zero__ | Positive

        PN_Signs, //!< Positive | Negative
        PZ_Signs, //!< Positive | __Zero__
        PP_Signs  //!< Positive | Positive
    };

    //__________________________________________________________________________
    //
    //
    //! Operations on signs
    //
    //__________________________________________________________________________
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

