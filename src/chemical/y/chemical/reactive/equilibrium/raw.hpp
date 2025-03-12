//! \file

#ifndef Y_Chemical_Equilibrium_Raw_Included
#define Y_Chemical_Equilibrium_Raw_Included 1

#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Equilibrium with parsed data string
        //
        //
        //______________________________________________________________________
        class RawEquilibrium : public Equilibrium
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            explicit RawEquilibrium(Library &,  XNode * const); //!< setup
        public:
            virtual ~RawEquilibrium() noexcept;                 //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________

            //! string from parsed XNode
            /**
             - if numeric, assume constant
             - if function name, use internal Lua VM
             */
            const AutoPtr<const String> Kdata;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RawEquilibrium);
        };
    }

}

#endif

