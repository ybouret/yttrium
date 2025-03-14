
//! \file


#ifndef Y_Chemical_Fragment_Included
#define Y_Chemical_Fragment_Included 1

#include "y/chemical/reactive/components.hpp"
#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Helper to format equilibria/components
        //
        //
        //______________________________________________________________________
        class Fragment
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Fragment()                    noexcept; //!< setup
            virtual ~Fragment()                    noexcept; //!< cleanup
            Fragment(const Fragment &)             noexcept; //!< duplicate
            Fragment & operator=(const Fragment &) noexcept; //!< assign

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void           enroll(const Components &eq) noexcept;             //!< update all
            std::ostream & display(std::ostream &, const Components &) const; //!< default display

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Assembly self; //!< equilibria name
            Assembly reac; //!< reactants  name
            Assembly prod; //!< products   name

        };

    }

}


#endif

