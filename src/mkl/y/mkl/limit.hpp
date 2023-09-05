
//! \file

#ifndef Y_MKL_Limit_Included
#define Y_MKL_Limit_Included 1

#include "y/type/args.hpp"
#include <iostream>

namespace Yttrium
{

    namespace MKL
    {

        enum LimitType
        {
            LimitNone,   //!< +/- infinity
            LimitClosed, //!< take value
            LimitOpen    //!< exclude value
        };

        template <typename T>
        class Limit
        {
        public:
            Y_ARGS_EXPOSE(T,Type);

            inline  Limit() : value(0), type(LimitNone) {}
            inline  Limit(const Limit &l) : value(l.value), type(l.type) {}
            inline ~Limit() noexcept {}

            inline Limit(ConstType x) : value(x), type(LimitClosed) {}
            inline Limit(ConstType x, const bool taken) : value(x), type(taken?LimitClosed:LimitOpen) {}

            ConstType       value;
            const LimitType type;

        private:
            Y_DISABLE_ASSIGN(Limit);
        };

    }

}

#endif
