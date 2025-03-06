
#include "y/chemical/reactive/equilibrium/component.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * RoleToText(const Role role) noexcept
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
                              const Role    _role) noexcept :
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
            return RoleToText(role);
        }

        void Component:: viz(OutputStream            &fp,
                             const GraphViz::Vizible &eq,
                             const char * const       color) const
        {
            const GraphViz::Vizible *src = & actor.sp;
            const GraphViz::Vizible *tgt = & eq;


            switch(role)
            {
                case Reactant: break;
                case Product: Swap(src,tgt); break;
            }

            GraphViz::Vizible::Arrow(fp, src, tgt) << '[';
            if(actor.nu>1)
            {
                fp("label=\"%u\"", unsigned(actor.nu));
            }
            else
            {
                fp << "label=\"\"";
            }
            
            if(color) fp << ",color=" << color << ",fontcolor=" << color;
            GraphViz::Vizible::Endl(fp << ']');
        }
    }

}

