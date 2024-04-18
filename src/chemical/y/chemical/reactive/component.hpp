//! \file

#ifndef Y_Chemical_Component_Included
#define Y_Chemical_Component_Included 1

#include "y/chemical/species.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Component = algebraic coefficients + species
        //
        //
        //______________________________________________________________________
        class Component
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef SuffixSet<String,Component> Set; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Component(const int coef, const Species &spec) noexcept; //!< setup
            Component(const Component &) noexcept;                   //!< copy
            ~Component() noexcept;                                   //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const String &key() const noexcept; //!< sp.name

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const int      nu; //!< stoichio
            const Species &sp; //!< species
            
        private:
            Y_DISABLE_ASSIGN(Component);
        };

    }

}

#endif
