
//! \file

#ifndef Y_Chemical_Equilibria_Fragment_Included
#define Y_Chemical_Equilibria_Fragment_Included 1

#include "y/chemical/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Fragment = equilibria+their species
        //
        //
        //______________________________________________________________________
        class Fragment : public EList
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Fragment() noexcept; //!< setup empty
            virtual ~Fragment() noexcept; //!< cleanup
            Fragment(const Fragment &);   //!< hard copy

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const SList species; //!< list of species
        private:
            Y_DISABLE_ASSIGN(Fragment);
        };
    }

}

#endif

