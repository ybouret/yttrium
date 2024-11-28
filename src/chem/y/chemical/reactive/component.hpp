
//! \file

#ifndef Y_Chemical_Component_Included
#define Y_Chemical_Component_Included 1

#include "y/chemical/reactive/actor.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! kind of component
        //
        //
        //______________________________________________________________________
        enum Role
        {
            Reactant, //!< Reactant
            Product   //!< Product
        };

        //! human readable role
        const char * RoleToText(const Role) noexcept;

        //______________________________________________________________________
        //
        //
        //
        //! Component = Categorized Actor
        //
        //
        //______________________________________________________________________
        class Component
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Component(const Actor &, const Role) noexcept; //!< setup
            Component(const Component &)         noexcept; //!< duplicate
            ~Component()                         noexcept; //!< cleanup


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const String & key()  const noexcept; //!< species name for Components
            const char *   side() const noexcept; //!< ActingToText(role)

            void viz(OutputStream            &fp,
                     const GraphViz::Vizible &eq,
                     const char * const       color) const;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Actor &actor; //!< persistent actor
            const Role   role;  //!< its role



        private:
            Y_DISABLE_ASSIGN(Component);
        };

    }

}

#endif

