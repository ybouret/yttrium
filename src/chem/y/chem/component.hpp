//! \file

#ifndef Y_Chemical_Component_Included
#define Y_Chemical_Component_Included 1

#include "y/chem/species.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Component is a species with a not-zero coefficient
        //
        //
        //______________________________________________________________________
        class Component
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            typedef SuffixSet<String,Component> DataBase; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Component(const Species &which,
                      const int      coeff) noexcept; //!< setup
            ~Component()                    noexcept; //!< cleanup
            Component(const Component &)    noexcept; //!< copy

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const String &key() const noexcept; //!< species name

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Species &sp; //!< species
            const int      nu; //!< coefficient

        private:
            Y_DISABLE_ASSIGN(Component);
        };

    }

}

#endif
