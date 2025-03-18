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
        //! Role of an actor
        //
        //
        //______________________________________________________________________
        enum Role
        {
            Reactant, //!< use as reactant
            Product   //!< use as product
        };

        //______________________________________________________________________
        //
        //
        //
        //! persistent Actor + its role
        //
        //
        //______________________________________________________________________
        class Component :  public Proxy<const Actor>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Component(const Role, const Actor &) noexcept; //!< setup with PERSISTENT actor
            virtual ~Component()                          noexcept; //!< cleanup
            Component(const Component &)                  noexcept; //!< no-throw copy

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const String &      key()          const noexcept; //!< species key
            const char *        roleText()     const noexcept; //!< human readable role
            static const char * RoleText(const Role) noexcept; //!< human readable role

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Role     role; //!< actor's rile
        private:
            Y_DISABLE_ASSIGN(Component);
            Y_PROXY_DECL();
            const Actor &actor;
        };

    }

}

#endif
