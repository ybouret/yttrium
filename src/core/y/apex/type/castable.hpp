
//! \file

#ifndef Y_Apex_Castable_Included
#define Y_Apex_Castable_Included 1

#include "y/apex/type/number.hpp"

namespace Yttrium
{
    namespace Apex
    {

        //______________________________________________________________________
        //
        //
        //
        //! base class for castable numbers
        //
        //
        //______________________________________________________________________
        class Castable : public Number
        {
        protected:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Castable() noexcept; //!< setup

        public:
            virtual ~Castable() noexcept; //!< cleanup


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! throw exception using callSign()
            void raiseCastOverflowFor(const char * const msg) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Castable);
        };

    }

}

#endif
