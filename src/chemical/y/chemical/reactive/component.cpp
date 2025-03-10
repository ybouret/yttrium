
#include "y/chemical/reactive/component.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        Component:: ~Component() noexcept
        {
            
        }

        Component:: Component(const Role r, const Actor &a) noexcept :
        role(r),
        actor(a)
        {

        }

        Component:: Component(const Component &_) noexcept :
        role(_.role),
        actor(_.actor)
        {
        }

        const String & Component:: key() const noexcept
        {
            return actor.sp.key();
        }

        const char * Component:: roleText() const noexcept
        {
            return RoleText(role);
        }

        const char * Component:: RoleText(const Role role) noexcept
        {
            switch(role)
            {
                case Reactant: return "Reactant";
                case Product:  return "Product";
            }
            return Core::Unknown;
        }

        Y_PROXY_IMPL(Component,actor)
    }

}


