

#include "y/chemical/reactive/components.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        ComponentsType::  ComponentsType() : ComponentsDB(), Latchable() {}
        ComponentsType:: ~ComponentsType() noexcept {}

    }

}

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Components:: Symbol   = "<=>";
        const char         Components:: Separator;

        Y_PROXY_IMPL(Components,db)


        void Components:: latch() noexcept
        {
            db.latch();
            Coerce(reac).latch();
            Coerce(prod).latch();

        }

        Components:: ~Components() noexcept
        {
            
        }

        Components:: Components(const String * const xname) :
        Entity(xname),
        Proxy<const ComponentsType>(),
        reac(Actor::AsComponentOnly),
        prod(Actor::AsComponentOnly),
        db()
        {
        }

        void Components:: operator()(const Role role, const unsigned nu, const Species &sp)
        {

            //------------------------------------------------------------------
            //
            //
            // sanity check
            //
            //
            //------------------------------------------------------------------
            const String &uid = sp.key();
            if( db.latched )     throw Specific::Exception( key().c_str(), "latched while inserting %s '%s'", Component::RoleText(role), uid.c_str());
            if( db.search(uid) ) throw Specific::Exception( key().c_str(), "multiple '%s'", uid.c_str());
            assert(!reac.latched);
            assert(!prod.latched);

            //------------------------------------------------------------------
            //
            //
            // select actors
            //
            //
            //------------------------------------------------------------------
            Actors * pActors = 0;
            switch(role)
            {
                case Reactant: pActors = &Coerce(reac); break;
                case Product:  pActors = &Coerce(prod); break;
            }
            assert(0!=pActors);
            Actors &mine = *pActors;

            //------------------------------------------------------------------
            //
            //
            // atomic build
            //
            //
            //------------------------------------------------------------------
            Actors          temporary(mine);                     // hard copy
            const Component component(role,temporary(nu,sp));    // new component to grown temporary
            if(!db.insert(component)) throw Specific::Exception( key().c_str(), "couldn't insert %s '%s'", component.roleText(), uid.c_str() );

            //------------------------------------------------------------------
            //
            //
            // temporary+db are valid
            //
            //
            //------------------------------------------------------------------
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
