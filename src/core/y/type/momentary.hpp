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
        //! force a flag to TRUE during a scope
        class On : public Temporary<bool>
        {
        public:
            explicit On(bool &flag) noexcept; //!< setup
            virtual ~On()           noexcept; //!< restore
        private:
            Y_DISABLE_COPY_AND_ASSIGN(On);
        };

        //! force a flag to FALSE during a scope
        class Off : public Temporary<bool>
        {
        public:
            explicit Off(bool &flag) noexcept; //!< setup
            virtual ~Off()           noexcept; //!< restore
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Off);
        };


    };


}

#endif
