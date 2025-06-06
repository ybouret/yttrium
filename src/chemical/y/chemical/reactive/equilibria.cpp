
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

        Equilibria::Iterator Equilibria:: begin() noexcept
        {
            return db.begin();
        }

        Equilibria::Iterator Equilibria:: end() noexcept
        {
            return db.end();
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

            if(!eq->neutral())
                throw Specific::Exception(CallSign, "adding not-neutral '%s",eid);

            if((*eq)->size()<=0)
                throw Specific::Exception(CallSign, "adding empty '%s",eid);

            assert(eq->reac.latched);
            assert(eq->prod.latched);

            if(!db.insert(eq))
                throw Specific::Exception(CallSign, "unexpected failure to add '%s'",eid);

            db.enroll(*eq);
        }

        Y_PROXY_IMPL(Equilibria, db);


        std::ostream & operator<<(std::ostream &os, const Equilibria &eqs)
        {
            os << '{';
            if(eqs->size()>0)
            {
                os << std::endl;
                for(Equilibria::ConstIterator it=eqs->begin();it!=eqs->end();++it)
                {
                    const Equilibrium &eq = **it;
                    eqs->display(os << '\t',eq, Coerce(eq).K(0)) << std::endl;
                }
            }
            os << '}';
            return os;
        }

        const Equilibrium & Equilibria:: operator[](const String &eid) const
        {
            const Equilibrium::Pointer * const pp = db.search(eid);
            if(!pp) throw Specific::Exception(CallSign,"no equilibrium '%s'", eid.c_str());
            return **pp;
        }

        const Equilibrium & Equilibria:: operator[](const char * const eid) const
        {
            const String _(eid);
            return (*this)[_];
        }
    }

}


