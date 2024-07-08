
#include "y/chemical/reactive/equilibria.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Equilibria:: Equilibria() : 
        Proxy<const EqSet>(),
        db()
        {
        }

        Equilibria:: ~Equilibria() noexcept {}

        Equilibria::ConstInterface &Equilibria::surrogate() const noexcept { return db; }


        std::ostream & operator<<(std::ostream &os, const Equilibria &eqs)
        {
            os << '{';
            for(Equilibria::ConstIterator it=eqs->begin();it!=eqs->end();++it)
            {
                const Equilibrium &eq = **it;
                eqs.display(os,eq) << std::endl;
            }
            os << '}';
            return os;
        }

        Equilibrium & Equilibria:: append(Equilibrium * const eq)
        {
            assert(0!=eq);
            const Equilibrium::Ptr pEq = eq;
            assert(pEq->indx[TopLevel]==db.size()+1);
            if(!db.insert(pEq)) throw Specific::Exception("Chemical::Equilibria","mutliple <%s>", pEq->name.c_str());
            update(*pEq);
            return *eq;
        }


    }

}
