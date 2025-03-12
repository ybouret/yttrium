
#include "y/chemical/reactive/equilibria.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        EquilibriaType:: EquilibriaType() : EquilibriaSet(), Fragment()
        {
        }

        EquilibriaType:: ~EquilibriaType() noexcept
        {
        }
    }

}

#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Equilibria:: CallSign = "Equilibria";

        Equilibria:: ~Equilibria() noexcept
        {
        }

        Equilibria:: Equilibria() :
        Proxy<const EquilibriaType>(),
        db()
        {
        }
        

        size_t Equilibria:: nextIndex() const noexcept
        {
            return db.size()+1;
        }

        void Equilibria:: operator()(const Equilibrium::Pointer &eq)
        {
            const String &     key = eq->key();
            const char * const eid = key.c_str();

            if(latched)
                throw Specific::Exception(CallSign,"latched while adding '%s'", eid);

            if( db.search(key) )
                throw Specific::Exception(CallSign,"multiple '%s'",eid);

            if(eq->indx[TopLevel]!=nextIndex())
                throw Specific::Exception(CallSign, "invalid index for '%s'",eid);

            if(!(*eq)->latched)
                throw Specific::Exception(CallSign, "adding unlatched '%s",eid);
            assert(eq->reac.latched);
            assert(eq->prod.latched);

            if(!db.insert(eq))
                throw Specific::Exception(CallSign, "unexpected failure to add '%s'",eid);

            db.enroll(*eq);
        }

        Y_PROXY_IMPL(Equilibria, db);
    }

}


