
//! \file

#ifndef Y_Concurrent_Primitive_Included
#define Y_Concurrent_Primitive_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Concurrent
    {

        namespace Nucleus
        {
            class Quark;
        }

        //______________________________________________________________________
        //
        //
        //
        //! Primitive to fetch global Quark instance
        //
        //
        //______________________________________________________________________
        class Primitive
        {
        public:
            virtual ~Primitive() noexcept; //!< cleanup

        protected:
            explicit Primitive();          //!< call Quark Instance
            Nucleus::Quark &quark;         //!< global quark

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Primitive);
        };
    }

}

#endif

