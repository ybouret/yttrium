
//! \file

#ifndef Y_Chemical_Component_Included
#define Y_Chemical_Component_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/assembly.hpp"
#include "y/chemical/types.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Component:: algebraic coefficient + species
        //
        //
        //______________________________________________________________________
        class Component : public Object, public Counted
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ArkPtr<String,const Component> Ptr; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Component(const int n, const Species &s); //!< setup
            virtual ~Component() noexcept;                     //!< cleanup
            Y_OSTREAM_PROTO(Component);                        //!< display
            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const String & key() const noexcept; //!< for associative

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const int      nu; //!< nu != 0
            const Species &sp; //!< species
            const xreal_t  xn; //!< nu

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Component);

       
        };

    }

}

#endif
