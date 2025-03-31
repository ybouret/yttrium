//! \file

#ifndef Y_Type_Momentary_Included
#define Y_Type_Momentary_Included 1

#include "y/type/temporary.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! temporary helpers
    //
    //__________________________________________________________________________

    struct Momentary
    {
        class On : public Temporary<bool>
        {
        public:
            explicit On(bool &flag) noexcept;
            virtual ~On()           noexcept;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(On);
        };

        class Off : public Temporary<bool>
        {
        public:
            explicit Off(bool &flag) noexcept;
            virtual ~Off()           noexcept;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Off);
        };


    };


}

#endif
