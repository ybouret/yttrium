
//! \file

#ifndef Y_Ptr_Core_Included
#define Y_Ptr_Core_Included 1

#include "y/ptr/immediate.hpp"
#include <iostream>

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //
        //! Base class for smart pointers
        //
        //
        //______________________________________________________________________
        class Ptr
        {
        public:    static const char Nil[]; //!< "(nil)"
        protected: explicit Ptr() noexcept; //!< setup
        public:    virtual ~Ptr() noexcept; //!< cleanup
            //! abort() is NULL handle
            static void Critical(const void *, const char * const) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Ptr);
        };
    }

}


#endif
