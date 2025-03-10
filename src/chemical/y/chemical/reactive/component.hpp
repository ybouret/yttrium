//! \file


#ifndef Y_Chemical_Component_Included
#define Y_Chemical_Component_Included 1


#include "y/chemical/reactive/actor.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        enum Role
        {
            Reactant,
            Product
        };

        class Component :  Proxy<const Actor>
        {
        public:
            static const char * RoleText(const Role) noexcept;

            explicit Component(const Role, const Actor &) noexcept;
            virtual ~Component() noexcept;
            Component(const Component &) noexcept;

            const String & key()      const noexcept;
            const char *   roleText() const noexcept;

            const Role     role;
        private:
            Y_DISABLE_ASSIGN(Component);
            Y_PROXY_DECL();
            const Actor &actor;
        };

    }

}

#endif
