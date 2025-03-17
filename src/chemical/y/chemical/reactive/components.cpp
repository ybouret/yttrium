

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

        Components:: Components(const String * const xname, const size_t i) :
        Indexed(xname,i),
        Proxy<const ComponentsType>(),
        reac(Actor::AsComponentOnly),
        prod(Actor::AsComponentOnly),
        db()
        {
        }

        void Components:: use(const Role role, const unsigned nu, const Species &sp)
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

            // atomic add
            Actors &        actors    = *pActors;
            String          savedName = *actors.name;
            const Component component(role, actors(nu,sp) );
            try {
                if(!db.insert(component)) throw Specific::Exception( key().c_str(), "couldn't insert %s '%s'", component.roleText(), uid.c_str() );
            }
            catch(...)
            {
                savedName.swapWith( Coerce(*actors.name) );
                throw;
            }

        }


        void Components:: use(const Role role, const Species &sp)
        {
            use(role,1,sp);
        }


        std::ostream & operator<<(std::ostream &os, const Components &cm)
        {
            os << cm.name << Components::Separator << cm.reac.name << Components::Symbol << cm.prod.name;
            return os;
        }

        bool Components:: AreConnected(const Components &lhs, const Components &rhs)
        {
            ConstIterator it = lhs->begin();
            for(size_t n=lhs->size();n>0;--n,++it)
            {
                if(rhs->search(it->key)) return true;
            }
            return false;
        }

        void Components:: gatherSpeciesIn(AddressBook &book) const
        {
            for(ConstIterator it=db.begin();it!=db.end();++it)
            {
                book |= (*it)->sp;
            }
        }

    }

}


#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        bool Components:: neutral() const noexcept
        {
            apz sum = 0;

            for(const Actor *a=reac->head;a;a=a->next)
            {
                const apn nu = a->nu;
                const apz z  = a->sp.z;
                sum -= nu * z;
            }

            for(const Actor *a=prod->head;a;a=a->next)
            {
                const apn nu = a->nu;
                const apz z  = a->sp.z;
                sum += nu * z;
            }
            
            return 0 == sum;
        }


    }

}
