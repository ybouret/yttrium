

#include "y/chemical/reactive/components.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Components:: Symbol   = "<=>";
        const char         Components:: Separator;

        Y_PROXY_IMPL(Components,db)

        Components:: ~Components() noexcept
        {
            
        }

        Components:: Components(const String * const xname) :
        Entity(xname),
        Proxy<const ComponentsType>(),
        Latchable(),
        reac(Actor::AsComponentOnly),
        prod(Actor::AsComponentOnly)
        {
        }

        void Components:: operator()(const Role role, const unsigned nu, const Species &sp)
        {
            const String &uid = sp.key();
            if( latched )        throw Specific::Exception( key().c_str(), "latched while inserting %s '%s'", Component::RoleText(role), uid.c_str());
            if( db.search(uid) ) throw Specific::Exception( key().c_str(), "multiple '%s'", uid.c_str());

            Actors * pActors = 0;
            switch(role)
            {
                case Reactant: pActors = &Coerce(reac); break;
                case Product:  pActors = &Coerce(prod); break;
            }
            assert(0!=pActors);
            Actors &mine = *pActors;

            Actors          temporary(mine);
            const Component component(role,temporary(nu,sp));
            if(!db.insert(component)) throw Specific::Exception( key().c_str(), "couldn't insert %s '%s'", component.roleText(), uid.c_str() );

            mine.xch(temporary);
            
        }


        void Components:: operator()(const Role role, const Species &sp)
        {
            (*this)(role,1,sp);
        }


        std::ostream & operator<<(std::ostream &os, const Components &cm)
        {
            os << cm.name << Components::Separator << cm.reac.name << Components::Symbol << cm.prod.name;
            return os;
        }

    }

}
