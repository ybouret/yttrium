//! \file


#ifndef Y_Chemical_Formula_Included
#define Y_Chemical_Formula_Included 1


#include "y/chemical/type/xcode.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Species formula
        //
        //
        //______________________________________________________________________
        class Formula
        {
        public:
            static const char * const CallSign; //!< "Formulat"
            static const char * const Name; //!< "Name"
            static const char * const Body; //!< "Body"
            static const char * const Mult; //!< "Mult"
            static const char * const Z;    //!< "Z"

            explicit Formula(const XCode   &) noexcept;
            explicit Formula(const Formula &) noexcept;
            virtual ~Formula() noexcept;

        private:
            Y_DISABLE_ASSIGN(Formula);
            const XCode code;
        };
    }

}


#endif

