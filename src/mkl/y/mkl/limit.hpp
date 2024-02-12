
//! \file

#ifndef Y_MKL_Limit_Included
#define Y_MKL_Limit_Included 1

#include "y/type/args.hpp"
#include "y/config/shallow.hpp"
#include <iostream>

namespace Yttrium
{

    namespace MKL
    {
        Y_SHALLOW_DECL(Infinity); //!< symbolic Infinity

        //______________________________________________________________________
        //
        //
        //
        //! Limit type
        //
        //
        //______________________________________________________________________
        enum LimitType
        {
            UnboundedLimit, //!< +/- infinity
            IncludingLimit, //!< take value
            ExcludingLimit  //!< exclude value
        };

        //______________________________________________________________________
        //
        //
        //
        //! Limit has a value and a type
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Limit
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_EXPOSE(T,Type); //!< aliases

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline  Limit(const Infinity_ &) : value(0), type(UnboundedLimit) {} //!< set unbounded
            inline  Limit(const Limit &l) : value(l.value), type(l.type)      {} //!< copy
            inline ~Limit() noexcept                                          {} //!< cleanup
            inline  Limit(ConstType x) : value(x), type(IncludingLimit)       {} //!< setup default

            //! more generic setup
            inline Limit(ConstType x, const bool includes) : value(x), type(includes?IncludingLimit:ExcludingLimit) {}

            //__________________________________________________________________
            //
            //
            // members
            //
            //__________________________________________________________________
            ConstType       value; //!< limit value, meaningless if type==Unbounded
            const LimitType type;  //!< type of limit

        private:
            Y_DISABLE_ASSIGN(Limit);
        };

    }

}

#endif
