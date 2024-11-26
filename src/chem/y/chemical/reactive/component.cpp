
#include "y/chemical/reactive/component.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * ActingToText(const Acting role) noexcept
        {
            switch(role)
            {
                case Reactant: return "reactant";
                case Product:  return "product";
            }
            return Core::Unknown;
        }


        Component:: ~Component() noexcept
        {
        }
        
        Component:: Component(const Component &_) noexcept :
        actor(_.actor),
        role(_.role)
        {
        }

        Component:: Component(const Actor & _actor,
                              const Acting  _role) noexcept :
        actor(_actor),
        role(_role)
        {
        }

        const String & Component:: key() const noexcept
        {
            return actor.sp.name;
        }

        const char *   Component:: side() const noexcept
        {
            return ActingToText(role);
        }

        
    }

}

